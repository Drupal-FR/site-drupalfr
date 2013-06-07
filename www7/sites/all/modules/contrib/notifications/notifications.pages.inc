<?php
/**
 * @file
 * User pages for Notifications
 */

/**
 * Menu callback add subscription
 *
 * Presents confirmation page or not depending on confirm parameter
 */
function notifications_page_subscribe($substype) {
  $account = $GLOBALS['user'];
  // Build subscriptions object
  if ($substype->user_access($account, 'subscribe')) {
    $subscription = $substype
      ->instance()
      ->set_user($account)
      ->set_properties_from_url();
    // If everything is ok and signed to skip confirmation, go ahead
    if ($subscription->check_destination() && $subscription->check_fields() && notifications_check_signature('skip')) {
      $subscription->save();
      drupal_set_message(t('Your subscription was activated.'));
      drupal_goto();
    }
    else {
      // Ask for confirmation
      drupal_set_title(t('Confirm your subscription'));
      return drupal_get_form('notifications_subscription_subscribe_form', $subscription);
    }
  }
  drupal_set_message(t('Subscription or parameters not allowed.'), 'error');
  drupal_goto();
  return "HOW WE REACHED HERE?";
}

/**
 * Menu callback for unsubscribe page
 *
 * @param $type
 *   Either 'sid' or 'uid' (for unsubscribe all)
 * @param $id
 *   Subscription id or user id, depending on type
 */
function notifications_page_unsubscribe_subscription($subscription) {

  // If everything is ok and signed to skip confirmation, go ahead
  if (notifications_check_signature('skip')) {
    $subscription->delete();
    drupal_set_message(t('Your subscription has been removed.'));
    drupal_goto();
  }
  else {
    // Ask for confirmation
    drupal_set_title(t('Delete your subscription'));
    return drupal_get_form('notifications_subscription_unsubscribe_form', $subscription);
  }
  drupal_access_denied();
}

/**
 * Menu callback for unsubscribe page
 *
 * @param $type
 *   Either 'sid' or 'uid' (for unsubscribe all)
 * @param $id
 *   Subscription id or user id, depending on typeunt
 */
function notifications_page_unsubscribe_user($account) {
  // If everything is ok and signed to skip confirmation, go ahead
  if (notifications_check_signature('skip')) {
    $count = Notifications_Subscription::delete_multiple(array('uid' => $account->uid));
    drupal_set_message(t('All your subscriptions have been removed.'));
    drupal_goto();
  }
  else {
    $subscriptions = new Notifications_Subscription_List();
    $subscriptions->load_multiple(array(), array('uid' => $account->uid));
    if ($subscriptions->count()) {
      // Ask for confirmation
      return drupal_get_form('notifications_subscription_list_form', 'unsubscribe', $subscriptions);
    }
    else {
      // No subscriptions. Print warning and fall back to access denied.
      drupal_set_message(t('You don\'t have any subscriptions on this site.'), 'error');
    }
  }
  drupal_access_denied();
}
/**
 * Display unsubscribe options for this page
 */
