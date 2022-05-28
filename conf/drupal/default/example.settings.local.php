<?php

declare(strict_types = 1);

// phpcs:ignoreFile
// See https://git.drupalcode.org/project/drupal/blob/HEAD/sites/default/default.settings.php

$databases = [];
$databases['default']['default'] = [
  'username' => $_ENV['DRUPAL_SITE_DEFAULT_DB_USER'],
  'password' => $_ENV['DRUPAL_SITE_DEFAULT_DB_PASSWORD'],
  'database' => $_ENV['DRUPAL_SITE_DEFAULT_DB_DATABASE'],
  'prefix' => $_ENV['DRUPAL_SITE_DEFAULT_DB_PREFIX'],
  'host' => $_ENV['DRUPAL_SITE_DEFAULT_DB_HOST'],
  'port' => $_ENV['DRUPAL_SITE_DEFAULT_DB_PORT'],
  'driver' => $_ENV['DRUPAL_SITE_DEFAULT_DB_DRIVER'],
  'collation' => $_ENV['DRUPAL_SITE_DEFAULT_DB_COLLATION'],
];

$settings['hash_salt'] = $_ENV['DRUPAL_SITE_DEFAULT_HASH_SALT'];

$settings['config_sync_directory'] = $app_root . '/../conf/drupal/' . $_ENV['DRUPAL_SITE_DEFAULT_FOLDER_NAME'] . '/sync';

$settings['update_free_access'] = FALSE;
$settings['allow_authorize_operations'] = FALSE;
$settings['entity_update_backup'] = FALSE;

$settings['file_public_path'] = 'sites/' . $_ENV['DRUPAL_SITE_DEFAULT_FOLDER_NAME'] . '/files';
$settings['file_private_path'] = $app_root . '/../private_files/' . $_ENV['DRUPAL_SITE_DEFAULT_FOLDER_NAME'];

$settings['file_scan_ignore_directories'] = [
  'node_modules',
  'bower_components',
];

// Prevent deletion of orphan files.
// @todo Remove this line when the following issues will be fixed:
// - https://www.drupal.org/node/2801777
// - https://www.drupal.org/node/2708411
// - https://www.drupal.org/node/1239558
// - https://www.drupal.org/node/2666700
// - https://www.drupal.org/node/2810355
$config['system.file']['temporary_maximum_age'] = 0;

// Trusted host pattern.
$settings['trusted_host_patterns'] = [
  '^localhost$',
  '^127\.0\.0\.1$',
  '.*\.ngrok\.io$',
];

$environment_trusted_host_patterns = [
  'WEBSERVER_HOST',
  'VARNISH_HOST',
  'DRUPAL_SITE_DEFAULT_DOMAIN_1',
  'DRUPAL_SITE_DEFAULT_DOMAIN_2',
  'DRUPAL_SITE_DEFAULT_DOMAIN_3',
  'DRUPAL_SITE_DEFAULT_DOMAIN_1_VARNISH',
  'DRUPAL_SITE_DEFAULT_DOMAIN_2_VARNISH',
  'DRUPAL_SITE_DEFAULT_DOMAIN_3_VARNISH',
];

foreach ($environment_trusted_host_patterns as $environment_trusted_host_pattern) {
  if (isset($_ENV[$environment_trusted_host_pattern])) {
    $settings['trusted_host_patterns'][] = '^' . str_replace('.', '\.', $_ENV[$environment_trusted_host_pattern]) . '$';
  }
}

// Reverse proxy.
$settings['reverse_proxy'] = TRUE;
if (isset($_ENV['DRUPAL_SITE_DEFAULT_REVERSE_PROXY_ADDRESSES']) && !empty($_ENV['DRUPAL_SITE_DEFAULT_REVERSE_PROXY_ADDRESSES'])) {
  $settings['reverse_proxy_addresses'] = explode(',', $_ENV['DRUPAL_SITE_DEFAULT_REVERSE_PROXY_ADDRESSES']);
}
else {
  $settings['reverse_proxy_addresses'] = [
    $_SERVER['REMOTE_ADDR'],
  ];
}

// Translations.
$config['locale.settings']['translation']['path'] = 'translations/contrib';
$config['locale.settings']['translation']['use_source'] = 'local';

// Config split.
$config['config_split.config_split.dev']['status'] = (bool) $_ENV['DRUPAL_SITE_DEFAULT_CONFIG_SPLIT_DEV'];
$config['config_split.config_split.overrides']['status'] = (bool) $_ENV['DRUPAL_SITE_DEFAULT_CONFIG_SPLIT_OVERRIDES'];

// Performance.
$settings['omit_vary_cookie'] = (bool) $_ENV['DRUPAL_SITE_DEFAULT_OMIT_VARY_COOKIE'];

