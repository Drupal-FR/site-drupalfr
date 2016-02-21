<?php

/**
 * @file
 * Implementaion of the views hooks.
 */

/**
 * Implements hook_views_data().
 */
function views_pdf_views_data() {
  $data['pdf']['table']['group'] = t('PDF');
  $data['pdf']['table']['join'] = array(
    '#global' => array(),
  );

  $data['pdf']['page_break'] = array(
    'title' => t('Page Break'),
    'help' => t('Insert in the PDF view a new page.'),
    'field' => array(
      'handler' => 'views_pdf_handler_page_break',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );

  $data['pdf']['page_number'] = array(
    'title' => t('Page Number'),
    'help' => t('Contains the page number of the current page.'),
    'field' => array(
      'handler' => 'views_pdf_handler_page_number',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  );

  return $data;
}


/**
 * Implements hook_views_plugins().
 */
function views_pdf_views_plugins() {
  $path = drupal_get_path('module', 'views_pdf');
  $theme_path = $path . '/theme';
  return array(
    'module' => 'views_pdf',
    'display' => array(
      'pdf' => array(
        'title' => t('PDF Page'),
        'help' => t('Outputs the view as a PDF file.'),
        'handler' => 'views_pdf_plugin_display',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use more' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('PDF Page'),
      ),
    ),
    'style' => array(
      'pdf_table' => array(
        'title' => t('PDF Table'),
        'help' => t('Display the view as a table.'),
        'handler' => 'views_pdf_plugin_style_table',
        'parent' => 'default',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'pdf',
      ),
      'pdf_unformatted' => array(
        'title' => t('PDF unformatted'),
        'help' => t('Display the view in a unformatted way.'),
        'handler' => 'views_pdf_plugin_style_unformatted',
        'parent' => 'default',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'pdf',
      ),
    ),
   'row' => array(
      'pdf_fields' => array(
        'title' => t('PDF Fields'),
        'help' => t('Displays the fields with an optional template.'),
        'handler' => 'views_pdf_plugin_row_fields',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'pdf',
        'help topic' => 'style-row-fields',
      ),
    ),


  );
}
