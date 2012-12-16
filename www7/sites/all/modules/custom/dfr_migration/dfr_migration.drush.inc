<?php

/**
 * Implements hook_drush_command().
 */
function dfr_migration_drush_command() {
  $items = array();

  $items['dfr-user-migration'] = array(
    'description' => "Export a feature from your site into a module.",
    //'drupal dependencies' => array('dfr_migration'),
    'aliases' => array('dfrum'),
  );

  return $items;
}

/**
 * Drush command: dfr-user-migration.
 */
function drush_dfr_migration_dfr_user_migration() {
  $batch = array(
    'operations' => array(
      array('dfr_migration_migrate_profiles_batch', array()),
    ),
    'finished' => 'dfr_migration_migrate_profiles_batch_finished',
    'title' => t('Migrating users'),
    'init_message' => t('Préparation de la migration des profils.'),
    'progress_message' => t('Traitement @current sur @total.'),
    'error_message' => t('Le batch de migraiton des profils a échoué.'),
  );
  batch_set($batch);

  $batch = &batch_get();
  $batch['progressive'] = FALSE;

  drush_backend_batch_process();
}
