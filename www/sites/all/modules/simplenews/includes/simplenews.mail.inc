<?php

/**
 * @file
 * Simplenews email send and spool handling
 *
 * @ingroup simplenews
 */

/**
 * Send newsletter node to subscribers.
 *
 * @param integer or object $node Newsletter node to be sent. integer = nid; object = node object
 * @param array $accounts  account objects to send the newsletter to.
 *   account = object (
 *     snid     = subscription id, or 0 if no subscription record exists.
 *     tids     = array(tid) array of newsletter tid's.
 *     uid      = user id, or 0 if subscriber is anonymous user.
 *     mail     = user email address.
 *     name     = <empty>. Added for compatibility with user account object.
 *     language = language object. User-preferred or default language.
 *   )
 *   NOTE: either snid, mail or uid is required.
 */
function simplenews_send_node($node, $accounts = array()) {
  $mails = array();

  // We always start with an empty spooler
  simplenews_clear_spool_node($node);

  if (is_numeric($node)) {
    $node = node_load($node);
  }
  if (is_object($node)) {
    $from = _simplenews_set_from($node);
    $params['context']['node'] = $node;
    $params['from'] = $from;

    $node_data['tid'] = $node->simplenews['tid'];
    $node_data['nid'] = $node->nid;
    $node_data['vid'] = $node->vid;

    $sent_subscriber_count = 0;
    if (empty($accounts)) {
      // No accounts specified.  Write all active subscriber addresses to mail spool.
      db_query('
        INSERT INTO {simplenews_mail_spool}
        (mail, nid, vid, tid, status, timestamp)
        SELECT s.mail, %d, %d, t.tid, %d, %d
        FROM {simplenews_subscriptions} s
        INNER JOIN {simplenews_snid_tid} t
          ON s.snid = t.snid
        WHERE s.activated = 1
          AND t.tid = %d
          AND t.status = %d',
        $node->nid,
        $node->vid,
        SIMPLENEWS_SPOOL_PENDING,
        time(),
        $node->simplenews['tid'],
        SIMPLENEWS_SUBSCRIPTION_STATUS_SUBSCRIBED
      );
      $sent_subscriber_count = db_affected_rows();
    }
    else {
      // Get email address of specified accounts.
      foreach ($accounts as $account) {
        $account = simplenews_get_subscription($account);
        $mails[] = array('mail' => $account->mail);
      }
      $sent_subscriber_count = count($mails);
    }

    // Persist subscriber count now
    db_query("
      UPDATE {simplenews_newsletters}
      SET sent_subscriber_count = %d
      WHERE nid = %d",
      $sent_subscriber_count,
      $node->nid);

    // To send the newsletter, the node id and target email addresses
    // are stored in the spool.
    // When cron is not used the newsletter is send immediately to the emails
    // in the spool. When cron is used newsletters are send to addresses in the
    // spool during the next (and following) cron run.
    foreach ($mails as $mail) {
      $data = array_merge($node_data, $mail);
      simplenews_save_spool($data);
    }
    if (variable_get('simplenews_use_cron', TRUE) == FALSE) {
      simplenews_mail_spool($node_data['nid'], $node_data['vid'], 999999);
      simplenews_clear_spool();
      // Update sent status for newsletter admin panel.
      simplenews_send_status_update();
      drupal_set_message(t('Newsletter sent.'));
      simplenews_clear_spool();
    }
    else {
      drupal_set_message(t('Newsletter pending.'));
    }
  }
}

/**
 * Send test version of newsletter.
 *
 * @param integer or object $node Newsletter node to be sent. Integer = nid; Object = node object
 */
