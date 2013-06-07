<?php

// Library for managing subscriptions, pages and forms
require_once drupal_get_path('module', 'notifications') . '/notifications.manage.inc';

/**
 * Menu callback: subscriptions administration.
 */
function notifications_admin_manage_subscriptions($form, &$form_state) {
  module_load_include('manage.inc', 'notifications');
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
   if ($selected = array_filter($form_state['values']['subscriptions'])) { // $form_state['values']['subscriptions']
      $list = Notifications_Subscription_List::build_sids(array_keys($selected));
      return notifications_subscription_list_form($form, $form_state, 'delete', $list);
    }
  }
  $form['filter'] = notifications_manage_subscriptions_filter_form(TRUE);
  $form['#submit'][] = 'notifications_manage_subscriptions_filter_form_submit';
  $form['#theme'] = 'notifications_manage_subscriptions_filter_form';
  $form['admin']  = notifications_manage_subscriptions_form();

  return $form;
}

/**
 * Current subscriptions page
 */
function notifications_admin_status_page() {
  $output = '';

  // Subscriptions summary by type
  $header = array(t('Type'), t('Number'));
  $result = db_query("SELECT type, count(*) AS count FROM {notifications_subscription} GROUP BY type");
  $count = 0;
  $types = notifications_subscription_type();
  $rows = array();
  while ($stype = db_fetch_object($result)) {
    $rows[] = array(
      !empty($types[$stype->type]['title']) ? $types[$stype->type]['title'] : '<strong>' . $stype->type . '</strong>' ,
      $stype->count);
    $count += $stype->count;
  }
  $summary = theme('table', array('header' => $header, 'rows' => $rows));
  $summary .= t('Total: %number', array('%number' => $count));
  $output .= theme('box', t('Subscriptions by type'), $summary);

  // Summary by sending method
  $header = array(t('Method'), t('Number'));
  $result = db_query("SELECT send_method, count(*) AS count FROM {notifications_subscription} GROUP BY send_method");
  $rows = array();
  while ($subs = db_fetch_object($result)) {
    $name = messaging_method_info($subs->send_method, 'title');
    $rows[] = array(
      $name ? $name : '<strong>' . $subs->send_method . '</strong>' ,
      $subs->count);
  }
  $summary = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('box', t('Subscriptions by send method'), $summary);

  return $output;
}

/**
 * Admin settings
 */
function notifications_settings_form() {
  $form['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General settings'),
      '#weight' => -10,
  );
  $form['general']['notifications_event_dispatch'] = array(
    '#title' => t('Notifications events'),
    '#type' => 'radios',
    '#default_value' => variable_get('notifications_event_dispatch', 1),
    '#options' => array(t('Disabled'), t('Enabled')),
    '#description' => t('Enable notifications sending. If disabled no new notifications will be produced (though the ones queued will still be sent).'),
  );

  $form['general']['notifications_event_queue'] = array(
    '#title' => t('Queue notifications'),
    '#type' => 'radios',
    '#default_value' => variable_get('notifications_event_queue', 0),
    '#options' => array(t('Disabled'), t('Enabled')),
    '#description' => t('Queue notifications to be sent on cron process later. Recommended for sites with a large number of users.'),
  );
  $form['general']['notifications_sendself'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Notify poster of own posts'),
    '#default_value' => variable_get('notifications_sendself', 1),
    '#description'   => t("Notifies a node poster about their own posts.  Useful principally during testing."),
  );
  $form['general']['notifications_sender'] = array(
    '#title' => t('Notifications Sender'),
    '#type' => 'radios',
    '#options' => array(
      t('No one (All notifications will appear as coming from the web site)'),
      t('User name, site data (Only the user name will be used)'),
      t('Full user data (User name and available user information)'),
    ),
    '#default_value' => variable_get('notifications_sender', 0),
    '#description' => t('Use the site information as the sender for notification messages or use account data from the user causing the event. WARNING: Selecting the last option (Full user data) may disclose private information to subscribers like the user e-mail address.'),
  );

  // Default options
  $form['defaults'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default settings'),
  );
  $form['defaults']['notifications_default_send_interval'] = array(
    '#type' => 'select',
    '#title' => t('Default send interval'),
    '#options' => notifications_send_intervals(),
    '#default_value' => variable_get('notifications_default_send_interval', 0),
  );

  return system_settings_form($form);
}


/**
 * Subscription settings
 */
