<?php

// Library for managing subscriptions, pages and forms
require_once drupal_get_path('module', 'notifications') . '/notifications.admin.inc';

/**
 * Event configuration administration
 */
function notifications_scheduler_admin_form($form, &$form_state, $aid = NULL) {
  $action_list = notifications_scheduler_admin_action_list();
  if ($aid && isset($action_list[$aid])) {
    return notifications_scheduler_admin_send_form($form, $form_state, $action_list[$aid]);
  }
  // Compile template list by type
  $template_list = array();
  foreach (notifications_info('message templates') as $key => $template) {
    $template_list[$key] = $template['title'];
  }
  $header = array(
    'action' => t('Action'),
    'template' => t('Template'),
    'triggers' => t('Triggers'),
    'operations' => t('Operations'),
  );
  $form['actions'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#theme' => 'notifications_admin_table_form',
    '#header' => $header,
    '#empty' => t('There are no scheduled notifications available.'),
  );
  foreach (notifications_scheduler_admin_action_list() as $key => $action) {
    $form['actions'][$key]['action']['#markup'] = $action['label'];
    $form['actions'][$key]['template']['#markup'] = isset($template_list[$action['template']]) ? $template_list[$action['template']] : t('None');
    $form['actions'][$key]['triggers']['#markup'] = implode(', ', $action['triggers']);
    $operations['send'] = array(
      '#type' => 'link',
      '#title' => t('Preview'),
      '#href' => 'admin/config/messaging/notifications/scheduler/' . $key,
    );
    $form['actions'][$key]['operations'] = $operations;
  }
  return $form;
}

/**
 * Preview form to send notifications
 */
function notifications_scheduler_admin_send_form($form, &$form_state, $action) {
  drupal_set_title(t('Preview: @action', array('@action' => $action['label'])));
  $form['notification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scheduled notification'),
  );
  $form['notification']['action'] = array(
    '#type' => 'item',
    '#title' => t('Action'),
    '#markup' => $action['label'],
  );
  $event = notifications_scheduler_admin_action_build($action);
  $form['event'] = array('#type' => 'value', '#value' => $event);
  $sids = $event->get_subscriptions();
  $form['sids'] = array('#type' => 'value', '#value' => $sids);

  $form['subscriptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscriptions (@count)', array('@count' => count($sids))),
    '#collapsible' => count($sids) > 2, '#collapsed' => count($sids) > 10,
  );
  $form['subscriptions']['list']['#markup'] = theme('notifications_admin_subscription_list', array('sids' => $sids));
  // Render some template parts
  $template = $event->get_template();
  // Add current user so we can see some token replacement in action
  global $user;
  if (($method = messaging_method_default($user)) && ($destination = messaging_send_method($method)->user_destination($user))) {
    $template->set_destination($destination);
  }
  $template->set_format(MESSAGING_FORMAT_HTML);
  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message preview (HTML)'),
    '#collapsible' => TRUE,
  );
  $form['message']['subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'item',
    '#markup' => $template->render('subject'),
  );

  // @todo Is this text safe for display without later filtering?
  $form['message']['body'] = array(
    '#type' => 'item',
    '#title' => t('Body'),
    '#markup' => $template->render('body'),
  );
  $template->set_format(MESSAGING_FORMAT_PLAIN);
  $form['plaintext'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message preview (Plaintext)'),
    '#collapsible' => TRUE, '#collapsed' => TRUE,
  );
  $form['plaintext']['subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'item',
    '#markup' => '<pre>' . check_plain($template->render('subject')) . '</pre>',
  );

  // @todo Is this text safe for display without later filtering?
  $form['plaintext']['body'] = array(
    '#type' => 'item',
    '#title' => t('Body'),
    '#markup' => '<pre>' . check_plain($template->render('body')) . '</pre>',
  );
  if (count($sids)) {
    $form['send'] = array(
      '#type' => 'fieldset',
      '#title' => t('Send'),
    );
    $form['send']['button'] = array('#type' => 'submit', '#value' => t('Send now'));
  }
  // We need explicit submit callback as this form can be wrapped by others
  $form['#submit'][] = 'notifications_scheduler_admin_send_form_submit';
  return $form;
}