function simplenews_send_test($node) {
  if (is_numeric($node)) {
    $node = node_load($node);
  }
  if (is_object($node)) {
    // switch to anonymous user - needed in foreground spool sends, adopted from D7 drupal_cron_run().
    // see #471594 for issue about sending with higher permissions
    // note that private (non-public) fields might be invisible in newsletters now always.
    $original_user = $GLOBALS['user'];
    $GLOBALS['user'] = drupal_anonymous_user();

    // Send the test newsletter to the test address(es) specified in the node.
    // Build array of test email addresses
    $mails = explode(',', $node->simplenews['test_address']);

    // Send newsletter to test addresses.
    // Emails are send direct, not using the spool.
    $recipients = array('anonymous' => array(), 'user' => array());
    foreach ($mails as $mail) {
      $mail = trim($mail);
      if (!empty($mail)) {
        $account = _simplenews_user_load($mail);
        if ($account->uid) {
          $recipients['user'][] = $account->name . ' <' . $mail . '>';
        }
        else {
          $recipients['anonymous'][] = $mail;
        }
        $tmpres = simplenews_mail_mail($node->nid, $node->vid, $mail, 'test');
      }
    }
    if (count($recipients['user'])) {
      $recipients_txt = implode(', ', $recipients['user']);
      drupal_set_message(t('Test newsletter sent to user %recipient.', array('%recipient' => $recipients_txt)));
    }
    if (count($recipients['anonymous'])) {
      $recipients_txt = implode(', ', $recipients['anonymous']);
      drupal_set_message(t('Test newsletter sent to anonymous %recipient.', array('%recipient' => $recipients_txt)));
    }

    // Restore the user.
    $GLOBALS['user'] = $original_user;
  }
}

/**
 * Send a node to an email address.
 *
 * @param $nid node id of newsletter node
 * @param $vid revision id of newsletter node
 * @param $mail target email address
 * @param $key email key [node|test]
 * @param $reset
 *   Reset static cache.
 *
 * @return TRUE if email is successfully delivered by php mail()
 */
function simplenews_mail_mail($nid, $vid, $mail, $key = 'node', $reset = FALSE) {
  static $cache = array();

  if ($reset) {
    $cache = array();
    return;
  }

  // Get subscription data for recipient and language
  $account = new stdClass();
  $account->mail = $mail;
  $subscription = simplenews_get_subscription($account);
  $params['context']['account'] = $subscription;

  // Get node data for the mail
  // Because node_load() only caches the most recent node we cache here based on nid and vid.
  if (isset($cache["$nid:$vid"])) {
    $node = $cache["$nid:$vid"];
  }
  else {
    $node = node_load($nid, $vid);
    $cache["$nid:$vid"] = $node;
  }

  if (is_object($node)) {
    $params['from'] = _simplenews_set_from($node);
    $params['context']['newsletter'] = taxonomy_get_term($node->simplenews['tid']);
    $params['context']['node'] = $node;

    // Send mail
    if (module_exists('mimemail')) {
      // If mimemail module is installed ALL emails are send via this module.
      // drupal_mail() builds the content of the email but does NOT send.
      $message = drupal_mail('simplenews', $key, $subscription->mail, $subscription->language, $params, $params['from']['formatted'], FALSE);
      $to = isset($message['params']['context']['account']) ? $message['params']['context']['account'] : $message['to'];
      $plain = $message['params']['context']['node']->simplenews['s_format'] == 'plain' ? TRUE : NULL;
      $mimemail_result = mimemail(
        $message['from'],
        $to,
        $message['subject'],
        $message['body'],
        $plain,
        $message['headers'],
        $plain ? $message['body'] : simplenews_html_to_text($message['body'], TRUE),
        isset($message['params']['context']['node']->files) ? $message['params']['context']['node']->files : array(),
        $message['id']
      );
      // Mimemail has changed its API (see http://drupal.org/node/808518) but we keep backward compatibility
      if (is_array($mimemail_result)) {
        $message = $mimemail_result;
      } else {
        $message['result'] = $mimemail_result;
      }
    }
    else {
      $message = drupal_mail('simplenews', $key, $subscription->mail, $subscription->language, $params, $params['from']['formatted'], TRUE);
    }

    // Log sent result in watchdog.
    if (variable_get('simplenews_debug', FALSE)) {
      $via_mimemail = '';
      if (module_exists('mimemail')) {
        $via_mimemail = t('Sent via Mime Mail');
      }
      //TODO Add line break before %mimemail.
      if ($message['result']) {
        watchdog('simplenews', 'Outgoing email. Message type: %type<br />Subject: %subject<br />Recipient: %to %mimemail', array('%type' => $key, '%to' => $message['to'], '%subject' => $message['subject'], '%mimemail' => $via_mimemail), WATCHDOG_DEBUG);
      }
      else {
        watchdog('simplenews', 'Outgoing email failed. Message type: %type<br />Subject: %subject<br />Recipient: %to %mimemail', array('%type' => $key, '%to' => $message['to'], '%subject' => $message['subject'], '%mimemail' => $via_mimemail), WATCHDOG_ERROR);
      }
    }

    // Build array of sent results for spool table and reporting.
    if ($message['result']) {
      $message['result'] = array(
        'status' => SIMPLENEWS_SPOOL_DONE,
        'error' => FALSE,
      );
    }
    else {
      // This error may be caused by faulty mailserver configuration or overload.
      // Some systems try to contact the target server immediately and return error if the domain or mail account is nonexistent. See #780132.
      // For now stop sending as this will result in an infinite loop.
      // @todo: Add counter of tries and abort after N tries. Build stats.
      $message['result'] = array(
        'status' => SIMPLENEWS_SPOOL_HOLD,
        'error' => TRUE,
      );
    }

  }
  else {
    // Node could not be loaded. The node is probably deleted while pending to be sent.
    // This error is not recoverable, mark "done".
    $message['result'] = array(
      'status' => SIMPLENEWS_SPOOL_DONE,
      'error' => TRUE,
    );
    watchdog('simplenews', 'Newsletter not sent: newsletter issue does not exist (nid = @nid; vid = @vid).', array('@nid' => $nid, '@vid' => $vid), WATCHDOG_ERROR);
  }

  return isset($message['result']) ? $message['result'] : FALSE;
}