function notifications_admin_subscriptions_settings() {
  // Enable / disable subscription types
  $type_list = array_map('notifications_format_title_description', notifications_subscription_type());
  $form['subscriptions'] = array(
    '#title' => t('Enabled Subscription types'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );
  $form['subscriptions']['enabled'] = array(
    '#type' => 'checkboxes',
    '#options' => $type_list,
    '#default_value' => notifications_subscription_type_enabled(),
    '#return_value' => TRUE,
    '#description' => t('Check the available subscription types that will be enabled globally'),
  );
  // Link options. These were UI settings that will become global now, so other modules can use them without Notifications UI
  $form['links'] = array(
    '#title' => t('Subscribe / unsubscribe links options'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );
  $options = array(
    t('<strong>Confirmation form</strong>. Links take the user through a confirmation form with some more options.'),
    t('<strong>Direct operation</strong>. Links create/delete a subscription without confirmation using default settings.'),
  );
  $form['links']['notifications_option_subscribe_links'] = array(
    '#title' => t('Subscribe links'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('notifications_option_subscribe_links', 0),
  );
  $form['links']['notifications_option_unsubscribe_links'] = array(
    '#title' => t('Unsubscribe links'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('notifications_option_unsubscribe_links', 0),
  );

  $form = system_settings_form($form);
  // We have our own processing to do after system settings
  $form['#submit'][] = 'notifications_admin_subscriptions_settings_submit';

  return $form;
}

/**
 * Subscription settings submit, disable all subscriptions not allowed
 *
 * @todo d7update
 */
function notifications_admin_subscriptions_settings_submit($form, &$form_state) {
  $types_enabled = $form_state['values']['enabled'];
  // Convert type name values to 1 for storage (instead of type name)
  $enabled = array_filter($types_enabled);
  $types_enabled = array_merge($types_enabled, array_combine($enabled, array_fill(0, count($enabled), 1)));
  notifications_option_array_set('subscription_types', 'enabled', $types_enabled);
  unset($form_state['values']['enabled']);
  // Submission may have been Save or Reset to defaults, so we cannot really trust submitted values
  // Instead we do a cache refresh and re-check enabled / disabled types
  drupal_static_reset('notifications_subscription_type_enabled');
  $types = notifications_subscription_type_enabled();
  $sql_update = 'UPDATE {notifications_subscription} SET status = %d WHERE status = %d';
  $enabled = $disabled = 0;
/*
  if ($types) {
    // Enable subscriptions of these types that were disabled before
    $placeholders = db_placeholders($types, 'varchar');
    $sql_where = ' AND type IN (' . $placeholders . ')';
    $params = array_merge(array(Notifications_Subscription::STATUS_ACTIVE, Notifications_Subscription::STATUS_DISABLED), $types);
    db_query($sql_update . $sql_where, $params);
    $enabled = db_affected_rows();

    $params = array_merge(array(Notifications_Subscription::STATUS_DISABLED, Notifications_Subscription::STATUS_ACTIVE), $types);
    $sql_where = ' AND type NOT IN (' . $placeholders . ')';
  }
  else {
    $params = array(Notifications_Subscription::STATUS_DISABLED, Notifications_Subscription::STATUS_ACTIVE);
    $sql_where = '';
  }
  db_query($sql_update . $sql_where, $params);
  $disabled = db_affected_rows();

  if ($enabled) {
    drupal_set_message(t('@count existing subscriptions have been enabled.', array('@count' => $enabled)), 'warning');
  }
  if ($disabled) {
    drupal_set_message(t('@count existing subscriptions have been disabled.', array('@count' => $disabled)), 'warning');
    // Delete rows from queue and do some clean up.
    notifications_queue()->queue_clean(FALSE);
  }
*/
}

/**
 * Event configuration administration
 */
function notifications_admin_events_form($form, &$form_state) {
  // Compile template list by type
  $template_list = array();
  foreach (notifications_info('message templates') as $key => $template) {
    $template_list[$template['object']][$key] = $template['title'];
  }

  $header = array(
    'name' => t('Event type'),
    'template' => t('Template'),
    'triggers' => t('Triggers'),
  );
  $form['events'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#theme' => 'notifications_admin_table_form',
    '#header' => $header,
    '#empty' => t('You need to enable some plug-ins to provide notifications events.'),
  );
  foreach (notifications_event_type() as $key => $event) {
    $form['events'][$key]['name']['#markup'] = $event['title'];
    $form['events'][$key]['template'] = array(
      '#type' => 'select',
      '#options' => isset($template_list[$event['object']]) ? $template_list[$event['object']] : array(),
      '#default_value' => $event['template'],
      '#parents' => array('events', $key),
    );
    $form['events'][$key]['triggers']['#markup'] = implode(', ', notifications_admin_event_triggers($key));
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return $form;
}

/**
 * Event configuration administration
 */
function notifications_admin_events_form_submit($form, &$form_state) {
  $events = $form_state['values']['events'];
  notifications_option_array_set('event_types', 'template', $events);
  drupal_set_message(t('Your configuration has been updated.'));
}

/**
 * Get assigned triggers for event
 *
 * @todo Better coding, get from db?
 */
function notifications_admin_event_triggers($type) {
  static $action_info, $trigger_info;
  if (!isset($action_info)) {
    $action_info = module_invoke_all('action_info');
    $trigger_info = module_invoke_all('trigger_info');
  }
  $result = array();
  if ($event_triggers = notifications_event_type($type, 'triggers', array())) {
    $event_actions = notifications_event_type($type, 'actions', array());
    foreach ($event_triggers as $trigger_type => $hooks) {
      if (in_array('any', $hooks) && isset($trigger_info[$trigger_type])) {
        $hooks = array_keys($trigger_info[$trigger_type]);
      }
      foreach ($hooks as $hook) {
        foreach (trigger_get_assigned_actions($hook) as $trigger_action) {
          $aid = $trigger_action['aid'];
          if (is_numeric($aid)) {
            $aid = db_query('SELECT callback FROM {actions} WHERE aid = :aid', array(':aid' => $aid))->fetchField();
          }
          if ($aid && in_array($aid, $event_actions)) {
            $result[$hook] = $trigger_info[$trigger_type][$hook]['label'];
          }
        }
      }

    }
  }
  return $result;
}
/**
 * Build event tree
 */
function notifications_admin_events_build_tree($events) {
  $tree = array();
  foreach ($events as $key => &$event) {
    if (empty($event['parent'])) {
      $tree[$key] = &$event;
    }
    else {
      $events[$event['parent']]['children'][$key] = &$event;
    }
  }
  return $tree;
}

/**
 * Build tree of event types
 */
function notifications_admin_events_tree_form($tree, &$form, $depth = 0) {
  foreach ($tree as $key => $event) {
    $form['name'][$key] = array('#value' => theme('indentation', $depth) . $event['description']);
    $form['notifications_event_enabled'][$key] = array(
      '#type' => 'checkbox',
      '#default_value' => notifications_event_enabled($key),
    );
    $form['notifications_event_template'][$key] = array(
      '#type' => 'select',
      '#default_value' => Notifications_Event::template_map($key),
      '#options' => notifications_template_list(),
    );
    if (!empty($event['children'])) {
      notifications_admin_events_tree_form($event['children'], $form, $depth +1);
    }
  }
}

/**
 * Plain list of template names
 */
function notifications_template_list() {
  $list = &drupal_static(__FUNCTION__);
  if (!isset($list)) {
    notifications_include('templates.inc');
    foreach (notifications_get_templates() as $key => $template) {
      $list[$key] = $template['name'];
    }
  }
  return $list;
}

/**
 * Theme administration form/fieldset as table
 */
function theme_notifications_admin_table_form($variables) {
  $form = $variables['form'];
  $default = language_default();
  $header = $form['#header'];
  // Build table rows using header indexes for the fields
  $rows = array();
  foreach (element_children($form) as $index) {
    $row = array();
    foreach (array_keys($header) as $field) {
      $row[] = drupal_render($form[$index][$field]);
    }
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'empty' => $form['#empty']));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Display big list of subscriptions
 */
function theme_notifications_admin_subscription_list($variables) {
  $sids = $variables['sids'];
  $limit = $variables['limit'];
  // Display fully only the first ones up to limit
  if (!$sids) {
    return t('No subscriptions');
  }
  else {
    $display = array_slice($sids, 0, $limit);
    $send_methods = _notifications_send_methods();
    foreach ($display as $sid) {
      $subscription = notifications_subscription_load($sid);
      $rows[] = array(
        l($sid, 'notifications/subscription/' . $sid),
        $subscription->get_name(),
        $send_methods[$subscription->send_method],
        theme('username', array('account' => $subscription->get_user())),
      );
    }
    if (count($sids) > count($display)) {
      $rows[] = array(array(
        'data' => t('And @count other subscriptions...', array('@count' => count($sids) - count($display))),
        'colspan' => 4
      ));
    }
    return theme('table', array('rows' => $rows));
  }
}
