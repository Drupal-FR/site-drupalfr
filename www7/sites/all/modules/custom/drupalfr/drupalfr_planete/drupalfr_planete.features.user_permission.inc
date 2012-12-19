<?php
/**
 * @file
 * drupalfr_planete.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function drupalfr_planete_user_default_permissions() {
  $permissions = array();

  // Exported permission: create planete content.
  $permissions['create planete content'] = array(
    'name' => 'create planete content',
    'roles' => array(
      0 => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: create planete_feed_source content.
  $permissions['create planete_feed_source content'] = array(
    'name' => 'create planete_feed_source content',
    'roles' => array(
      0 => 'admin',
      1 => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any planete content.
  $permissions['delete any planete content'] = array(
    'name' => 'delete any planete content',
    'roles' => array(
      0 => 'admin',
      1 => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any planete_feed_source content.
  $permissions['delete any planete_feed_source content'] = array(
    'name' => 'delete any planete_feed_source content',
    'roles' => array(
      0 => 'admin',
      1 => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own planete content.
  $permissions['delete own planete content'] = array(
    'name' => 'delete own planete content',
    'roles' => array(
      0 => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own planete_feed_source content.
  $permissions['delete own planete_feed_source content'] = array(
    'name' => 'delete own planete_feed_source content',
    'roles' => array(
      0 => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any planete content.
  $permissions['edit any planete content'] = array(
    'name' => 'edit any planete content',
    'roles' => array(
      0 => 'admin',
      1 => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any planete_feed_source content.
  $permissions['edit any planete_feed_source content'] = array(
    'name' => 'edit any planete_feed_source content',
    'roles' => array(
      0 => 'admin',
      1 => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own planete content.
  $permissions['edit own planete content'] = array(
    'name' => 'edit own planete content',
    'roles' => array(
      0 => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own planete_feed_source content.
  $permissions['edit own planete_feed_source content'] = array(
    'name' => 'edit own planete_feed_source content',
    'roles' => array(
      0 => 'admin',
    ),
    'module' => 'node',
  );

  return $permissions;
}