/**
 * Send simplenews newsletters from the spool.
 *
 * Individual newsletter emails are stored in database spool.
 * Sending is triggered by cron or immediately when the node is saved.
 * Mail data is retrieved from the spool, rendered and send one by one
 * If sending is successful the message is marked as send in the spool.
 *
 * TODO: Redesign API to allow language counter in multilingual sends.
 */
function simplenews_mail_spool($nid = NULL, $vid = NULL, $limit = NULL) {
  // Send pending messages from database cache
  // A limited number of mails is retrieved from the spool
  $limit = isset($limit) ? $limit : variable_get('simplenews_throttle', 20);
  if ($messages = simplenews_get_spool(array(SIMPLENEWS_SPOOL_PENDING, SIMPLENEWS_SPOOL_IN_PROGRESS), $limit)) {
    $count_fail = $count_success = 0;

    // switch to anonymous user - needed in foreground spool sends, adopted from D7 drupal_cron_run().
    // see #471594 for issue about sending with higher permissions
    // note that private (non-public) fields might be invisible in newsletters now always.
    $original_user = $GLOBALS['user'];
    $GLOBALS['user'] = drupal_anonymous_user();

    // Get PHP maximum execution time. 30 seconds default.
    $max_execution_time = ini_get('max_execution_time') ? ini_get('max_execution_time') : SIMPLENEWS_MAX_EXECUTION_TIME;

    _simplenews_measure_usec(TRUE);

    $check_counter = 0;
    foreach ($messages as $key => $message) {
      $result = simplenews_mail_mail($message['nid'], $message['vid'], $message['mail']);

      // Update spool status.
      // This is not optimal for performance but prevents duplicate emails
      // in case of PHP execution time overrun.
      simplenews_update_spool(array($key), $result);
      if ($result['status'] == SIMPLENEWS_SPOOL_DONE) {
        $count_success++;
      }
      if ($result['error']) {
        $count_fail++;
      }

      // Check every n emails if we exceed the limit.
      // When PHP maximum execution time is almost elapsed we interrupt
      // sending. The remainder will be sent during the next cron run.
      if (++$check_counter >= SIMPLENEWS_SEND_CHECK_INTERVAL) {
        $check_counter = 0;
        // Break the sending if a percentage of max execution time was exceeded.
        $elapsed = _simplenews_measure_usec();
        if ($max_execution_time
          && $elapsed > SIMPLENEWS_SEND_TIME_LIMIT * $max_execution_time) {
          watchdog('simplenews', 'Sending interrupted: PHP maximum execution time almost exceeded. Remaining newsletters will be sent during the next cron run. If this warning occurs regularly you should reduce the !cron_throttle_setting.', array('!cron_throttle_setting' => l(t('Cron throttle setting'), 'admin/settings/simplenews/mail')), WATCHDOG_WARNING);
          break;
        }
      }
    }

    // Report sent result and elapsed time. On Windows systems getrusage() is
    // not implemented and hence no elapsed time is available.
    if (function_exists('getrusage')) {
      watchdog('simplenews', '%success emails sent in %sec seconds, %fail failed sending.', array('%success' => $count_success, '%sec' => round(_simplenews_measure_usec(), 1), '%fail' => $count_fail));
    }
    else {
      watchdog('simplenews', '%success emails sent, %fail failed.', array('%success' => $count_success, '%fail' => $count_fail));
    }

    variable_set('simplenews_last_cron', time());
    //@todo: set mail_sent from last batch
    variable_set('simplenews_last_sent', $count_success + $count_fail);

    // Restore the user.
    $GLOBALS['user'] = $original_user;
  }
}

