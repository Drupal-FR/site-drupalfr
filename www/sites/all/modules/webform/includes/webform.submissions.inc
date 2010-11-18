<?php
// $Id: webform.submissions.inc,v 1.18.2.10 2010/08/15 16:04:56 quicksketch Exp $

/**
 * @file
 * This file is loaded when handling submissions, either submitting new,
 * editing, or viewing. It also contains all CRUD functions for submissions.
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

/**
 * Given an array of submitted values, flatten it into data for a submission.
 */
function webform_submission_data($node, $submitted) {
  $data = array();

  foreach ($submitted as $cid => $values) {
    // Don't save pagebreaks as submitted data.
    if ($node->webform['components'][$cid]['type'] == 'pagebreak') {
      continue;
    }

    if (is_array($values)) {
      $data[$cid]['value'] = $values;
    }
    else {
      $data[$cid]['value'][0] = $values;
    }
  }

  return $data;
}

function webform_submission_update($node, $submission) {
  // Allow other modules to modify the submission before saving.
  foreach (module_implements('webform_submission_presave') as $module) {
    $function = $module . '_webform_submission_presave';
    $function($node, $submission);
  }

  // Update the main submission info.
   $result = db_query("UPDATE {webform_submissions} SET uid = %d, submitted = %d, remote_addr = '%s', is_draft = %d WHERE sid = %d", $submission->uid, $submission->submitted, $submission->remote_addr, $submission->is_draft, $submission->sid);

  // If is draft, only delete data for components submitted, to
  // preserve any data from form pages not visited in this submission.
  if ($submission->is_draft) {
    $submitted_cids = array_keys($submission->data);
    db_query("DELETE FROM {webform_submitted_data} WHERE sid = %d AND cid IN (" . implode(', ', $submitted_cids) . ")", $submission->sid);
  }
  else {
    db_query("DELETE FROM {webform_submitted_data} WHERE sid = %d", $submission->sid);
  }

  // Then re-add submission data to the database.
  $submission->is_new = FALSE;
  webform_submission_insert($node, $submission);

  module_invoke_all('webform_submission_update', $node, $submission);

  return $submission->sid;
}

function webform_submission_insert($node, $submission) {
  // The submission ID may already be set if being called as an update.
  if (!isset($submission->sid) && (!isset($submission->is_new) || $submission->is_new == FALSE)) {
    // Allow other modules to modify the submission before saving.
    foreach (module_implements('webform_submission_presave') as $module) {
      $function = $module . '_webform_submission_presave';
      $function($node, $submission);
    }

    $result = db_query("INSERT INTO {webform_submissions} (nid, uid, submitted, remote_addr, is_draft) VALUES (%d, %d, %d, '%s', %d)", $node->nid, $submission->uid, $submission->submitted, $submission->remote_addr, $submission->is_draft);
    $submission->sid = db_last_insert_id('webform_submissions', 'sid');
    $is_new = TRUE;
  }

  foreach ($submission->data as $cid => $values) {
    foreach ($values['value'] as $delta => $value) {
      db_query("INSERT INTO {webform_submitted_data} (nid, sid, cid, no, data) VALUES (%d, %d, %d, '%s', '%s')", $node->nid, $submission->sid, $cid, $delta, $value);
    }
  }

  // Invoke the insert hook after saving all the data.
  if (isset($is_new)) {
    module_invoke_all('webform_submission_insert', $node, $submission);
  }

  return $submission->sid;
}

/**
 * Delete a single submission.
 *
 * @param $nid
 *   ID of node for which this webform was submitted.
 * @param $sid
 *   ID of submission to be deleted (from webform_submitted_data).
 */
function webform_submission_delete($node, $submission) {
  // Iterate through all components and let each do cleanup if necessary.
  foreach ($node->webform['components'] as $cid => $component) {
    if (isset($submission->data[$cid])) {
      webform_component_invoke($component['type'], 'delete', $component, $submission->data[$cid]['value']);
    }
  }

  // Delete any anonymous session information.
  if (isset($_SESSION['webform_submission'][$submission->sid])) {
    unset($_SESSION['webform_submission'][$submission->sid]);
  }

  db_query('DELETE FROM {webform_submitted_data} WHERE nid = %d AND sid = %d', $node->nid, $submission->sid);
  db_query('DELETE FROM {webform_submissions} WHERE nid = %d AND sid = %d', $node->nid, $submission->sid);

  module_invoke_all('webform_submission_delete', $node, $submission);
}

