<?php

/**
 * @file
 * drupalfr_user.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function drupalfr_user_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer BOTCHA settings'.
  $permissions['administer BOTCHA settings'] = array(
    'name' => 'administer BOTCHA settings',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'botcha',
  );

  // Exported permission: 'administer CAPTCHA settings'.
  $permissions['administer CAPTCHA settings'] = array(
    'name' => 'administer CAPTCHA settings',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'captcha',
  );

  // Exported permission: 'administer honeypot'.
  $permissions['administer honeypot'] = array(
    'name' => 'administer honeypot',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'honeypot',
  );

  // Exported permission: 'administer recaptcha'.
  $permissions['administer recaptcha'] = array(
    'name' => 'administer recaptcha',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'recaptcha',
  );

  // Exported permission: 'bypass honeypot protection'.
  $permissions['bypass honeypot protection'] = array(
    'name' => 'bypass honeypot protection',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'honeypot',
  );

  // Exported permission: 'create field_facebook'.
  $permissions['create field_facebook'] = array(
    'name' => 'create field_facebook',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_google_plus'.
  $permissions['create field_google_plus'] = array(
    'name' => 'create field_google_plus',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_linkedin'.
  $permissions['create field_linkedin'] = array(
    'name' => 'create field_linkedin',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_membership_number'.
  $permissions['create field_membership_number'] = array(
    'name' => 'create field_membership_number',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_twitter'.
  $permissions['create field_twitter'] = array(
    'name' => 'create field_twitter',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_viadeo'.
  $permissions['create field_viadeo'] = array(
    'name' => 'create field_viadeo',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_facebook'.
  $permissions['edit field_facebook'] = array(
    'name' => 'edit field_facebook',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_google_plus'.
  $permissions['edit field_google_plus'] = array(
    'name' => 'edit field_google_plus',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_linkedin'.
  $permissions['edit field_linkedin'] = array(
    'name' => 'edit field_linkedin',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_membership_number'.
  $permissions['edit field_membership_number'] = array(
    'name' => 'edit field_membership_number',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_twitter'.
  $permissions['edit field_twitter'] = array(
    'name' => 'edit field_twitter',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_viadeo'.
  $permissions['edit field_viadeo'] = array(
    'name' => 'edit field_viadeo',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_facebook'.
  $permissions['edit own field_facebook'] = array(
    'name' => 'edit own field_facebook',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_google_plus'.
  $permissions['edit own field_google_plus'] = array(
    'name' => 'edit own field_google_plus',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_linkedin'.
  $permissions['edit own field_linkedin'] = array(
    'name' => 'edit own field_linkedin',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_membership_number'.
  $permissions['edit own field_membership_number'] = array(
    'name' => 'edit own field_membership_number',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_twitter'.
  $permissions['edit own field_twitter'] = array(
    'name' => 'edit own field_twitter',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_viadeo'.
  $permissions['edit own field_viadeo'] = array(
    'name' => 'edit own field_viadeo',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'skip BOTCHA'.
  $permissions['skip BOTCHA'] = array(
    'name' => 'skip BOTCHA',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'botcha',
  );

  // Exported permission: 'skip CAPTCHA'.
  $permissions['skip CAPTCHA'] = array(
    'name' => 'skip CAPTCHA',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'captcha',
  );

  // Exported permission: 'view any unpublished faq content'.
  $permissions['view any unpublished faq content'] = array(
    'name' => 'view any unpublished faq content',
    'roles' => array(),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view field_facebook'.
  $permissions['view field_facebook'] = array(
    'name' => 'view field_facebook',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_google_plus'.
  $permissions['view field_google_plus'] = array(
    'name' => 'view field_google_plus',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_linkedin'.
  $permissions['view field_linkedin'] = array(
    'name' => 'view field_linkedin',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_membership_number'.
  $permissions['view field_membership_number'] = array(
    'name' => 'view field_membership_number',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_twitter'.
  $permissions['view field_twitter'] = array(
    'name' => 'view field_twitter',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_viadeo'.
  $permissions['view field_viadeo'] = array(
    'name' => 'view field_viadeo',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_facebook'.
  $permissions['view own field_facebook'] = array(
    'name' => 'view own field_facebook',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_google_plus'.
  $permissions['view own field_google_plus'] = array(
    'name' => 'view own field_google_plus',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_linkedin'.
  $permissions['view own field_linkedin'] = array(
    'name' => 'view own field_linkedin',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_membership_number'.
  $permissions['view own field_membership_number'] = array(
    'name' => 'view own field_membership_number',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_twitter'.
  $permissions['view own field_twitter'] = array(
    'name' => 'view own field_twitter',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_viadeo'.
  $permissions['view own field_viadeo'] = array(
    'name' => 'view own field_viadeo',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
