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

  // Exported format: Filtered HTML.
  $formats[1] = array(
    'format' => 1,
    'name' => 'Filtered HTML',
    'cache' => 1,
    'status' => 1,
    'weight' => -1,
    'filters' => array(
      'filter_markdown' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(),
      ),
      'pathologic' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(
          'local_paths' => '',
          'protocol_style' => 'path',
          'local_paths_exploded' => array(
            0 => array(
              'path' => '/',
            ),
            1 => array(
              'path' => '/',
              'host' => 'default',
            ),
          ),
          'base_url_host' => 'default',
        ),
      ),
      'filter_html' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd> <p> <br> <img> <h3> <h4> <b> <strike>',
          'filter_html_help' => 0,
          'filter_html_nofollow' => 1,
        ),
      ),
      'codefilter' => array(
        'weight' => -46,
        'status' => 1,
        'settings' => array(
          'nowrap_expand' => FALSE,
          'codefilter_prism' => FALSE,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -44,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_autop' => array(
        'weight' => -43,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -42,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
    ),
  );

  // Exported format: Full HTML.
  $formats[3] = array(
    'format' => 3,
    'name' => 'Full HTML',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_autop' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(),
      ),
      'codefilter' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(
          'nowrap_expand' => FALSE,
          'codefilter_prism' => FALSE,
        ),
      ),
      'pathologic' => array(
        'weight' => -43,
        'status' => 1,
        'settings' => array(
          'local_paths' => '',
          'protocol_style' => 'path',
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -42,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  // Exported format: markdown.
  $formats[6] = array(
    'format' => 6,
    'name' => 'markdown',
    'cache' => 1,
    'status' => 0,
    'weight' => 0,
    'filters' => array(),
  );

  // Exported format: Format Wiki.
  $formats[7] = array(
    'format' => 7,
    'name' => 'Format Wiki',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_html' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<h1> <h2> <h3> <h4> <a> <em> <strong> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd> <strike> <img>',
          'filter_html_help' => 0,
          'filter_html_nofollow' => 1,
        ),
      ),
      'codefilter' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(
          'nowrap_expand' => FALSE,
          'codefilter_prism' => FALSE,
        ),
      ),
      'filter_markdown' => array(
        'weight' => -45,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -43,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
    ),
  );

  // Exported format: Messaging plain text.
  $formats[8] = array(
    'format' => 8,
    'name' => 'Messaging plain text',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(),
  );

  // Exported format: Plain text.
  $formats[9] = array(
    'format' => 9,
    'name' => 'Plain text',
    'cache' => 1,
    'status' => 1,
    'weight' => 1,
    'filters' => array(
      'filter_html_escape' => array(
        'weight' => 0,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => 1,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'filter_autop' => array(
        'weight' => 2,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