/**
 * Confirm form to delete a single form submission.
 *
 * @param $form_state
 *   The current form state.
 * @param $node
 *   The node for which this webform was submitted.
 * @param $submission
 *   The submission to be deleted (from webform_submitted_data).
 */
function webform_submission_delete_form($form_state, $node, $submission) {
  webform_set_breadcrumb($node, $submission);

  // Keep the NID and SID in the same location as the webform_client_form().
  // This helps mollom identify the same fields when deleting a submission.
  $form['#tree'] = TRUE;
  $form['details']['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['details']['sid'] = array(
    '#type' => 'value',
    '#value' => $submission->sid,
  );

  $question = t('Are you sure you want to delete this submission?');

  if (isset($_GET['destination'])) {
    $destination = $_GET['destination'];
  }
  elseif (user_access('access all webform results') || ($node->uid == $user->uid && user_access('access own webform results'))) {
    $destination = 'node/' . $node->nid . '/webform-results';
  }
  else {
    $destination = 'node/' . $node->nid . '/submissions';
  }

  return confirm_form($form, NULL, $destination, $question, t('Delete'), t('Cancel'));
}

function webform_submission_delete_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['details']['nid']);
  $submission = webform_get_submission($form_state['values']['details']['nid'], $form_state['values']['details']['sid']);
  webform_submission_delete($node, $submission);
  drupal_set_message(t('Submission deleted.'));

  $form_state['redirect'] = 'node/' . $node->nid . '/webform-results';
}

/**
 * Menu title callback; Return the submission number as a title.
 */
function webform_submission_title($node, $submission) {
  return t('Submission #@sid', array('@sid' => $submission->sid));
}

/**
 * Menu callback; Present a Webform submission page for display or editing.
 */
function webform_submission_page($node, $submission, $format) {
  global $user;
  webform_set_breadcrumb($node, $submission);

  if ($format == 'form') {
    $output = drupal_get_form('webform_client_form_' . $node->nid, $node, $submission);
  }
  else {
    $output = webform_submission_render($node, $submission, NULL, $format);
  }

  // Determine the mode in which we're displaying this submission.
  $mode = ($format != 'form') ? 'display' : 'form';
  if (preg_match("!^q=print/!", $_SERVER['QUERY_STRING'])) {
    $mode = 'print';
  }
  if (preg_match("!^q=printpdf/!", $_SERVER['QUERY_STRING'])) {
    $mode = 'pdf';
  }

  // Add navigation for administrators.
  if (user_access('access all webform results') || (user_access('access own webform results') && $user->uid == $node->uid)) {
    $navigation = ($mode != 'pdf') ? theme('webform_submission_navigation', $node, $submission, $mode) : NULL;
    $information = theme('webform_submission_information', $node, $submission);
  }
  else {
    $navigation = NULL;
    $information = NULL;
  }

  // Disable the page cache for anonymous users viewing or editing submissions.
  if (!$user->uid) {
    webform_disable_page_cache();
  }

  return theme('webform_submission_page', $node, $submission, $output, $navigation, $information);
}

/**
 * Print a Webform submission for display on a page or in an e-mail.
 */
function webform_submission_render($node, $submission, $email, $format) {
  $component_tree = array();
  $renderable = array();
  $page_count = 1;
  $excluded_components = isset($email) ? $email['excluded_components'] : array();

  // Meta data that may be useful for modules implementing
  // hook_webform_submission_render_alter().
  $renderable['#node'] = $node;
  $renderable['#submission'] = $submission;
  $renderable['#email'] = $email;
  $renderable['#format'] = $format;

  _webform_components_tree_build($node->webform['components'], $component_tree, 0, $page_count);

  // Recursively add components to the form.
  foreach ($component_tree['children'] as $cid => $component) {
    if (!in_array($cid, $excluded_components) && _webform_client_form_rule_check($node, $component, $component['page_num'], NULL, $submission)) {
      _webform_client_form_add_component($node, $component, NULL, $renderable, $renderable, NULL, $submission, $format);
    }
  }

  drupal_alter('webform_submission_render', $renderable);
  return drupal_render($renderable);
}

/**
 * Return all the submissions for a particular node.
 *
 * @param $filters
 *   An array of filters to apply to this query. Usually in the format
 *   array('nid' => $nid, 'uid' => $uid). A single integer may also be passed
 *   in, which will be equivalent to specifying a $nid filter.
 * @param $header
 *   If the results of this fetch will be used in a sortable
 *   table, pass the array header of the table.
 * @param $pager_count
 *   Optional. The number of submissions to include in the results.
 */
