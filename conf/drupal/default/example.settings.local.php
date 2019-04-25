<?php

$databases['default']['default'] = [
  'driver' => 'mysql',
  'database' => 'drupal',
  'username' => 'drupal',
  'password' => 'drupal',
  'host' => 'mysql',
  'prefix' => '',
];

$settings['hash_salt'] = 'drupalfr';
$settings['trusted_host_patterns'] = [
  '^localhost$',
  '^127\.0\.0\.1$',
  '^varnish$',
  '^web$',
];

$environment_trusted_host_patterns = [
  'DRUPAL_TRAEFIK_FRONTEND_RULE_HOSTNAME',
  'VARNISH_TRAEFIK_FRONTEND_RULE_HOSTNAME',
];

foreach ($environment_trusted_host_patterns as $environment_trusted_host_pattern) {
  if (getenv($environment_trusted_host_pattern)) {
    $settings['trusted_host_patterns'][] = getenv($environment_trusted_host_pattern);
  }
}

$settings['file_private_path'] = '/project/private_files/default';

// Config split.
$config['config_split.config_split.dev']['status'] = FALSE;
$config['config_split.config_split.prod']['status'] = FALSE;

// Translations.
$config['locale.settings']['translation']['path'] = 'translations/contrib';
$config['locale.settings']['translation']['use_source'] = 'local';

// Redis.
$settings['redis.connection']['interface'] = 'PhpRedis';
$settings['redis.connection']['host'] = 'redis';
$settings['redis.connection']['port'] = '6379';
$settings['redis.connection']['base'] = 0;

$settings['container_yamls'][] = 'modules/contrib/redis/redis.services.yml';
$settings['container_yamls'][] = 'modules/contrib/redis/example.services.yml';

$settings['cache']['default'] = 'cache.backend.redis';

// Varnish.
$config['varnish_purger.settings.varnish']['hostname'] = 'varnish';

// Twitter.
// Go to https://apps.twitter.com to get these tokens.
$config['drupalfr_social.twitter']['consumer_key'] = '';
$config['drupalfr_social.twitter']['consumer_secret'] = '';
$config['drupalfr_social.twitter']['access_token'] = '';
$config['drupalfr_social.twitter']['access_token_secret'] = '';

// Meetup.
$config['drupalfr_meetup.settings']['api_key'] = '';
$config['drupalfr_meetup.settings']['group_urlname'] = '';

// Mailchimp.
$config['mailchimp.settings']['api_key'] = '';

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

if (file_exists($app_root . '/' . $site_path . '/../development.settings.php')) {
  include $app_root . '/' . $site_path . '/../development.settings.php';
}
