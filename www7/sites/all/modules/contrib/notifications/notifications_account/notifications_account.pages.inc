<?php
/**
 * @file
 * User pages for Notifications
 */

/**
 * Menu callback. Overview page for user subscriptions.
 *
 * We create the page as a form so it can be altered
 *
 */
function notifications_account_overview($account) {
  $form['account'] = array('#type' => 'value', '#value' => $account);

  $path = 'user/' . $account->uid;

  // Status, send method, interval
  $count = db_query("SELECT status, COUNT(*) AS num FROM {notifications_subscription} WHERE uid = :uid GROUP BY status", array(':uid' => $account->uid));
  $status_list = Notifications_Subscription::status_list();
  while ($current = $count->fetchObject()) {
    $status[$current->status] = t('You have %count @status subscriptions.', array('%count' => $current->num, '@status' => $status_list[$current->status]));
  }
  if (empty($status)) {
    $status['none'] = t('You don\'t have any subscriptions yet.');
  }
  $send_intervals = notifications_send_intervals($account);
  $send_methods = notifications_send_methods($account);
  if ($method = notifications_user_setting('send_method', $account)) {
    if (isset($send_methods[$method])) {
      $status['method'] = t('Your default sending method for new subscriptions is @send_method', array('@send_method' => $send_methods[$method]));
    }
  }
  $interval = notifications_user_setting('send_interval', $account);
  if (isset($send_intervals[$interval])) {
    $status['interval'] = t('Your default sending interval for new subscriptions is @send_interval', array('@send_interval' => $send_intervals[$interval]));
  }

  $form['status'] = array('#type' => 'item', '#weight' => 10, '#title' => t('Current status'), '#markup' => theme('item_list', array('items' => $status)));

  // Build shortcut tips
  if (notifications_access_user($account, 'manage')) {
    $tips['admin'] = l(t('Administer your subscriptions'), "$path/notifications/subscription");
  }
  $tips['edit'] = l(t('Edit your notifications settings'), "$path/edit");

  // Enable / disable all subscriptions
  if (notifications_access_user($account, 'maintain')) {
    if (!empty($status[Notifications_Subscription::STATUS_ACTIVE])) {
      $tips['disable'] = l(t('Temporarily disable all your subscriptions'), "$path/notifications/update/disable");
    }
    if (!empty($status[Notifications_Subscription::STATUS_INACTIVE]) || !empty($status[Notifications_Subscription::STATUS_BLOCKED])) {
      $tips['enable'] = l(t('Enable all your subscriptions'), "$path/notifications/update/enable");
    }
  }
  /*
  $link = notifications_get_link('unsubscribe', array('uid' => $account->uid, 'destination' => TRUE));
  $tips['cancel'] = l(t('Cancel all your subscriptions'), $link['href'], $link['options']);
  */
  //$output .= theme('item_list', $tips, t('You can'));
  $form['tips'] = array('#type' => 'item', '#weight' => 20, '#title' => t('You can'), '#markup' => theme('item_list', array('items' => $tips)));

  return $form;
}

/**
 * Menu callback: user subscriptions management
 */
function notifications_account_manage_subscriptions_form($form, &$form_state, $account) {
  module_load_include('manage.inc', 'notifications');
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    $list = Notifications_Subscription_List::build_sids(array_filter($form_state['values']['subscriptions']));
    return notifications_subscription_list_form($form, $form_state, 'delete', $list);
  }
  $form = notifications_manage_subscriptions_filter_form();
  $form['filters']['#title'] = t('Filter');
  // Display collapsed only when no filters set
  $form['filters'] += array('#collapsible' => TRUE, '#collapsed' => empty($_SESSION['subscriptions_overview_filter']));
  $form['#submit'][] = 'notifications_manage_subscriptions_filter_form_submit';
  $form['#theme'] = 'notifications_manage_subscriptions_filter_form';
  $form['admin']  = notifications_manage_subscriptions_form(array('uid' => $account->uid));

  return $form;
}