/**
 * Save mail message in mail cache table.
 *
 * @param array $message data array to be stored
 *  $message['mail']
 *  $message['nid']
 *  $message['vid']
 *  $message['tid']
 *  $message['status']  (Default: 1 = pending)
 *  $message['time']    (default: current unix timestamp)

 * @param array $message Mail message array
 */
function simplenews_save_spool($message) {
  $status = isset($message['status']) ? $message['status'] : SIMPLENEWS_SPOOL_PENDING;
  $time = isset($message['time']) ? $message['time'] : time();

  db_query("
    INSERT INTO {simplenews_mail_spool}
      (mail, nid, vid, tid, status, timestamp)
    VALUES ('%s', %d, %d, %d, %d, %d)",
    $message['mail'],
    $message['nid'],
    $message['vid'],
    $message['tid'],
    $status, $time);
}

/*
 * Returns the expiration time for IN_PROGRESS status.
 *
 * @return int Time the message allocation expires and is resent
 */
function simplenews_get_expiration_time() {
  $timeout = variable_get('simplenews_spool_progress_expiration', 3600);
  $expiration_time = time() - $timeout;
  return $expiration_time;
}

/**
 * This function allocates messages to be sent in current run.
 *
 * Drupal acquire_lock quarantees that no concurrency issue happend.
 * If message status is SIMPLENEWS_SPOOL_IN_PROGRESS but the maximum send time
 * has expired the message id will be returned as a message which is not allocated
 * to another process.
 * MessageIDs to be sent in current run are returned.
 *
 * @param array $status of data to be retrieved
 *  SIMPLENEWS_SPOOL_HOLD, _PENDING, _DONE, _IN_PROGRESS
 * @param integer $limit The maximum number of mails to load from the spool
 *
 * @return array Mail message array
 *  $message['msid']
 *  $message['mail']
 *  $message['nid']
 *  $message['vid']
 *  $message['tid']
 *  $message['status']
 *  $message['time']
 */
function simplenews_get_spool($status, $limit = NULL) {
  $messages = array();
  $clauses = array();
  $params = array();

  if (!is_array($status)) {
    $status = array($status);
  }

  foreach ($status as $s) {
    if ($s == SIMPLENEWS_SPOOL_IN_PROGRESS) {
      // Select messages which are allocated by another process, but where the maximum send time has expired.
      $clauses[] = '(s.status = %d AND s.timestamp < %d)';
      $params[] = $s;
      $params[] = simplenews_get_expiration_time();
    }
    else {
      $clauses[] = 's.status = %d';
      $params[] = $s;
    }
  }

  $query = "SELECT *
    FROM {simplenews_mail_spool} s
    WHERE " . implode(' OR ', $clauses) . "
    ORDER BY s.timestamp ASC";

  /* BEGIN CRITICAL SECTION */
  // The semaphore ensures that multiple processes get different message ID's. So there could not occur any duplicate messages.

  if (lock_acquire('simplenews_acquire_mail')) {
    // Get message id's
    // Allocate messages
    if (is_numeric($limit)) {
      $result = db_query_range($query, $params, 0, $limit);
    }
    else {
      $result = db_query($query, $params);
    }
    while ($message = db_fetch_array($result)) {
      $messages[$message['msid']] = $message;
    }
    if (count($messages) > 0) {
    // Set the state and the timestamp of the messages
      simplenews_update_spool(array_keys($messages), array('status' => SIMPLENEWS_SPOOL_IN_PROGRESS));
    }

    lock_release('simplenews_acquire_mail');
  }

  /* END CRITICAL SECTION */

  return $messages;
}

