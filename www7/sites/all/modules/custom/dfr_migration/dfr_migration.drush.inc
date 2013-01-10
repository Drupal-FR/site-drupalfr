<?php

/**
 * Implements hook_drush_command().
 */
function dfr_migration_drush_command() {
  $items = array();

  $items['dfr-user-migration'] = array(
    'description' => "Migrate users during Dfr migration",
    //'drupal dependencies' => array('dfr_migration'),
    'aliases' => array('dfrum'),
  );
  $items['dfr-remove-alias-redirect'] = array(
    'description' => "Remove redirects that are aliases",
    'aliases' => array('dfrrar'),
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

function drush_dfr_migration_dfr_remove_alias_redirect() {
  $query = db_select('redirect', 'r');
  $query->join('url_alias', 'u', 'r.redirect = u.source AND r.source = u.alias');
  $query->fields('r', array('rid'));
  $results = $query->execute();
  foreach($results as $redirect) {
    redirect_delete($redirect->rid);
  }
}
