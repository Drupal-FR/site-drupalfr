<?php

$databases = array();

$config_directories = array(
  CONFIG_SYNC_DIRECTORY => __DIR__ . '/../../../conf',
);

$settings['omit_vary_cookie'] = TRUE;

$settings['update_free_access'] = FALSE;
$settings['allow_authorize_operations'] = FALSE;

$settings['file_public_path'] = 'sites/default/files';


$settings['container_yamls'][] = __DIR__ . '/services.yml';

if (file_exists(__DIR__ . '/settings.local.php')) {
  include __DIR__ . '/settings.local.php';
}
