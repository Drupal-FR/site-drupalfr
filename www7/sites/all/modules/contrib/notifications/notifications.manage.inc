<?php

/**
 * @file
 *   Common functions for bulk management of subscriptions
 */

/**
 * Check access for current user to manage multiple subscriptions
 *
 * @param $sids
 *   Array of subscription ids
 */
function notifications_manage_subscriptions_access($sids) {
  if (user_access('administer notifications') || user_access('manage all subscriptions')) {
    return TRUE;
  }
  else {
    $count = db_query("SELECT COUNT(*) FROM {notifications_subscription} WHERE uid = :uid AND sid IN (:sid)" , array(
      ':uid' => $GLOBALS['user']->uid,
      ':sid' => $sids
    ))->fetchField();
    return $count == count($sids);
  }
}

/**
 * Administer subscriptions. For user tabs and admin tabs
 *
 * @param $conditions
 *   Filter conditions. I.e. for user administering its own subscriptions, we'll have an 'uid' condition
 */
function notifications_manage_subscriptions_form($conditions = array()) {
  $admin_access = user_access('administer notifications') || user_access('manage all subscriptions') ||   user_access('maintain own subscriptions');
  // It is a user list if we have an uid parameter
  $account = !empty($conditions['uid']) ? user_load($conditions['uid']) : NULL;
  $status_list = Notifications_Subscription::status_list();
  $type_list = notifications_subscription_type(NULL, 'title');
  $send_methods = messaging_method_info(NULL, 'name');
  $send_intervals = notifications_send_intervals();
  $send_intervals[-1] = t('Scheduled');
  $drupal_destination = drupal_get_destination();
  // This is the full options subsform
  $form = notifications_manage_subscriptions_form_options($admin_access);
  // Depending on the context (user, admin) we'll have a different base path for the operation links
  $base_path = $account ? 'user/' . $account->uid . '/notifications/subscription' : 'notifications/subscription';

  // Build the sortable table header.
  $header = array(
    'sid' => array('data' => t('Id'), 'field' => 's.sid'),
    'type' => array('data' => t('Type'), 'field' => 's.type'),
    'status' => array('data' => t('Status'), 'field' => 's.status'),
    'name' => t('Description'),
    'created' => array('data' => t('Created'), 'field' => 's.created', 'sort' => 'desc')
  );
  if (!$account) {
    $header['uid'] = array('data' => t('User'), 'field' => 's.uid');
  }
  // If we have more than one send method or interval, add the columns
  if (count($send_methods) > 1) {
    $header['send_method'] = array('data' => t('Send method'), 'field' => 's.send_method');
  }
  if (count($send_intervals) > 1) {
    $header['send_interval'] = array('data' => t('Send interval'), 'field' => 's.send_interval');
  }
  // Add operations at the end
  $header['operations'] = array('data' => t('Operations'));

  // Query data with $conditions and filters
  $query = db_select('notifications_subscription', 's')->extend('PagerDefault')->extend('TableSort');
  foreach ($conditions as $field => $value) {
    $query->condition('s.' . $field, $value);
  }
  notifications_manage_subscriptions_build_filter_query($query, $conditions);
  $sids = $query
    ->fields('s', array('sid'))
    ->limit(50)
    ->orderByHeader($header)
    ->execute()
    ->fetchCol();

  $subscriptions = Notifications_Subscription_Table::build_sids($sids)
    ->set_header($header);
  // Only use a tableselect when the current user is able to perform any operations.
  if ($admin_access) {
    $form['subscriptions'] = $subscriptions->table_select();
  }
  // Otherwise, use a simple table.
  else {
    $form['subscriptions'] = $subscriptions->table_list();
  }
  $form['pager'] = array('#markup' => theme('pager', array('tags' => NULL)));
  return $form;
}

/**
 * Form options
 */
function notifications_manage_subscriptions_form_options($admin_access = FALSE) {
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    // if the current user is able to perform any operations hide the form element.
    '#access' => $admin_access,
  );
  $options = array();
  foreach (notifications_manage_subscriptions_operations($admin_access) as $operation => $array) {
    if (!empty($array['parent'])) {
      $options[$array['parent']][$operation] = $array['label'];
    } else {
      $options[$operation] = $array['label'];
    }
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('notifications_manage_subscriptions_form_validate'),
    '#submit' => array('notifications_manage_subscriptions_form_submit'),
  );
  return $form;
}

/**
 * Validate notifications_admin_subscriptions form submissions.
 *
 * Check if any items have been selected to perform the chosen
 * 'Update option' on.
 */
function notifications_manage_subscriptions_form_validate($form, &$form_state) {
  $items = array_filter($form_state['values']['subscriptions']);
  if (count($items) == 0) {
    form_set_error('', t('No items selected.'));
  }
  elseif (!notifications_manage_subscriptions_access(array_keys($items))) {
    form_set_error('', t('You don\'t have permissions to manage these subscriptions'));
  }
}

/**
 * Handle manage form submissions, run batch operations
 */