/**
 * Update status of mail data in spool table.
 *
 * Time stamp is set to current time.
 *
 * @param array $msids
 *   Mail spool id of record to be updated
 * @param array $result
 *   Array containing email sent result
 *    'status' => SIMPLENEWS_SPOOL_HOLD, _PENDING, _DONE, _IN_PROGRESS
 *    'error' => error id (optional; defaults to '')
 */
function simplenews_update_spool($msids, $result) {
  $params[] = $result['status'];
  $params[] = isset($result['error']) ? $result['error'] : FALSE;
  $params[] = time();
  $params = array_merge($params, $msids);
  db_query("
    UPDATE {simplenews_mail_spool}
    SET status = %d,
      error = %d,
      timestamp = %d
    WHERE msid IN(" . db_placeholders($msids, 'int') . ")",
    $params);
}

/**
 * Count data in mail spool table.
 *
 * @param integer $nid newsletter node id
 * @param integer $vid newsletter revision id
 * @param string $status email sent status
 *
 * @return integer count of mail spool elements wich owns the attributes passed in as params
 */
function simplenews_count_spool($nid, $vid, $status = array(SIMPLENEWS_SPOOL_PENDING, SIMPLENEWS_SPOOL_IN_PROGRESS)) {
  $clauses = array();
  $params = array();

  if (!is_array($status)) {
    $status = array($status);
  }

  foreach ($status as $s) {
    $clauses[] = 's.status = %d';
    $params[] = $s;
  }
  $params[] = $nid;
  $params[] = $vid;

  $query = "SELECT COUNT(nid)
    FROM {simplenews_mail_spool} s
    WHERE (" . implode(' OR ', $clauses) . ")
    AND nid = %d
    AND vid = %d";

  return db_result(db_query($query, $params));
}

/**
 * Remove records from mail spool table.
 *
 * All records with status 'send' and time stamp before the expiration date
 * are removed from the spool.
 *
 * @return Count deleted
 */
function simplenews_clear_spool() {
  $expiration_time = time() - variable_get('simplenews_spool_expire', 0) * 86400;
  db_query("
    DELETE FROM {simplenews_mail_spool}
    WHERE status = %d
      AND timestamp <= %d",
    SIMPLENEWS_SPOOL_DONE, $expiration_time);
  $deleted_count = db_affected_rows();
  return $deleted_count;
}

/**
 * Remove records from mail spool table for node.
 *
 * @return Count deleted
 */
function simplenews_clear_spool_node($node) {
  db_query("
    DELETE FROM {simplenews_mail_spool}
    WHERE nid = %d",
    $node->nid);
  $deleted_count = db_affected_rows();
  return $deleted_count;
}

/**
 * Update newsletter sent status.
 *
 * Set newsletter sent status based on email sent status in spool table.
 * Translated and untranslated nodes get a different treatment.
 *
 * The spool table holds data for emails to be sent and (optionally)
 * already send emails. The simplenews_newsletters table contains the overall
 * sent status of each newsletter issue (node).
 * Newsletter issues get the status 'pending' when sending is initiated. As
 * long as unsent emails exist in the spool, the status of the newsletter remains
 * 'unsent'. When no pending emails are found the newsletter status is set to 'sent'.
 *
 * Translated newsletters are a group of nodes that share the same tnid ({node}.tnid).
 * Only one node of the group is found in the spool, but all nodes should share
 * the same state. Therefore they are checked for the combined number of emails
 * in the spool.
 */