/**
 * List form for subscriptions of the same type for a user
 *
 * @todo Add enable /disable operations
 * @todo Add pager
 *
 * @param $type
 *   Subscription type
 * @param $account
 *   User account, owner of these subscriptions
 * @param $subscriptions
 *   Predefined array of subscriptions to display, so we don't search more nor allow new ones.
 * @param $fields
 *   Predefined field values for new subscriptions, as type => value array
 */
function notifications_account_subscription_list_form($form, &$form_state, $type, $account) {
  if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Delete')) {
    if ($selected = array_filter($form_state['values']['subscriptions']['select'])) {
      $list = Notifications_Subscription_List::build_sids(array_keys($selected));
      return notifications_subscription_list_form($form, $form_state, 'delete', $list);
    }
  }
  $account = messaging_user_object($account);
  $substype = notifications_subscription($type);
  $form['account'] = array('#type' => 'value', '#value' => $account);
  $form['subscription_type'] = array('#type' => 'value', '#value' => $type);

  // Build a template subscriptions. These subscriptions should have a unique subscription variable field
  $form['info'] = array(
    '#type' => 'item',
    '#title' => t('@type subscriptions', array('@type' => $substype->get_title())),
    '#description' => check_plain($substype->get_description()),
  );

  $subscriptions = new Notifications_Subscription_Table();
  $subscriptions
    ->set_header()
    ->set_conditions(array('uid' => $account->uid, 'type' => $type))
    ->query_load();
  $form['subscriptions'] = $subscriptions->table_select();

  if (count($subscriptions)) {
    $form['operations'] = array(
      '#type' => 'fieldset',
    );

  }
  return $form;
}

function notifications_account_subscription_list_operations() {
  $form['operations']['update'] = array('#type' => 'submit', '#value' => t('Update'));
  $form['operations']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  //$form['operations']['enable'] = array('#type' => 'submit', '#value' => t('Enable'));
  //$form['operations']['disable'] = array('#type' => 'submit', '#value' => t('Disable'));
}

/**
 * Submit list of subscriptions
 */
function notifications_account_subscription_list_form_validate($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : NULL;
  switch ($op) {
    case t('Add'):
      $save_fields = array();
      foreach ($form_state['values']['editable_fields'] as $field) {
        $type = $field->field;
        $key = 'field_' . $type;
        $value = isset($form_state['values']['subscriptions'][$key]['new']) ? $form_state['values']['subscriptions'][$key]['new'] : NULL;
        if ($real_value = notifications_field_real_value($type, $value)) {
          $save_fields[] = array('type' => $type, 'value' => $real_value);
        }
        else {
          form_set_error("subscriptions][$key][new", t('You must set a valid value for this field.'));
        }
      }
      $form_state['values']['parsed_fields'] = $save_fields;
      break;
    case t('Delete'):
    case t('Enable'):
    case t('Disable'):
      // For this cases check only that we have selected some subscriptions
      $selected = array_filter($form_state['values']['subscriptions']['select']);
      if (isset($selected['new'])) {
        unset($selected['new']);
        unset($form_state['values']['subscriptions']['select']['new']);
      }
      if (!$selected) { //count($selected) < 1) {
        form_set_error('', t('You must select some subscriptions for that operation.'));
      }
      break;
  }
}

/**
 * Submit list of subscriptions
 */
function notifications_account_subscription_list_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : NULL;
  $updated = 0;
  switch ($op) {
    case t('Add'):
      $subscription = Notifications_Subscription::build_object($form_state['values']['template']);
      $subscription->add_fields($form_state['values']['parsed_fields']);
      notifications_account_subscription_list_form_values($form_state['values']['subscriptions'], $subscription, 'new');
      notifications_submit_subscription($subscription);
      break;
    case t('Update');
      $updated = 0;
      foreach ($form_state['values']['current_subscriptions'] as $key => $subscription) {
        if (notifications_account_subscription_list_form_values($form_state['values']['subscriptions'], $subscription)) {
          $subscription->save();
          $updated++;
        }
      }
      break;
    case t('Delete');
      $form_state['rebuild'] = TRUE;
      break;

  }
  if ($updated) {
    drupal_set_message(format_plural($updated, 'One subscription has been updated.', '@count subscriptions have been updated.'));
  }
}

