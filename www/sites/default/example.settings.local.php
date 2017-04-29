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

// Redis.
$settings['redis.connection']['host'] = 'redis';

// Varnish.
$config['varnish_purger.settings.varnish']['hostname'] = 'varnish';

// Solr.
$config['search_api.server.solr']['backend_config']['connector_config']['host'] = 'solr';

// Twitter.
// Go to https://apps.twitter.com to get these tokens.
$config['drupalfr_social.twitter']['consumer_key'] = '';
$config['drupalfr_social.twitter']['consumer_secret'] = '';
$config['drupalfr_social.twitter']['access_token'] = '';
$config['drupalfr_social.twitter']['access_token_secret'] = '';

// Mailchimp.
$config['mailchimp.settings']['api_key'] = '';

if (file_exists(__DIR__ . '/../development.settings.php')) {
  include __DIR__ . '/../development.settings.php';
}
