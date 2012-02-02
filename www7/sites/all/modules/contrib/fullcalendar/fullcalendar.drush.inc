<?php
// $Id: fullcalendar.drush.inc,v 1.3 2011/01/26 21:45:10 timplunkett Exp $

/**
 * @file
 * Drush integration for FullCalendar.
 */

/**
 * The FullCalendar plugin URI.
 */
include_once('fullcalendar.module');
define('FULLCALENDAR_DOWNLOAD_URI', 'http://arshaw.com/fullcalendar/downloads/fullcalendar-' . FULLCALENDAR_MIN_PLUGIN_VERSION . '.zip');

/**
 * Implements hook_drush_command().
 */
function fullcalendar_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['fullcalendar-plugin'] = array(
    'description' => dt("Downloads the FullCalendar plugin."),
    'arguments' => array(
      'path' => dt('Optional. A path where to install the FullCalendar plugin. If omitted Drush will use the default location.'),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function fullcalendar_drush_help($section) {
  switch ($section) {
    case 'drush:fullcalendar-plugin':
      return dt("Downloads the FullCalendar plugin from arshaw.com, default location is sites/all/libraries.");
  }
}

/**
 * Commands to download the FullCalendar plugin.
 */
function drush_fullcalendar_plugin($path = 'sites/all/libraries') {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $filename = basename(FULLCALENDAR_DOWNLOAD_URI);
  $dirname = basename(FULLCALENDAR_DOWNLOAD_URI, '.zip');

  // Remove any existing FullCalendar plugin directory
  if (is_dir($dirname)) {
    drush_log(dt('A existing FullCalendar plugin was overwritten at @path', array('@path' => $path)), 'notice');
  }
  // Remove any existing FullCalendar plugin zip archive
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive
  if (!drush_shell_exec('wget '. FULLCALENDAR_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O '. FULLCALENDAR_DOWNLOAD_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive
    drush_shell_exec('unzip -qq -o '. $filename);
    // Remove the zip archive
    drush_op('unlink', $filename);
    if (is_dir('fullcalendar')) {
      drush_shell_exec('rm -rf fullcalendar');
    }
    drush_shell_exec('mv ' . $dirname . '/fullcalendar fullcalendar');
    drush_shell_exec('rm -rf ' . $dirname);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path .'/fullcalendar')) {
    drush_log(dt('FullCalendar plugin has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the FullCalendar plugin to @path', array('@path' => $path)), 'error');
  }
}
