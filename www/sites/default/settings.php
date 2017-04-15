<?php

$databases = array();

$config_directories = array(
  CONFIG_SYNC_DIRECTORY => __DIR__ . '/../../../conf/drupal',
);

$settings['omit_vary_cookie'] = TRUE;

$settings['update_free_access'] = FALSE;
$settings['allow_authorize_operations'] = FALSE;

$settings['file_public_path'] = 'sites/default/files';

$settings['file_scan_ignore_directories'] = [
  'node_modules',
  'bower_components',
];

$settings['container_yamls'][] = __DIR__ . '/services.yml';

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
$config['system.performance']['cache']['page']['max_age'] = '86400';
$config['system.performance']['css']['preprocess'] = TRUE;
$config['system.performance']['js']['preprocess'] = TRUE;

// Redis.
$settings['redis.connection']['interface'] = 'PhpRedis';

// External cache.
if (file_exists(__DIR__ . '/.cache_activated')) {
  // Additional redis services.
  $settings['container_yamls'][] = 'modules/contrib/redis/example.services.yml';

  $settings['cache']['default'] = 'cache.backend.redis';

  // Always set the fast backend for bootstrap, discover and config, otherwise
  // this gets lost when redis is enabled.
  $settings['cache']['bins']['bootstrap'] = 'cache.backend.chainedfast';
  $settings['cache']['bins']['discovery'] = 'cache.backend.chainedfast';
  $settings['cache']['bins']['config'] = 'cache.backend.chainedfast';
}

if (file_exists(__DIR__ . '/settings.local.php')) {
  include __DIR__ . '/settings.local.php';
}