function simplenews_send_status_update() {
  $counts = array();  // number pending of emails in the spool
  $sum = array();     // sum of emails in the spool per tnid (translation id)
  $send = array();    // nodes with the status 'send'

  // For each pending newsletter count the number of pending emails in the spool.
  $result = db_query("
    SELECT s.nid, n.vid, s.tid, n.tnid
    FROM {simplenews_newsletters} s
    JOIN {node} n
      ON s.nid = n.nid
    WHERE s.s_status = %d",
    SIMPLENEWS_STATUS_SEND_PENDING);
  while ($newsletter = db_fetch_object($result)) {
    // nid-vid are combined in one unique key.
    $counts[$newsletter->tnid][$newsletter->nid . '-' . $newsletter->vid] = simplenews_count_spool($newsletter->nid, $newsletter->vid);
  }

  // Determine which nodes are sent per translation group and per individual node.
  foreach ($counts as $tnid => $node_count) {
    // The sum of emails per tnid is the combined status result for the group of translated nodes.
    // Untranslated nodes have tnid == 0 which will be ignored later.
    $sum[$tnid] = array_sum($node_count);
    foreach ($node_count as $nidvid => $count) {
      // Translated nodes (tnid != 0)
      if ($tnid != '0' && $sum[$tnid] == '0') {
          $send[] = $nidvid;
      }
      // Untranslated nodes (tnid == 0)
      elseif ($tnid == '0' && $count == '0') {
        $send[] = $nidvid;
      }
    }
  }

  // Update overall newsletter status
  if (!empty($send)) {
    foreach ($send as $nidvid) {
      // Split the combined key 'nid-vid'
      $nid = strtok($nidvid, '-');
      $vid = strtok('-');
      db_query("
        UPDATE {simplenews_newsletters}
        SET s_status = '%s'
        WHERE nid = %d
          AND vid = %d",
        SIMPLENEWS_STATUS_SEND_READY,
        $nid,
        $vid);
    }
  }
}

/**
 * Build header array with priority and receipt confirmation settings.
 *
 * @param $node: node object
 * @param $from: from email address
 *
 * @return Header array with priority and receipt confirmation info
 */
function _simplenews_headers($node, $from) {
  $headers = array();

  // If receipt is requested, add headers.
  if ($node->simplenews['receipt']) {
    $headers['Disposition-Notification-To'] = $from;
    $headers['X-Confirm-Reading-To'] = $from;
  }

  // Add priority if set.
  switch ($node->simplenews['priority']) {
    case SIMPLENEWS_PRIORITY_HIGHEST:
      $headers['Priority'] = 'urgent';
      $headers['X-Priority'] = '1';
      $headers['X-MSMail-Priority'] = 'Highest';
      break;
    case SIMPLENEWS_PRIORITY_HIGH:
      $headers['Priority'] = 'urgent';
      $headers['X-Priority'] = '2';
      $headers['X-MSMail-Priority'] = 'High';
      break;
    case SIMPLENEWS_PRIORITY_NORMAL:
      $headers['Priority'] = 'normal';
      $headers['X-Priority'] = '3';
      $headers['X-MSMail-Priority'] = 'Normal';
      break;
    case SIMPLENEWS_PRIORITY_LOW:
      $headers['Priority'] = 'non-urgent';
      $headers['X-Priority'] = '4';
      $headers['X-MSMail-Priority'] = 'Low';
      break;
    case SIMPLENEWS_PRIORITY_LOWEST:
      $headers['Priority'] = 'non-urgent';
      $headers['X-Priority'] = '5';
      $headers['X-MSMail-Priority'] = 'Lowest';
      break;
  }

  // Add general headers
  $headers['Precedence'] = 'bulk';

  return $headers;
}

/**
 * Build formatted from-name and email for a mail object.
 *
 * Each newsletter (series; tid) can have a different from address.
 * The from name and address depend on the newsletter term tid which is included in the $node object
 *
 * @param object $node Node object of a simplenews newsletter
 *
 * @return array [address] = from address; [formatted] = formatted from name and address
 */
function _simplenews_set_from($node = NULL) {
  $address_default = variable_get('site_mail', ini_get('sendmail_from'));
  $name_default = variable_get('site_name', 'Drupal');

  if (isset($node->simplenews['tid'])) {
    $address = variable_get('simplenews_from_address_' . $node->simplenews['tid'], variable_get('simplenews_from_address', $address_default));
    $name = variable_get('simplenews_from_name_' . $node->simplenews['tid'], variable_get('simplenews_from_name', $name_default));
  }
  else {
    $address = variable_get('simplenews_from_address', $address_default);
    $name = variable_get('simplenews_from_name', $name_default);
  }

  // Windows based PHP systems don't accept formatted email addresses.
  $formatted_address = drupal_substr(PHP_OS, 0, 3) == 'WIN' ? $address : '"'. addslashes(mime_header_encode($name)) .'" <'. $address .'>';

  return array(
    'address' => $address,
    'formatted' => $formatted_address,
  );
}

