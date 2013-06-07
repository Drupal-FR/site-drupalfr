<?php
/**
 * @file
 *   User pages for User Interface for subscriptions modules
 */

/**
 *  Site-wide settings form.
 */
function notifications_ui_settings_form() {
  // Display enabled subscription types
  $subscription_types = array();
  foreach (notifications_subscription_type_enabled() as $type) {
    $subscription_types[] = array(
      array('data' => notifications_subscription_type($type, 'title'), 'header' => TRUE),
      notifications_subscription_type($type, 'description')
    );
  }
  $form['notifications_ui_types'] = array(
    '#type' => 'item',
    '#title' => t('Enabled subscription types'),
    '#markup' => theme('table', array('rows' => $subscription_types)),
    '#description' => t('Only options for enabled subscription types will be displayed. To enable or disable more subscription types go to <a href="@subscription-options">Subscription Options</a>.', array('@subscription-options' => url('admin/config/messaging/subscriptions'))),
  );

  // Widget options for each subscription type
  $form['notifications_ui_options'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Subscription widgets'),
  /*
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'node') . '/content_types.js'),
    ),
    */
  );
  // Collect all subscription options, some of them will be 'display options'
  $subscription_options = notifications_info('display options');
  foreach (notifications_subscription_type() as $type => $info) {
    // We need to collect options that are display options for each type
    $widgets = array('subscriptions_block');
    if (!empty($info['display_options'])) {
      $widgets = array_unique(array_merge($widgets, $info['display_options']));
    }
    $form[$type] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($info['title']),
      '#description' => check_plain($info['description']),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'notifications_ui_options',
      '#tree' => TRUE,
    );
    $options = array();
    foreach ($widgets as $index) {
      if (isset($subscription_options[$index])) {
         $form[$type][$index] = $subscription_options[$index] + array(
          '#type' => 'checkbox',
          '#default_value' => !empty($info[$index]),
        );
      }
    }
  }
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit display options
 */
function notifications_ui_settings_form_submit(&$form, &$form_state) {
  $options = array();
  foreach (notifications_subscription_type() as $type => $info) {
    foreach ($form_state['values'][$type] as $display => $value) {
     $options[$display][$type] = $value;
    }
  }
  foreach ($options as $display => $values) {
    notifications_option_array_set('subscription_types', $display, $values);
  }
  // This can affect menus so
  menu_rebuild();
}


/**
 * Display the list of available subscription types for creation
 *
 * @ingroup themeable
 */
function theme_notifications_ui_add_list($content) {
  $output = '';

  if ($content) {
    $output = '<dl class="subscriptions-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  return $output;
}
