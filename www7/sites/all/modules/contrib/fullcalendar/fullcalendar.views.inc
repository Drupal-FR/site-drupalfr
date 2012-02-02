<?php
// $Id: fullcalendar.views.inc,v 1.4.2.7 2011/02/09 19:20:23 timplunkett Exp $

/**
 * @file
 * Contains views module hooks
 */

/**
 * Implements hook_views_plugins().
 */
function fullcalendar_views_plugins() {
  $path = drupal_get_path('module', 'fullcalendar');
  $views_path = drupal_get_path('module', 'views');
  return array(
    'module' => 'fullcalendar',
    'display' => array(
      'parent' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_display',
        'path' => "$views_path/plugins",
        'parent' => '',
      ),
      'page' => array(
        'handler' => 'views_plugin_display_page',
        'path' => "$views_path/plugins",
        'parent' => 'parent',
      ),
      'fullcalendar' => array(
        'title' => t('FullCalendar'),
        'handler' => 'fullcalendar_plugin_display_page',
        'help' => t('For use with FullCalendar style'),
        'path' => $path,
        'parent' => 'page',
        'theme' => 'views_view',
        'uses hook menu' => TRUE,
        'use pager' => TRUE,
      ),
    ),
    'style' => array(
      'fullcalendar' => array(
      'title' => t('FullCalendar'),
      'help' => t('Displays items on a calendar.'),
      'handler' => 'views_plugin_style_fullcalendar',
      'theme' => 'views_view_fullcalendar',
      'theme path' => $path,
      'uses row plugin' => TRUE,
      'uses options' => TRUE,
      'uses grouping' => TRUE,
      'type' => 'normal',
      'parent' => 'default',
      ),
    ),
    'row' => array(
      'fullcalendar_node' => array(
        'title' => t('Fields (FullCalendar)'),
        'help' => t('For use with FullCalendar style'),
        'handler' => 'views_plugin_node_fullcalendar',
        'theme' => 'views_view_node_fullcalendar',
        'theme path' => $path,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data()
 */
function fullcalendar_views_data() {

  $data['fullcalendar'] = array(
    'table' => array(
      'group' => t('FullCalendar'),
      'join' => array('#global' => array()),
    ),
    'area' => array(
      'title' => t('Emtpy calendar'),
      'help' => t('Provide empty calendar.'),
      'area' => array(
        'handler' => 'fullcalendar_handler_empty',
      ),
    ),
    'gcal' => array(
      'title' => t('Google Calendar'),
      'help' => t('Display events from a Google Calendar.'),
      'field' => array(
        'handler' => 'fullcalendar_handler_field_gcal',
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 *
 * Find all date fields and assign them our handler.
 */
function fullcalendar_views_data_alter(&$data) {
  foreach ($data as $module => &$table) {
    if (isset($table['entity_id'])) {
      $fields = fullcalendar_date_fields(array($table['entity_id']['field']['field_name']));
      if (!empty($fields)) {
        $table['entity_id']['field']['handler'] = 'fullcalendar_handler_field_field';
      }
    }
    else {
      foreach ($table as $id => &$field) {
        if (isset($field['field']) && isset($field['field']['handler']) && ($field['field']['handler'] == 'views_handler_field_date')) {
          $field['field']['handler'] = 'fullcalendar_handler_field_date';
        }
      }
    }
  }
}
