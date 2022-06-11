<?php

// phpcs:ignoreFile

if (file_exists($app_root . '/' . $site_path . '/../../../conf/drupal/default/settings.local.php')) {
  include $app_root . '/' . $site_path . '/../../../conf/drupal/default/settings.local.php';
}
