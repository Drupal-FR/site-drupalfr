<?php
/**
 * @file
 * Views hook implementations
 */

/**
 * Implemenation of hook_views_data()
 */
function views_view_field_views_data() {


  $data['view']['table']['group'] = t('View');
  $data['view']['table']['join'] = array(
    '#global' => array(),
  );

  $data['view']['include'] = array(
    'title' => t('Include View'),
    'help' => t('Includes a view into this view.'),
    'field' => array(
      'handler' => 'views_view_field_handler_include_view',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  return $data;
}