/**
 * HTML to text conversion for HTML and special characters.
 *
 * Converts some special HTMLcharacters in addition to drupal_html_to_text()
 *
 * @param string $text Source text with HTML and special characters
 * @param boolean $inline_hyperlinks
 *   TRUE: URLs will be placed inline.
 *   FALSE: URLs will be converted to numbered reference list.
 * @return string Target text with HTML and special characters replaced
 */
function simplenews_html_to_text($text, $inline_hyperlinks = TRUE) {
  // By replacing <a> tag by only its URL the URLs will be placed inline
  // in the email body and are not converted to a numbered reference list
  // by drupal_html_to_text().
  // URL are converted to absolute URL as drupal_html_to_text() would have.
  if ($inline_hyperlinks) {
    $pattern = '@<a[^>]+?href="([^"]*)"[^>]*?>(.+?)</a>@is';
    $text = preg_replace_callback($pattern, '_simplenews_absolute_mail_urls', $text);
  }

  // Replace some special characters before performing the drupal standard conversion.
  $preg = _simplenews_html_replace();
  $text = preg_replace(array_keys($preg), array_values($preg), $text);

  // Perform standard drupal html to text conversion.
  return drupal_html_to_text($text);
}

/**
 * Helper function for simplenews_html_to_text().
 *
 * Replaces URLs with absolute URLs.
 */
function _simplenews_absolute_mail_urls($match) {
  global $base_url, $base_path;
  static $regexp;
  $url = $label = '';

  if ($match) {
    if (empty($regexp)) {
      $regexp = '@^' . preg_quote($base_path, '@') . '@';
    }
    list(, $url, $label) = $match;
    $url = strpos($url, '://') ? $url : preg_replace($regexp, $base_url .'/', $url);

    // If the link is formed by Drupal's URL filter, we only return the URL.
    // The URL filter generates a label out of the original URL.
    if (strpos($label, '...') === drupal_strlen($label) - 3) {
      // Remove ellipsis from end of label.
      $label = drupal_substr($label, 0, drupal_strlen($label) - 3);
    }
    if (strpos($url, $label) !== FALSE) {
      return $url;
    }
    return $label . ' ' . $url;
  }
}

/**
 * Helper function for simplenews_html_to_text().
 *
 * List of preg* regular expression patterns to search for and replace with
 */
function _simplenews_html_replace() {
  return array(
    '/&quot;/i'  => '"',
    '/&gt;/i'  => '>',
    '/&lt;/i'  => '<',
    '/&amp;/i'   => '&',
    '/&copy;/i'  => '(c)',
    '/&trade;/i' => '(tm)',
    '/&#8220;/'  => '"',
    '/&#8221;/'  => '"',
    '/&#8211;/'  => '-',
    '/&#8217;/'  => "'",
    '/&#38;/'  => '&',
    '/&#169;/'   => '(c)',
    '/&#8482;/'  => '(tm)',
    '/&#151;/'   => '--',
    '/&#147;/'   => '"',
    '/&#148;/'   => '"',
    '/&#149;/'   => '*',
    '/&reg;/i'   => '(R)',
    '/&bull;/i'  => '*',
    '/&euro;/i'  => 'Euro ',
    '/&frasl;/i' => '/',
    // put a space before cell content, avoid collapse
    '#(</(?:th|td)[^>]*>)#i'  => ' $1',
    // make sure <tr> represents a line
    '#(</tr[^>]*>)#i'         => '<br />$1',
  );
}

/**
 * Helper function to measure PHP execution time in microseconds.
 *
 * @param bool $start TRUE reset the time and start counting.
 * @return float: elapsed PHP execution time since start.
 */
function _simplenews_measure_usec($start = FALSE) {
  // Windows systems don't implement getrusage(). There is no alternative.
  if (!function_exists('getrusage')) {
    return 0;
  }

  static $start_time;

  $usage = getrusage();
  $now = (float)($usage["ru_utime.tv_sec"] . '.' . $usage["ru_utime.tv_usec"]);

  if ($start) {
    $start_time = $now;
    return 0;
  }
  return $now - $start_time;
}