/**
 * Extract values from subscription list form
 */
function notifications_account_subscription_list_form_values($form_values, $subscription, $key = NULL) {
  $key = isset($key) ? $key : $subscription->sid;
  $values = array();
  foreach (array('send_method', 'send_interval', 'status') as $field) {
    if (isset($form_values[$field][$key]) && (!isset($subscription->$field) || $subscription->$field != $form_values[$field][$key])) {
      // We just set the values that have changed and return them if any
      $subscription->$field = $values[$field] = $form_values[$field][$key];
    }
  }
  return $values;
}

/**
 * Bulk update all user subscriptions, confirmation form
 *
 * @param $account
 *   User account
 * @param $operation
 *   Operation to perform: enable, disable
 */
function notifications_account_update_subscriptions($form_state, $account, $operation) {
  $form['account'] = array('#type' => 'value', '#value' => $account);
  $form['operation'] = array('#type' => 'value', '#value' => $operation);
  $destination = 'user/' . $account->uid . '/notifications';
  $form['#redirect'] = $destination;
  if ($operation == 'enable') {
    return confirm_form($form,
                      t('Are you sure you want to enable all your subscriptions?'),
                      $destination, t('You will get notifications again for all of them.'),
                      t('Enable'), t('Cancel'));
  }
  elseif ($operation == 'disable') {
    return confirm_form($form,
                      t('Are you sure you want to disable all your subscriptions?'),
                      $destination, t('You will stop getting notifications until you enable them again.'),
                      t('Disable'), t('Cancel'));
  }
  else {
    drupal_access_denied();
  }
}

/**
 * Mass update user subscriptions
 */
function notifications_account_update_subscriptions_submit($form, $form_state) {
  $account = $form_state['values']['account'];

  switch ($form_state['values']['operation']) {
    case 'enable':
      // Enable also blocked subscriptions just in case there are some
      db_query('UPDATE {notifications_subscription} SET status = %d WHERE uid = %d', Notifications_Subscription::STATUS_ACTIVE, $account->uid);
      drupal_set_message(t('All your subscriptions have been enabled.'));
      break;
    case 'disable':
      db_query('UPDATE {notifications_subscription} SET status = %d WHERE status = %d AND uid = %d', Notifications_Subscription::STATUS_INACTIVE, Notifications_Subscription::STATUS_ACTIVE, $account->uid);
      notifications_queue()->queue_clean(array('uid' => $account->uid));
      drupal_set_message(t('All your subscriptions have been disabled.'));
      break;
  }
}

/**
 * Form for unsubscription confirmation
 *
 * It works for both single subscription or account (all subscriptions)
 */
function notifications_account_form_unsubscribe_confirm($form_state, $subscription, $account = NULL) {
  // Pass on subscription values
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);
  $form['account'] = array('#type' => 'value', '#value' => $account);
  if ($subscription) {
    $message = t('Are you sure you want to delete this subscription?');
    // The subscription description will be added here
    $form['info'] = $subscription->form_info();
  }
  else {
    $message = t('Are you sure you want to remove all your subscriptions on this site?');
  }

  return confirm_form($form,
    $message,
    isset($_GET['destination']) ? $_GET['destination'] : '',
    t('This action cannot be undone.'),
    t('Unsubscribe'),
    t('Cancel')
  );
}

/**
 * Process unsubscribe form submission
 */
function notifications_account_form_unsubscribe_confirm_submit($form, &$form_state) {
  $subscription = $form_state['values']['subscription'];
  $account = $form_state['values']['account'];
  switch ($form_state['values']['op']) {
    case t('Unsubscribe'):
      if ($subscription) {
        Notifications_Subscription::delete_subscription($subscription->sid);
        drupal_set_message(t('Your subscription has been removed.'));
      }
      elseif ($account) {
        Notifications_Subscription::delete_multiple(array('uid' => $account->uid));
        drupal_set_message(t('All your subscriptions have been removed.'));
      }
      $form_state['redirect'] = '';
      break;
    case t('Cancel'):
      // Do nothing, not worth showing a message, just get back
      $form_state['redirect'] = '';
      break;
  }
}

