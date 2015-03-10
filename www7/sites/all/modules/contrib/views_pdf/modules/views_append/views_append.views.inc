<?php
/**
 * @file
 * This file contains all the views hooks for the appending of a view
 * to another view.
 */

/**
 * Implementaion of hook_views_data()
 */
function views_append_views_data() {

  $data['views_append']['table']['group'] = t('View');
  $data['views_append']['table']['join'] = array(
    '#global' => array(),
  );

  $data['views_append']['append'] = array(
    'title' => t('Append View'),
    'help' => t('Appends a view at this view.'),
    'field' => array(
      'handler' => 'views_append_handler_append_view',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );
  return $data;
}