function webform_get_submissions($filters = array(), $header = NULL, $pager_count = 0) {
  $submissions = array();

  if (!is_array($filters)) {
    $filters = array('nid' => $filters);
  }

  // UID filters need to be against a specific table.
  if (isset($filters['uid'])) {
    $filters['u.uid'] = $filters['uid'];
    unset($filters['uid']);
  }

  // No need to find SIDs if it was given to us.
  if (isset($filters['sid'])) {
    $sids = array($filters['sid']);
  }
  // Build the list of SIDs that need to be retrieved.
  else {
    $arguments = array_values($filters);
    $where = array();
    foreach ($filters as $column => $value) {
      $where[] = $column . ' = ' . (is_numeric($value) ? '%d' : "'%s'");
    }

    if (isset($filters['uid']) && $filters['uid'] === 0 && isset($_SESSION['webform_submission'])) {
      $anonymous_sids = array_keys($_SESSION['webform_submission']);
      if (count($anonymous_sids)) {
        $placeholders = array_fill(0, count($anonymous_sids), "%d");
        $where[] = 'sid IN (' . implode(',', $placeholders) . ')';
        $arguments = array_merge($arguments, $anonymous_sids);
      }
    }

    $where_clause = implode(' AND ', $where);
    $pager_query = 'SELECT sid FROM {webform_submissions} s LEFT JOIN {users} u ON u.uid = s.uid WHERE ' . $where_clause;

    if (is_array($header)) {
      $pager_query .= tablesort_sql($header);
    }
    else {
      $pager_query .= ' ORDER BY sid ASC';
    }

    if ($pager_count) {
      $result = pager_query($pager_query, $pager_count, 0, NULL, $arguments);
    }
    else {
      $result = db_query($pager_query, $arguments);
    }

    $sids = array();
    while ($row = db_fetch_object($result)) {
      $sids[] = $row->sid;
      $submissions[$row->sid] = FALSE;
    }
  }

  // If there are no submissions being retrieved, return an empty array.
  if (empty($sids)) {
    return $submissions;
  }

  // Query the required submission data.
  $query = 'SELECT s.*, sd.cid, sd.no, sd.data, u.name, u.mail, u.status ' .
           'FROM {webform_submissions} s ' .
           'LEFT JOIN {webform_submitted_data} sd ON sd.sid = s.sid ' .
           'LEFT JOIN {users} u ON u.uid = s.uid ' .
           'WHERE s.sid IN (' . implode($sids, ',') . ') ' .
           'ORDER BY sd.sid ASC, sd.cid ASC, sd.no ASC';

  $result = db_query($query);

  // Convert the queried rows into submissions.
  $previous = array();
  while ($row = db_fetch_object($result)) {
    if ($row->sid != $previous) {
      $submissions[$row->sid] = new stdClass();
      $submissions[$row->sid]->sid = $row->sid;
      $submissions[$row->sid]->nid = $row->nid;
      $submissions[$row->sid]->submitted = $row->submitted;
      $submissions[$row->sid]->remote_addr = $row->remote_addr;
      $submissions[$row->sid]->uid = $row->uid;
      $submissions[$row->sid]->name = $row->name;
      $submissions[$row->sid]->is_draft = $row->is_draft;
    }
    $submissions[$row->sid]->data[$row->cid]['value'][$row->no] = $row->data;
    $previous = $row->sid;
  }

  foreach (module_implements('webform_submission_load') as $module) {
    $function = $module . '_webform_submission_load';
    $function($submissions);
  }

  return $submissions;
}

/**
 * Return a count of the total number of submissions for a node.
 *
 * @param $nid
 *   The node ID for which submissions are being fetched.
 * @param $uid
 *   Optional; the user ID to filter the submissions by.
 * @return
 *   An integer value of the number of submissions.
 */
function webform_get_submission_count($nid, $uid = NULL, $reset = FALSE) {
  static $counts;

  if (!isset($counts[$nid][$uid]) || $reset) {
    $query = 'SELECT count(*) FROM {webform_submissions} WHERE nid = %d';
    $arguments = array($nid);
    if ($uid !== NULL) {
      $query .= ' AND uid = %d';
      $arguments[] = $uid;
    }
    if ($uid === 0) {
      $submissions = isset($_SESSION['webform_submission']) ? $_SESSION['webform_submission'] : array();
      $placeholders = count($submissions) ? array_fill(0, count($submissions), "%d") : array();
      $query .= count($submissions) ? ' AND sid IN (' . implode(',', $placeholders) . ')' : ' AND sid = 0';
      $arguments = array_merge($arguments, array_keys($submissions));
    }

    $counts[$nid][$uid] = db_result(db_query($query, $arguments));
  }
  return $counts[$nid][$uid];
}

