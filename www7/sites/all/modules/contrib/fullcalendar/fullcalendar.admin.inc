<?php
// $Id: fullcalendar.admin.inc,v 1.3.2.1 2011/02/11 05:35:16 timplunkett Exp $

/**
 * @file
 * Administrative page callbacks for the fullcalendar module.
 */

/**
 * General configuration form for controlling the fullcalendar behaviour.
 */
function fullcalendar_admin_settings() {
  $fullcalendar_path = variable_get('fullcalendar_path', FULLCALENDAR_PATH);

  $form['fullcalendar_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to FullCalendar'),
    '#default_value' => $fullcalendar_path,
    '#description' => t('Enter the path relative to Drupal root where the FullCalendar plugin directory is located.'),
  );
  $form['fullcalendar_compression_type'] = array(
    '#type' => 'radios',
    '#title' => t('Choose FullCalendar compression level'),
    '#options' => array(
      'min' => t('Production (Minified)'),
      'none' => t('Development (Uncompressed Code)'),
    ),
    '#default_value' => variable_get('fullcalendar_compression_type', 'min'),
  );

  return system_settings_form($form);
}

function fullcalendar_admin_settings_validate($form, &$form_state) {
  form_set_value($form['fullcalendar_path'], rtrim($form['fullcalendar_path']['#value'], "/"), $form_state);
}
