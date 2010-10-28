<?php
// $Id: admin_views.views.inc,v 1.1.2.1 2010/03/11 01:10:55 sun Exp $

/**
 * @file
 * Views plugin handlers.
 */

/**
 * Implements hook_views_plugins().
 */
function admin_views_views_plugins() {
  $path = drupal_get_path('module', 'admin_views');

  $plugins = array(
    'module' => 'views', // This just tells our themes are elsewhere.
    'display' => array(
      'system' => array(
        'title' => t('System'),
        'help' => t('Display the view as replacement for an existing menu path.'),
        'path' => "$path/plugins",
        'handler' => 'views_plugin_display_system',
        'theme' => 'views_view',
        'uses hook menu' => TRUE,
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'accept attachments' => TRUE,
        'admin' => t('System'),
      ),
    ),
  );

  return $plugins;
}

