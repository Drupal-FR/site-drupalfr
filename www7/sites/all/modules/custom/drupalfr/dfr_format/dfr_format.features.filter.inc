<?php
/**
 * @file
 * dfr_format.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function dfr_format_filter_default_formats() {
  $formats = array();

  // Exported format: Format Wiki
  $formats['7'] = array(
    'format' => '7',
    'name' => 'Format Wiki',
    'cache' => '1',
    'status' => '1',
    'weight' => '0',
    'filters' => array(
      'codefilter' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(),
      ),
      'filter_html' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(
          'allowed_html' => '<h1> <h2> <h3> <h4> <a> <em> <strong> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd> <strike>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 1,
        ),
      ),
      'filter_markdown' => array(
        'weight' => '6',
        'status' => '1',
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => '10',
        'status' => '1',
        'settings' => array(
          'filter_url_length' => '72',
        ),
      ),
    ),
  );

  return $formats;
}
