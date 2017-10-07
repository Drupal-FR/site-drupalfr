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
  '^127\.0\.0\.1$',
  'varnish',
  'web',
];

$settings['file_private_path'] = '/project/private_files/default';

// Translations.
$config['locale.settings']['translation']['path'] = 'translations/contrib';
$config['locale.settings']['translation']['use_source'] = 'local';

// Redis.
$settings['redis.connection']['host'] = 'redis';

// Varnish.
$config['varnish_purger.settings.varnish']['hostname'] = 'varnish';

// Solr.
$config['search_api.server.solr']['backend_config']['connector_config']['host'] = 'solr';
$config['search_api.server.showcase']['backend_config']['connector_config']['host'] = 'solr-showcase';

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