/**
 * Fetch a specified submission for a webform node.
 */
function webform_get_submission($nid, $sid, $reset = FALSE) {
  static $submissions = array();

  if ($reset) {
    $submissions = array();
    if (!isset($sid)) {
      return;
    }
  }

  // Load the submission if needed.
  if (!isset($submissions[$sid])) {
    $new_submissions = webform_get_submissions(array('sid' => $sid));
    $submissions[$sid] = $new_submissions[$sid];
  }

  // Ensure that the requested NID matches the submission NID.
  if ($submissions[$sid]->nid != $nid) {
    return FALSE;
  }

  return $submissions[$sid];
}

function _webform_submission_spam_check($to, $subject, $from, $headers = array()) {
  $headers = implode('\n', (array)$headers);
  // Check if they are attempting to spam using a bcc or content type hack.
  if (preg_match('/(b?cc\s?:)|(content\-type:)/i', $to . "\n" . $subject . "\n" . $from . "\n" . $headers)) {
    return TRUE; // Possible spam attempt.
  }
  return FALSE; // Not spam.
}

/**
 * Check if the current user has exceeded the limit on this form.
 *
 * @param $node
 *   The webform node to be checked.
 * @return
 *   Boolean TRUE if the user has exceeded their limit. FALSE otherwise.
 */
function _webform_submission_limit_check($node) {
  global $user;

  // Check if submission limiting is enabled.
  if ($node->webform['submit_limit'] == '-1') {
    return FALSE; // No check enabled.
  }

  // Retrieve submission data for this IP address or username from the database.
  $query = 'SELECT count(*) ' .
           'FROM {webform_submissions} ' .
           "WHERE (( 0 = %d AND remote_addr = '%s') OR (uid > 0 AND uid = %d)) " .
           'AND submitted > %d AND nid = %d AND is_draft = 0';

  // Fetch all the entries from the database within the submit interval with this username and IP.
  $num_submissions_database = db_result(db_query($query, $user->uid, ip_address(), $user->uid, ($node->webform['submit_interval'] != -1) ? (time() - $node->webform['submit_interval']) : $node->webform['submit_interval'], $node->nid));

  // Double check the submission history from the users machine using cookies.
  $num_submissions_cookie = 0;
  if ($user->uid == 0 && variable_get('webform_use_cookies', 0)) {
    $cookie_name = 'webform-' . $node->nid;

    if (isset($_COOKIE[$cookie_name]) && is_array($_COOKIE[$cookie_name])) {
      foreach ($_COOKIE[$cookie_name] as $key => $timestamp) {
        if ($node->webform['submit_interval'] != -1 && $timestamp <= time() - $node->webform['submit_interval']) {
          // Remove the cookie if past the required time interval.
          setcookie($cookie_name . '[' . $key . ']', '', 0);
        }
      }
      // Count the number of submissions recorded in cookies.
      $num_submissions_cookie = count($_COOKIE[$cookie_name]);
    }
    else {
      $num_submissions_cookie = 0;
    }
  }

  if ($num_submissions_database >= $node->webform['submit_limit'] || $num_submissions_cookie >= $node->webform['submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}

/**
 * Preprocess function for webform-submission-navigation.tpl.php
 */
function template_preprocess_webform_submission_navigation(&$vars) {
  $start_path = ($vars['mode'] == 'print') ? 'print/' : 'node/';
  $vars['previous'] = db_result(db_query('SELECT MAX(sid) FROM {webform_submissions} WHERE nid = %d AND sid < %d', array($vars['node']->nid, $vars['submission']->sid)));
  $vars['next'] = db_result(db_query('SELECT MIN(sid) FROM {webform_submissions} WHERE nid = %d AND sid > %d', array($vars['node']->nid, $vars['submission']->sid)));
  $vars['previous_url'] = $start_path . $vars['node']->nid . '/submission/' . $vars['previous'] . ($vars['mode'] == 'form' ? '/edit' : '');
  $vars['next_url'] = $start_path . $vars['node']->nid . '/submission/' . $vars['next'] . ($vars['mode'] == 'form' ? '/edit' : '');
}

/**
 * Preprocess function for webform-submission-navigation.tpl.php
 */
function template_preprocess_webform_submission_information(&$vars) {
  $vars['account'] = user_load($vars['submission']->uid);
}