function notifications_manage_subscriptions_form_submit($form, &$form_state) {
  $operations = notifications_manage_subscriptions_operations();
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked subscriptions
  $items = array_filter($form_state['values']['subscriptions']);

  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($items), $operation['callback arguments']);
    }
    else {
      $args = array($items);
    }
    call_user_func_array($function, $args);
  }
  else {
    // We need to rebuild the form to go to a second step.  For example, to
    // show the confirmation form for the deletion of subscriptions.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Build query for node administration filters based on session.
 */
function notifications_manage_subscriptions_build_filter_query($query, $conditions) {
  $filters = notifications_manage_subscriptions_filters();
  // Build query
  if (!empty($_SESSION['subscriptions_overview_filter'])) {
    foreach ($_SESSION['subscriptions_overview_filter'] as $index => $filter) {
      list($key, $value) = $filter;
      switch ($key) {
        case 'type':
        case 'status':
        case 'send_interval':
        case 'send_method':
          $query->condition('s.' . $key, $value);
          break;
      }
    }
  }
}

/**
 * List node administration filters that can be applied.
 *
 * @param $admin
 *   Whether this is for the site admin page, will display more options
 */
function notifications_manage_subscriptions_filters($admin = FALSE) {
  global $user;

  $filters['status'] = array(
    'title' => t('Status'),
    'options' => Notifications_Subscription::status_list(),
  );
  $filters['type'] = array(
    'title' => t('Type'),
    // If not admin page, check access to each type
    'options' => notifications_subscription_type(NULL, 'title'),
  );
  $filters['send_method'] = array(
    'title' => t('Method'),
    // If not admin mode, filter for current user
    'options' => _notifications_send_methods($admin ? NULL : $user),
  );
  $filters['send_interval'] = array(
    'title' => t('Interval'),
    'options' => notifications_send_intervals(),
  );
  // Take out the filters when only one option
  foreach ($filters as $key => $data) {
    if (empty($data['options']) || count($data['options']) == 1) {
      unset($filters[$key]);
    }
    else {
      $filters[$key]['options'] =  array('[any]' => t('any')) + $data['options'];
    }
  }
  return $filters;
}

/**
 * Subscription mass operations.
 *
 * @param $account
 *   User account if we are administering subscriptions for this user
 */
function notifications_manage_subscriptions_operations($account = NULL) {
  $operations = array(
    'activate' => array(
      'label' => t('Activate'),
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => Notifications_Subscription::STATUS_ACTIVE)),
    ),
    'deactivate' => array(
      'label' => t('Deactivate'),
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => Notifications_Subscription::STATUS_INACTIVE)),
    ),
    'delete' => array(
      'label' => t('Delete'),
      'callback' => NULL,
    ),
  );
  // Block option only for administrators
  if (user_access('administer notifications') || user_access('manage all subscriptions')) {
    $operations['block'] = array(
      'label' => t('Block'),
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => Notifications_Subscription::STATUS_BLOCKED)),
    );
  }
  // Sending methods, not for destination
  if (!empty($account->mdid)) {
    $parent = t('Change send method to');
    foreach (_notifications_send_methods($account) as $key => $name) {
      $operations['send_method-' . $key] = array(
        'label' => $name,
        'parent' => $parent,
        'callback' => 'notifications_manage_subscriptions_mass_update',
        'callback arguments' => array('updates' => array('send_method' => $key)),
      );
    }
  }
  $parent = t('Change send interval to');
  foreach (notifications_send_intervals() as $key => $name) {
    $operations['send_interval-' . $key] = array(
      'label' => $name,
      'parent' => $parent,
      'callback' => 'notifications_manage_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('send_interval' => $key)),
    );
  }

  // Intervals
  return $operations;
}

/**
 * Make mass update of subscriptions, changing all nodes in the $nodes array
 * to update them with the field values in $updates.
 *
 * IMPORTANT NOTE: This function is intended to work when called
 * from a form submit handler. Calling it outside of the form submission
 * process may not work correctly.
 *
 * @param array $subscriptions
 *   Array of subscriptions nid to update.
 * @param array $updates
 *   Array of key/value pairs with node field names and the
 *   value to update that field to.
 */
function notifications_manage_subscriptions_mass_update($subscriptions, $updates) {
  foreach ($subscriptions as $id) {
    _notifications_manage_subscriptions_mass_update_helper($id, $updates);
  }
  drupal_set_message(t('The update has been performed.'));
}

function _notifications_manage_subscriptions_mass_update_helper($sid, $updates) {
  $subs = notifications_subscription_load($sid);
  foreach ($updates as $name => $value) {
    $subs->$name = $value;
  }
  notifications_save_subscription($subs);
  return $subs;
}

/**
 * Return form for node administration filters.
 *
 * @param $admin
 *   Whether this is for the site admin page, will display more options
 */
function notifications_manage_subscriptions_filter_form($admin = FALSE) {
  $session = isset($_SESSION['subscriptions_overview_filter']) ? $_SESSION['subscriptions_overview_filter'] : array();
  $filters = notifications_manage_subscriptions_filters($admin);
  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    $value = $filters[$type]['options'][$value];
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('type', 'language', 'status'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i > 0 && count($filters) > 0) ? '<div class="additional-filters">' . t('and where') . '</div>' : '',
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#name' => 'filter',
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'), '#name' => 'undo');
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'), '#name' => 'reset');
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Process result from node administration filter form.
 */
function notifications_manage_subscriptions_filter_form_submit($form, &$form_state) {
  $filters = notifications_manage_subscriptions_filters();
  $triggering_element = isset($form_state['triggering_element']['#name']) ? $form_state['triggering_element']['#name'] : '';
  switch ($triggering_element) {
    case 'filter':
    case 'refine':
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['subscriptions_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case 'undo':
      array_pop($_SESSION['subscriptions_overview_filter']);
      break;
    case 'reset':
      $_SESSION['subscriptions_overview_filter'] = array();
      break;
  }
}
