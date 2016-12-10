<?php
/**
 * @file
 * Admin settings for Menu Trail By Path
 */

/**
 * Form builder; create and display the admin configuration settings form.
 */
function menu_trail_by_path_form($form, &$form_state) {
  // Menu trail
  $form['menu_trail_by_path_menu_trail'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu-trail'),
  );
  $form['menu_trail_by_path_menu_trail']['menu_trail_by_path_menu_trail_active_on_admin_paths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active on admin paths'),
    '#description' => t("If checked, menu-trail handling will also be active on the administrative section of the site."),
    '#default_value' => MenutrailbypathConfig::get('menu_trail_active_on_admin_paths'),
  );
  $form['menu_trail_by_path_menu_trail']['menu_trail_by_path_menu_trail_active_on_ajax_requests'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active on ajax requests'),
    '#description' => t("If checked, menu-trail handling will also be active on ajax requests."),
    '#default_value' => MenutrailbypathConfig::get('menu_trail_active_on_ajax_requests'),
  );

  // Breadcrumb
  $form['menu_trail_by_path_breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
  );
  $form['menu_trail_by_path_breadcrumb']['menu_trail_by_path_breadcrumb_handling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable breadcrumb handling'),
    '#description' => t("If checked, breadcrumb will be set according to url path."),
    '#default_value' => MenutrailbypathConfig::get('breadcrumb_handling'),
  );
  $form['menu_trail_by_path_breadcrumb']['menu_trail_by_path_breadcrumb_active_on_admin_paths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active on admin paths'),
    '#description' => t("If checked, breadcrumb handling will also be active on the administrative section of the site."),
    '#default_value' => MenutrailbypathConfig::get('breadcrumb_active_on_admin_paths'),
  );
  $form['menu_trail_by_path_breadcrumb']['menu_trail_by_path_breadcrumb_active_on_ajax_requests'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active on ajax requests'),
    '#description' => t("If checked, breadcrumb handling will also be active on ajax requests."),
    '#default_value' => MenutrailbypathConfig::get('breadcrumb_active_on_ajax_requests'),
  );
  $form['menu_trail_by_path_breadcrumb']['menu_trail_by_path_breadcrumb_display_current_page'] = array(
    '#title' => t('Display current page in the breadcrumb trail'),
    '#type' => 'radios',
    '#options' => array(
      'no' => t('No, don\'t show the current page in the breadcrumb trail.'),
      'yes_span' => t('Yes, show the current page as plain text.'),
      'yes_link' => t('Yes, show the current page as a active link.')
    ),
    '#default_value' => MenutrailbypathConfig::get('breadcrumb_display_current_page'),
  );

  return system_settings_form($form);
}
