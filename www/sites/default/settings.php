<?php

$databases = [];

$config_directories = [
  CONFIG_SYNC_DIRECTORY => $app_root . '/' . $site_path . '/../../../conf/drupal/default/sync',
];

$settings['omit_vary_cookie'] = TRUE;

$settings['update_free_access'] = FALSE;
$settings['allow_authorize_operations'] = FALSE;

$settings['file_public_path'] = 'sites/default/files';

$settings['file_scan_ignore_directories'] = [
  'node_modules',
  'bower_components',
];

$settings['container_yamls'][] = $app_root . '/' . $site_path . '/services.yml';

// TODO: Remove this line when it will no more be added when installing with
// Drush.
$settings['install_profile'] = 'drupalfr';

// Prevent deletion of orphan files.
// TODO: Remove this line when the following issues will be fixed:
// - https://www.drupal.org/node/2801777
// - https://www.drupal.org/node/2708411
// - https://www.drupal.org/node/1239558
// - https://www.drupal.org/node/2666700
// - https://www.drupal.org/node/2810355
$config['system.file']['temporary_maximum_age'] = 0;

// Performance.
$config['system.performance']['cache']['page']['max_age'] = 86400;
$config['system.performance']['css']['preprocess'] = TRUE;
$config['system.performance']['js']['preprocess'] = TRUE;

if (file_exists($app_root . '/' . $site_path . '/../../../conf/drupal/default/settings.local.php')) {
  include $app_root . '/' . $site_path . '/../../../conf/drupal/default/settings.local.php';
}
