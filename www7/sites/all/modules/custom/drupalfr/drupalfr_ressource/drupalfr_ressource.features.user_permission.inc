<?php
/**
 * @file
 * drupalfr_ressource.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function drupalfr_ressource_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ressource content.
  $permissions['create ressource content'] = array(
    'name' => 'create ressource content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: delete any ressource content.
  $permissions['delete any ressource content'] = array(
    'name' => 'delete any ressource content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: delete own ressource content.
  $permissions['delete own ressource content'] = array(
    'name' => 'delete own ressource content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: edit any ressource content.
  $permissions['edit any ressource content'] = array(
    'name' => 'edit any ressource content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: edit own ressource content.
  $permissions['edit own ressource content'] = array(
    'name' => 'edit own ressource content',
    'roles' => array(),
    'module' => 'node',
  );

  return $permissions;
}