function notifications_page_unsubscribe_overview() {
  $account = $GLOBALS['user'];
  notifications_include('destination.inc');

  $output = '';
  $send_methods = notifications_destination_anonymous_methods();
  $subscriptions = 0;
  // Provide information for user subscriptions if logged in
  if ($user->uid) {
    $subscriptions = db_result(db_query("SELECT COUNT(*) FROM {notifications_subscription} WHERE uid = %d", $user->uid));
    if ($subscriptions) {
      $output .= '<p>' . format_plural($subscriptions, 'There is one subscription for this user account.', 'There are @count subscriptions for this user account.') . '</p>';
      $output .= '<p>' . t('You can:') . '<br />';
      $options = array();
      if (notifications_access_user($account, 'maintain')) {
        $options[] = l(t('Administer your subscriptions'), "user/$user->uid/notifications");
        $options[] = l(t('Temporarily disable all your subscriptions'), "user/$user->uid/notifications/update/disable");
      }
      $options[] = notifications_build_link('unsubscribe', array('title' => t('Cancel all your subscriptions')), 'user', $user, 'link');
      $output .= theme('item_list', $options);
    }
    else {
      $output .= '<p>' . t('There are no subscriptions for this user account') . '</p>';
      if ($send_methods) {
        $output .= '<p>' . t('However if you are still getting notifications from this site, enter your address below and you\'ll get a link to disable that address.') . '</p>';
      }
    }
  }

  if (!$subscriptions && $send_methods) {
    // Present destination unsubscribe request form
    $output .= drupal_get_form('notifications_destination_request_form', $account, $send_methods);
  }
  elseif (!$user->uid) {
    // WTF? Someone is getting spam from here?
    $output .= '<p>' . t('You may have an account on this site or someone may have created it for you.') . '</p>';
    $output .= '<p>' . t('You can try to <a href="@user-login">log in</a>, request a <a href="@request-password">new password</a>, or contact the site administrator.', array('@user-login' => url('user/login'), '@request-password' => url('user/password'))) . '</p>';
  }
  return $output;
}

/**
 * Manage destination form. Edit subscriptions for a destination
 */
function notifications_edit_destination_form($form_state, $destination) {
  notifications_include('destination.inc');
  $account = $destination->get_user();
  $form = notifications_destination_view_subform($destination);

  return $form;
}

/**
 * Check access to current page, included signature
 *
 * @param $owner
 *   User id, owner of the object in the page (subscription, destination, account)
 *
 */
function notifications_page_check_access($owner) {
  global $user;

  if (user_access('administer notifications')) {
    return TRUE; // Administrator can access all
  }
  elseif ($owner) {
    // Current user is the owner or the URL is signed
    return $user->uid == $owner || notifications_page_check_signature();
  }
  elseif (module_exists('notifications_anonymous')) {
    // Owner is anonymous but we just allow it if the module is enabled the URL is signed
    return notifications_page_check_signature();
  }
}

/**
 * Check current URL is signed
 */
function notifications_page_check_signature() {
  static $signed;
  if (!isset($signed)) {
    $signed = !empty($_GET['signature']) && notifications_check_signature();
  }
  return $signed;
}

/**
 * Check URL is signed and it doesn't need confirmation
 */
function notifications_page_check_confirmation() {
  return !empty($_GET['confirm']) && notifications_page_check_signature();
}

/**
 * Base subscription form. Present the main fields and check all values
 */
