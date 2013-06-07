<?php
/**
 * @file
 * Subscriptions to content events
 */

/**
 * Admin settings form
 */
function notifications_content_settings_form($form, &$form_state) {
  // Build check boxes table with content types x subscription types
  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled subscription types'),
    '#weight' => -10,
    '#collapsible' => TRUE,
    '#description' => t('Check the subscription types that will be enabled. You can use the global settings here or set different options for each content type.'),
  );
  $form['content']['notifications_content_per_type'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('notifications_content_per_type', 0),
    '#options' => array(
      t('Use global settings on this page for all content types'),
      t('Set up for each content type on <a href="@content-type-settings">Administer Content Types</a>.', array('@content-type-settings' => url('admin/structure/types'))),
    ),
  );
  $form['content']['notifications_content_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Global options'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('notifications_content_type', array()),
    '#description' => t('Content types for which subscriptions will be enabled.'),
  );

  return system_settings_form($form);
}

/**
 * Template testing form
 */
function notifications_content_test_template_form($form_state) {
  global $user, $language;

  module_load_include('admin.inc', 'notifications');
  $message = !empty($form_state['values']['message']) ? $form_state['values']['message'] : NULL;
  if ($message) {
    $form['showmessage'] = array(
      '#title' => t('Message'),
      '#type' => 'item',
      '#value' => _notifications_content_test_format_message(array('subect' => $message->subject, 'body' => $message->body)),
    );
    $form['showtemplate'] = array(
      '#title' => t('Template parts'),
      '#type' => 'item',
      '#value' => _notifications_content_test_format_template($message->text_parts),
    );
  }
  $form['type'] = array(
    '#title' => t('Node type'),
    '#type' => 'select',
    '#options' => node_get_types('names'),
    '#default_value' => isset($form_state['values']['type']) ? $form_state['values']['type'] : NULL,
  );
  $form['action'] = array(
    '#title' => t('Action'),
    '#type' => 'select',
    '#options' => notifications_info('event actions'),
    '#default_value' => isset($form_state['values']['action']) ? $form_state['values']['action'] : NULL,
  );
  $form['subscription_type'] = array(
    '#title' => t('Subscription type'),
    '#type' => 'select',
    '#options' => notifications_subscription_type(NULL, 'title'),
    '#default_value' => isset($form_state['values']['subscription_type']) ? $form_state['values']['subscription_type'] : NULL,
    '#description' => t('Some modules may define a different template for their subscription types.'),
  );
  $form['send_method'] = array(
    '#title' => t('Send method'),
    '#type' => 'select',
    '#options' => notifications_send_methods($user),
  );
  $form['language'] = array(
    '#title' => t('Language'),
    '#type' => 'select',
    '#options' => messaging_template_language_list(),
    '#default_value' => isset($form_state['values']['language']) ? $form_state['values']['language'] : $language->language,
  );
  $form['node'] = array(
    '#title' => t('Select node'),
    '#type' => 'textfield',
    '#default_value' => isset($form_state['values']['node']) ? $form_state['values']['node'] : NULL,
    '#description' => t("Enter a node id. If empty, a dummy node will be generated with title 'Test title' and body 'Test body line' (x 5 lines)"),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Try'),
    '#submit' => array('notifications_content_test_template_submit'),
    '#description' => t('This test will generate fake events so only advised for development sites.'),
  );
  return $form;
}

/**
 * Process template test
 */
function notifications_content_test_template_submit(&$form, &$form_state) {
  global $user;

  // Load node or build fake node
  if ($nid = (int)$form_state['values']['node']) {
    if (($node = node_load($nid)) && node_access('view', $node)) {
      drupal_set_message(t('Loaded node type @type', array('@type' => $node->type)));
    }
  }
  if (empty($node)) {
    $type = $form_state['values']['type'];
    $node = _notifications_content_test_generate_content($type);
    $node->nid = 1;
    $node->uid = $user->uid;
    $node->type = $type;
  }
  // Build fake event for node/action
  $action = $form_state['values']['action'];
  $objects['node'] = $node;
  // Add comment if needed
  if ($action == 'comment') {
    $comment = _notifications_content_test_generate_content('comment');
    $comment->cid = 1;
    $comment->uid = $user->uid;
    $comment->nid = $node->nid;
    $objects['comment'] = $comment;
  }
  $event = Notifications_Event::create(
    array(
     'eid' => 1,
     'module' => 'node',
     'type' => 'node',
     'action' => $action,
    ),
    $objects
  );
  if (!$event->queue) {
    drupal_set_message(t('That event type is not enabled though we try building a message for it.'), 'warning');
  }
  // Get a subscription template, that may determine the module
  $subscription = Notifications_Subscription::build_type($form_state['values']['subscription_type']);

  // Build the message and store it
  $send_method = $form_state['values']['send_method'];
  if ($destination = messaging_account_build_destination($user, $send_method)) {
    $template = new Notifications_Message();
    $template->method = $send_method;
    $template->set_destination($destination);
    $template->language = $form_state['values']['language'];
    $message = Notifications_Message::build_simple_message($template, $event, array(), $subscription->module);
    // Store message and rebuild. The message will come with template information (text_parts);
    $form_state['values']['message'] = $message;
    $form_state['rebuild'] = TRUE;
  }
  else {
    drupal_set_message(t('Cannot build destination.'), 'warning');
  }

}

/**
 * Generate fake node or comment
 */
function _notifications_content_test_generate_content($type) {
  $node = new stdClass();
  $title = t('Test title for @type', array('@type' => $type));
  for ($i = 1; $i <= 5; $i++) {
    $body[] = t('Test body line @number for @type.', array('@number' => $i, '@type' => $type));
  }
  if ($type == 'comment') {
    $node->subject = $title;
    $node->comment = implode("\n", $body);
  }
  else {
    $node->title = $title;
    $node->body = implode("\n", $body);
    $node->teaser = node_teaser($node->body);
  }
  return $node;
}

/**
 * Format resulting message array as table
 */
function _notifications_content_test_format_message($message) {
  $rows = array();
  foreach ($message as $key => $value) {
    $rows[] = array($key, is_array($value) ?  _notifications_content_test_format_message($value) : '<pre>' . check_plain($value) . '</pre>');
  }
  return theme('table', array(), $rows);
}

/**
 * Format template information as table
 */
function _notifications_content_test_format_template($parts) {
  $rows = array();
  $header = array('msgkey' => t('Key'), 'type' => t('Template'), 'method' => t('Method'), 'message' => t('Text'), 'language' => t('Language'), 'format' => t('Format'));
  foreach ($parts as $key => $value) {
    $row = array();
    foreach (array_keys($header) as $field) {
      $row[] = isset($value->$field) ? check_plain($value->$field) : '';
    }
    $rows[] = $row;
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}
/**
 * Theme content type settings
 */
function theme_notifications_content_type_settings(&$elements) {
  $output = '';
  $options = _notifications_content_type_options();
  $header = array_merge(array(''), array_values($options));
  $rows = array();
  foreach (element_children($elements) as $key) {
    $row = array($elements[$key]['#title']);
    unset($elements[$key]['#title']);
    foreach (array_keys($options) as $index) {
      unset($elements[$key][$index]['#title']);
      $row[] = drupal_render($elements[$key][$index]);
    }
    $rows[] = $row;
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render($elements);
  return $output;
}

