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

  // Exported permission: 'create field_flag_moderation'.
  $permissions['create field_flag_moderation'] = array(
    'name' => 'create field_flag_moderation',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create offre content'.
  $permissions['create offre content'] = array(
    'name' => 'create offre content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit field_flag_moderation'.
  $permissions['edit field_flag_moderation'] = array(
    'name' => 'edit field_flag_moderation',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_flag_moderation'.
  $permissions['edit own field_flag_moderation'] = array(
    'name' => 'edit own field_flag_moderation',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own offre content'.
  $permissions['edit own offre content'] = array(
    'name' => 'edit own offre content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'unpublish own offre content'.
  $permissions['unpublish own offre content'] = array(
    'name' => 'unpublish own offre content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'view field_flag_moderation'.
  $permissions['view field_flag_moderation'] = array(
    'name' => 'view field_flag_moderation',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_flag_moderation'.
  $permissions['view own field_flag_moderation'] = array(
    'name' => 'view own field_flag_moderation',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