/**
 * Actually send notifications
 */
function notifications_scheduler_admin_send_form_submit($form, &$form_state) {
  $event = $form_state['values']['event'];
  $sids = $form_state['values']['sids'];
  $batch = _notifications_scheduler_create_batch($event, $sids);
  batch_set($batch);
}

/**
 * Compile scheduled actions by action key
 */
function notifications_scheduler_admin_action_list() {
  $list = array();
  foreach (notifications_event_type() as $typekey => $event_info) {
    if (!empty($event_info['scheduler'])) {
      foreach ($event_info['actions'] as $callback) {
        if ($actions = db_select('actions', 'a')->fields('a')->condition('callback', $callback)->execute()->fetchAll()) {
          foreach ($actions as $action) {
            $list[$action->aid] = (array)$action + array(
              'triggers' => array(),
              'template' => $event_info['template'],
              'event_key' => $typekey,
            );
          }
        }
      }
    }
  }
  if ($list) {
    // Compile trigger info indexed by hook
    $trigger_info = array();
    foreach (module_invoke_all('trigger_info') as $trigger_type => $type_info) {
      foreach ($type_info as $hook => $hook_info) {
        $trigger_info[$hook] = $hook_info['label'];
      }
    }
    $query = db_select('trigger_assignments', 't')->fields('t')->condition('aid', array_keys($list))->execute();
    foreach ($query->fetchAll() as $trigger) {
      $list[$trigger->aid]['triggers'][$trigger->hook] = $trigger_info[$trigger->hook];
    }
  }
  return $list;
}

/**
 * Exedute action from admin form. Instead of triggering the event we attempt a batch execution;
 */
function notifications_scheduler_admin_action_build($action) {
  $object = NULL; // It should be a job scheduler object
  $context = !empty($action['parameters']) ? unserialize($action['parameters']) : array();
  return notifications_event($action['event_key'])
    ->set_action($object, $context)
    ->prepare();
}

/**
 * Send to a list of subscriptions
 * @todo Create batch
 */
function _notifications_scheduler_create_batch($event, $sids, $step = 100) {
  // Split the destinations into chunks of smaller size
  $chunks = array_chunk($sids, $step);
  $operations = array();
  foreach ($chunks as $list) {
    $operations[] = array('_notifications_scheduler_batch_send', array($event, $list));
  }
  $batch = array(
    'operations'    => $operations,
    'title'         => t('Sending notifications'),
    'init_message'  => t('Starting sending'),
    'error_message' => t('Error sending notifications'),
    'file'          => drupal_get_path('module', 'notifications_scheduler') . '/notifications_scheduler.admin.inc',
    'finished'      => '_notifications_scheduler_batch_send_finished',
  );
  return $batch;
}

/**
 * Batch send event to list of sids
 */
function _notifications_scheduler_batch_send($event, $sids, &$context) {
  // Only the first time we set the event so the same object is updated (and keeps track of sent notifications).
  if (!isset($context['results']['event'])) {
    $context['results'] = array('event' => $event, 'processed' => array(), 'sent' => array(), 'skip' => array());
  }
  else {
    $event = $context['results']['event'];
  }
  $context['message'] = t('Sending event @event to @count subscriptions.', array('@event' => $event->get_title(), '@count' => count($sids)));
  $results = $event->send_list($sids);
  $results += array('processed' => $sids);
  foreach (array('processed', 'sent', 'skip') as $key) {
    $context['results'][$key] = array_merge($context['results'][$key], $results[$key]);
  }
}

/**
 * Batch send finished
 */
function _notifications_scheduler_batch_send_finished($success, $results) {
  $event = $results['event'];
  $event->done();
  $total = count($results['processed']);
  $sent = count($results['sent']);
  $skip = count($results['skip']);
  $success = count(array_filter($results['sent']));

  drupal_set_message(t('Sent notifications to @count subscriptions: @success success, @errors errors, @skip skipped.', array('@count' => $total, '@success' => $success, '@errors' => $sent - $success, '@skip' => $skip)));
}