function notifications_subscription_base_form($form_state, $subscription, $destination_path = NULL) {
  // We may have a destination query string instead of a destination path
  if (!empty($_REQUEST['destination'])) {
    $destination_path = $_REQUEST['destination'];
  }
  if (isset($destination_path)) {
    $form['#redirect'] = $destination_path;
  }

  // Check the subscription object is built properly
  $subscription = notifications_subscription_build($subscription);
  // Pass on whole subscription and then the fields to be saved along
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);
  $account = $subscription->get_user();
  $save_fields = array();

  // The subscription description will be added here, fields may be editable or not
  if (!$subscription->is_editable()) {
    // Just display general information, no editable fields
    $form['info'] = $subscription->form_info();
  }
  else {
    $form['fields'] = $subscription->fields_subform();
    $save_fields[] = 'fields';
  }
  if ($subscription->is_instance()) {
    $form['info']['#title'] = t('Edit subscription');
  }
  else {
    $form['info']['#title'] = t('Create @type subscription', array('@type' => $subscription->get_title()));
  }

  // Send interval, visible only if more than one
  if ($send_intervals = notifications_send_intervals($account)) {
    $save_fields[] = 'send_interval';
    if (count($send_intervals) == 1) {
      $form['send_interval'] = array('#type' => 'value', '#value' => key($send_intervals));
    }
    else {
      $form['notifications']['send_interval'] = array(
        '#type' => 'select',
        '#title' => t('Send interval'),
        '#options' => $send_intervals,
        '#default_value' => $subscription->send_interval,
      );
    }
  }
  else {
    drupal_set_message(t('No sending intervals available.'), 'error');
    $form['#error']['send_interval'] = TRUE;
  }
  // Send method, visible only if more than one
  if ($send_methods = notifications_send_methods($account)) {
    $save_fields[] = 'send_method';
    if (count($send_methods) == 1) {
      $form['send_method'] = array('#type' => 'value', '#value' => key($send_methods));
    }
    else {
      $form['notifications']['send_method'] = array(
        '#type' => 'select',
        '#title' => t('Send method'),
        '#options' => $send_methods,
        '#default_value' => $subscription->send_method,
        '#disabled' => count($send_methods) < 2,
      );
    }
  }
  else {
    drupal_set_message(t('No sending methods available.'), 'error');
    $form['#error']['send_method'] = TRUE;
  }
  // Add notifications fieldset if send method or send_interval
  if (!empty($form['notifications'])) {
    $form['notifications']['#title'] = t('Notifications');
    $form['notifications']['#type'] = 'fieldset';
    $form['notifications']['#description'] = t('How do you want to get notifications for this subscription.');
  }
  // Status field, show blocked only to administrators
  if ($subscription->is_instance()) {
    $save_fields[] = 'status';
    $status = Notifications_Subscription::status_list();
    if (!user_access('administer notifications') && !user_access('manage all subscriptions')) {
      unset($status[Notifications_Subscription::STATUS_BLOCKED]);
    }
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#options' => $status,
      '#default_value' => $subscription->status,
      '#description' => t('You can temporarily disable this subscription for not getting notifications.')
    );
  }
  // Fields to be saved on submit. If error we just show the 'Cancel' button.
  $form['subscription_fields'] = array('#type' => 'value', '#value' => $save_fields);

  // Add form buttons depending on what we are doing
  if ($subscription->is_instance()) {
    $form['actions']['save'] = array('#type' => 'submit', '#value' => t('Save'));
    $form['actions']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  }
  elseif ($subscription->is_editable()) {
    $form['actions']['subscribe'] = array('#type' => 'submit', '#value' => t('Create subscription'));
  }
  else {
    // Subscription parameters are fixed so this is just a confirmation form
    $form['actions']['subscribe'] = array('#type' => 'submit', '#value' => t('Subscribe'));
  }
  if (isset($destination_path)) {
    $form['actions']['cancel'] = array('#value' => l(t('Cancel'), $destination_path));
  }
  // Add the right callbacks
  $form['#submit'][] = 'notifications_subscription_form_submit';
  $form['#validate'][] = 'notifications_subscription_form_validate';
  return $form;
}

/**
 * Validate subscription
 */
function notifications_subscription_base_form_validate($form, &$form_state) {
  Notifications_Subscription::validate_submission($form_state);
}

/**
 * Save edited subscription
 */
function notifications_subscription_base_form_submit($form, &$form_state) {
  $subscription = Notifications_Subscription::build_submission($form_state);
  switch ($form_state['values']['op']) {
    case t('Save'):
    case t('Subscribe'):
    case t('Create subscription'):
      // We are updating or creating a new subscription
      notifications_submit_subscription($subscription);
      break;
    case t('Delete'):
      Notifications_Subscription::delete_subscription($subscription->sid);
      drupal_set_message(t('Your subscription has been deleted.'));
      $form_state['redirect'] = 'user/' . $subscription->uid . '/notifications';
      break;
  }
}

/**
 * Submit subscription printing out the results
 */
