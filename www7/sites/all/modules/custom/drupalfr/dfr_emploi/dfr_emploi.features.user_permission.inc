<?php
/**
 * @file
 * dfr_emploi.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function dfr_emploi_user_default_permissions() {
  $permissions = array();

  // Exported permission: create offre content.
  $permissions['create offre content'] = array(
    'name' => 'create offre content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own offre content.
  $permissions['edit own offre content'] = array(
    'name' => 'edit own offre content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: unpublish own offre content.
  $permissions['unpublish own offre content'] = array(
    'name' => 'unpublish own offre content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'publishcontent',
  );

  return $permissions;
}