$config['system.performance']['cache']['page']['max_age'] = (int) $_ENV['DRUPAL_SITE_DEFAULT_CACHE_PAGE_MAX_AGE'];
$config['system.performance']['css']['preprocess'] = TRUE;
$config['system.performance']['js']['preprocess'] = TRUE;

$settings['cache_prefix'] = $_ENV['DRUPAL_SITE_DEFAULT_CACHE_PREFIX'];
$settings['cache']['default'] = $_ENV['DRUPAL_SITE_DEFAULT_CACHE_DEFAULT'];

// Redis.
if (isset($_ENV['DRUPAL_SITE_DEFAULT_REDIS']) && $_ENV['DRUPAL_SITE_DEFAULT_REDIS'] === 'yes') {
  $settings['redis.connection']['interface'] = $_ENV['DRUPAL_SITE_DEFAULT_REDIS_INTERFACE'];
  $settings['redis.connection']['host'] = $_ENV['DRUPAL_SITE_DEFAULT_REDIS_HOST'];
  $settings['redis.connection']['port'] = $_ENV['DRUPAL_SITE_DEFAULT_REDIS_PORT'];
  if (isset($_ENV['DRUPAL_SITE_DEFAULT_REDIS_PASSWORD']) && !empty($_ENV['DRUPAL_SITE_DEFAULT_REDIS_PASSWORD'])) {
    $settings['redis.connection']['password'] = $_ENV['DRUPAL_SITE_DEFAULT_REDIS_PASSWORD'];
  }
  $settings['redis.connection']['base'] = (int) $_ENV['DRUPAL_SITE_DEFAULT_REDIS_BASE'];
  $settings['redis_compress_length'] = (int) $_ENV['DRUPAL_SITE_DEFAULT_REDIS_COMPRESS_LENGTH'];
  $settings['redis_compress_level'] = (int) $_ENV['DRUPAL_SITE_DEFAULT_REDIS_COMPRESS_LEVEL'];

  $settings['container_yamls'][] = 'modules/contrib/redis/redis.services.yml';
  $settings['container_yamls'][] = 'modules/contrib/redis/example.services.yml';
}

// Varnish.
$config['varnish_purger.settings.varnish']['hostname'] = $_ENV['VARNISH_HOST'];
$config['varnish_purger.settings.varnish']['port'] = (int) $_ENV['VARNISH_PORT'];

// Contact form.
$config['webform.webform.contact']['handlers']['email_notification']['settings']['to_options']['webmaster'] = 'example@example.org';
$config['webform.webform.contact']['handlers']['email_notification']['settings']['to_options']['bureau'] = 'example@example.org';

// Paypal.
$config['commerce_payment.commerce_payment_gateway.paypal_test']['configuration']['api_username'] = '';
$config['commerce_payment.commerce_payment_gateway.paypal_test']['configuration']['api_password'] = '';
$config['commerce_payment.commerce_payment_gateway.paypal_test']['configuration']['signature'] = '';
$config['commerce_payment.commerce_payment_gateway.paypal']['configuration']['api_username'] = '';
$config['commerce_payment.commerce_payment_gateway.paypal']['configuration']['api_password'] = '';
$config['commerce_payment.commerce_payment_gateway.paypal']['configuration']['signature'] = '';

// Errors.
$config['system.logging']['error_level'] = 'hide';

// Services.
$settings['container_yamls'][] = $app_root . '/../conf/drupal/' . $_ENV['DRUPAL_SITE_DEFAULT_FOLDER_NAME'] . '/services.yml';

// Development.
// See https://git.drupalcode.org/project/drupal/blob/HEAD/sites/example.settings.local.php
if (isset($_ENV['DRUPAL_DEVELOPMENT_MODE_ENABLED']) && $_ENV['DRUPAL_DEVELOPMENT_MODE_ENABLED'] === 'yes') {
  assert_options(\ASSERT_ACTIVE, TRUE);
  \Drupal\Component\Assertion\Handle::register();
  $settings['container_yamls'][] = $app_root . '/../conf/drupal/' . $_ENV['DRUPAL_SITE_DEFAULT_FOLDER_NAME'] . '/development.services.yml';
  $config['devel.settings']['devel_dumper'] = 'var_dumper';
  $config['system.logging']['error_level'] = 'verbose';
  $config['views.settings']['ui']['show']['advanced_column'] = TRUE;
  $settings['extension_discovery_scan_tests'] = TRUE;
  $settings['rebuild_access'] = TRUE;
  $settings['skip_permissions_hardening'] = TRUE;

  $config['system.performance']['css']['preprocess'] = FALSE;
  $config['system.performance']['js']['preprocess'] = FALSE;
  $settings['cache']['bins']['dynamic_page_cache'] = 'cache.backend.null';
  $settings['cache']['bins']['page'] = 'cache.backend.null';
  $settings['cache']['bins']['render'] = 'cache.backend.null';

  $settings['http_client_config']['verify'] = FALSE;
}
