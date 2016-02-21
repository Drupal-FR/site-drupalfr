<?php

/**
 * @file
 * Rules integration of the mimemail and the PDF Views module.
 *
 */

/**
 * Implements hook_rules_action_info().
 */
function views_pdf_rules_action_info() {

  $items = array();

  $items['views_pdf_rules_action_save'] = array(
    'label' => t('Save PDF as file on server'),
    'group' => t('Views PDF'),
    'parameter' => array(
      'views_pdf' => array(
        'type' => 'text',
        'label' => t('View'),
        'options list' => 'views_pdf_rules_view_list',
        'description' => t('You need to enter the View and the Display to use. Use the ids of them and separate them by a ":".'),
      ),
      'arguments' => array(
        'type' => 'text',
        'label' => t('Views Arguments'),
        'optional' => TRUE,
        'description' => t('Place on each line one argument.')
      ),
      'path' => array(
        'type' => 'text',
        'label' => t('Store Path'),
        'optional' => FALSE,
        'description' => t('Enter an relative path where the view should be saved. You may use some tokens.'),
      ),
    ),
  );

  return $items;
}

function views_pdf_rules_view_list() {
  $views = views_get_all_views();

  $list = array();

  foreach ($views as $view => $view_object) {
    foreach ($view_object->display as $display => $display_object) {
      if ($display_object->display_plugin == 'pdf') {
        $list[$view . ':' . $display] = $view_object->human_name . ': ' . $display_object->display_title;
      }
    }
  }

  return $list;
}


function views_pdf_rules_action_save($views_pdf, $arguments, $path) {
  $splits = explode(':', $views_pdf);
  $view_id = $splits[0];
  $display_id = $splits[1];

  if (!empty($view_id)) {
    $view = views_get_view($view_id);

    $view->set_arguments(preg_split("/\r\n|\n|\r/", $arguments));

    // Try to get pdf display
    if (!$view->set_display($display_id)) {
      // Try the display type
      if (!$view->set_display('pdf_1')) {
        // There is definitly no pdf display
        return;
      }
    }

    $view->pre_execute();
    foreach ($view->display as $id => $display) {
      if ($display->display_plugin == 'pdf' && isset($display->handler)) {
        $display->handler->execute($path, 'F');
      }
    }

  }
}
