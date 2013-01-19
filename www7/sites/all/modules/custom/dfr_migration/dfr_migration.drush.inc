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
  $items['dfr-update-pathauto-aliases'] = array(
    'description' => "Create missing pathauto aliases",
    'aliases' => array('dfrupa'),
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
  // Delete Redirect that are aliases
  $query = db_select('redirect', 'r');
  $query->join('url_alias', 'u', 'r.redirect = u.source AND r.source = u.alias');
  $query->fields('r', array('rid'));
  $results = $query->execute();
  foreach($results as $redirect) {
    redirect_delete($redirect->rid);
  }
  // Delete Redirect that are anti-aliases
  $query = db_select('redirect', 'r');
  $query->join('url_alias', 'u', 'r.redirect = u.alias AND r.source = u.source');
  $query->fields('r', array('rid'));
  $results = $query->execute();
  foreach($results as $redirect) {
    redirect_delete($redirect->rid);
  }
  // Delete redirects that are redirecting to another anti-redirect
  $query = db_select('redirect', 'r');
  $query->join('redirect', 'r2', 'r.source = r2.redirect AND r.redirect = r2.source AND r.rid <> r2.rid');
  $query->fields('r', array('rid'));
  $results = $query->execute();
  foreach($results as $redirect) {
    redirect_delete($redirect->rid);
  }
}

function drush_dfr_migration_dfr_update_pathauto_aliases() {
  $nids = db_select('node', 'n')
   ->fields('n', array('nid'))
   ->condition('type', array('annuaire','doc'), 'IN')
   ->execute()
   ->fetchAllAssoc('nid');
  pathauto_node_update_alias_multiple(array_keys($nids), 'update');
}