function notifications_submit_subscription($subscription) {
  global $user;

  $instance = $subscription->is_instance();
  // If a new subscription we may need to set current user
  if (!isset($subscription->uid)) {
    $subscription->set_account($user);
  }
  // Check some parameters are ok (checking fills in some missing values too)
  if (!$subscription->check_all()) {
    drupal_set_message($subscription->error_message, 'error');
    return FALSE;
  }

  // Check permissions for current user to create this subscription
  if (!$subscription->check_access($user)) {
    if (!empty($subscription->error_message)) {
      drupal_set_message($subscription->error_message);
    }
    else {
      drupal_set_message(t("You are not allowed to create this type of subscription."), 'error');
    }
    return FALSE;
  }

  // Actually update or create subscription, skipping checks that we've already done.
  $result = notifications_save_subscription($subscription, FALSE);

  switch ($result) {
    case SAVED_NEW:
      drupal_set_message(t('Your subscription has been created.'));
      return TRUE;
    case SAVED_UPDATED:
      if ($instance) {
        drupal_set_message(t('Your subscription has been updated.'));
      }
      else {
        drupal_set_message(t('Your subscription has been created updating an existing one of the same type.'));
      }
      return TRUE;
    default:
      if ($instance) {
        drupal_set_message(t('Your subscription cannot be updated.'), 'error');
      }
      else {
        drupal_set_message(t('Your subscription cannot be created.'), 'error');
      }
      return FALSE;
  }
}
/**
 * Form for multiple delete. When account passed check that all subscriptions belong to the user account
 */
function notifications_multiple_delete_confirm(&$form_state, $items, $destination = NULL) {
  $destination = $destination ? $destination : $_GET['q'];
  $sids = array_keys($items);
  if (notifications_manage_subscriptions_access(array_keys($items))) {
    $form['items'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
    // array_filter returns only elements with TRUE values
    foreach ($items as $id => $value) {
      // Load the subscription to display a friendly name
      $subscription = notifications_subscription_load($id);
      $form['items'][$id] = array(
        '#type' => 'hidden',
        '#value' => $id,
        '#prefix' => '<li>',
        '#suffix' => $subscription->format_long() . "</li>\n",
      );
    }
    $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
    $form['#submit'][] = 'notifications_multiple_delete_confirm_submit';
    $form['#validate'][] = 'notifications_multiple_delete_confirm_validate';
    $form['#redirect'] = $destination;
    return confirm_form($form,
                      t('Are you sure you want to delete these items?'),
                      $destination, t('This action cannot be undone.'),
                      t('Delete all'), t('Cancel'));
  }
  else {
    drupal_set_message(t('Validation error. You don\'t have permission to delete some of these subscriptions'), 'error');
    drupal_access_denied();
  }
}

/**
 * Validate permissions to delete all the subscriptions
 */
function notifications_multiple_delete_confirm_validate($form, &$form_state) {
  if (!notifications_manage_subscriptions_access(array_keys($form_state['values']['items']))) {
    form_set_error('', t('You don\'t have permission to manage all these subscriptions'));
  }
}

/**
 * Submit multiple delete from
 */
function notifications_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    Notifications_Subscription::delete_subscription(array_keys($form_state['values']['items']));
    drupal_set_message(t('The subscriptions have been deleted.'));
  }
  return;
}
/**
 * Theme function for fields in form
 */
function theme_notifications_field_type($elements) {
  $elements = $variables['element'];
  $header = array(t('Field type'), t('Value'));
  $rows = array();
  foreach (element_children($elements['name']) as $key) {
    $rows[] = array(
      drupal_render($elements['name'][$key]),
      drupal_render($elements['value'][$key]),
    );
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render($elements);
  return $output;
}


/**
 * Generic table formatting for forms
 */
function theme_notifications_table_form($variables) {
  $form = $variables['form'];
  $output = '';
  // Get table information from special form properties
  $index = !empty($form['#table_index']) ? $form['#table_index'] : $form['#table_fields'][0];
  $header = !empty($form['#table_header']) ? $form['#table_header'] : array();

  foreach (element_children($form[$index]) as $key) {
    $row = array();
    foreach ($form['#table_fields'] as $field) {
      $row[] = drupal_render($form[$field][$key]);
    }
    $rows[] = $row;
  }

  if ($rows) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('No elements') . '</p>';
  }
  $output .= drupal_render($form);
  return $output;
}
