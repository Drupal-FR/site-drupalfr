<?php

$databases = array();

$config_directories = array(
  CONFIG_SYNC_DIRECTORY => __DIR__ . '/../../../conf/drupal',
);

$settings['omit_vary_cookie'] = TRUE;

$settings['update_free_access'] = FALSE;
$settings['allow_authorize_operations'] = FALSE;

$settings['file_public_path'] = 'sites/default/files';


$settings['container_yamls'][] = __DIR__ . '/services.yml';

// Performance.
$config['system.performance']['cache']['page']['max_age'] = '86400';
$config['system.performance']['css']['preprocess'] = TRUE;
$config['system.performance']['js']['preprocess'] = TRUE;

// Redis.
$settings['redis.connection']['interface'] = 'PhpRedis';

// External cache.
if (file_exists(__DIR__ . '/.cache_activated')) {
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
