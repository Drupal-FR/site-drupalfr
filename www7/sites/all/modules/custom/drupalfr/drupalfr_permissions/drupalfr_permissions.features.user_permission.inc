<?php
/**
 * @file
 * drupalfr_permissions.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function drupalfr_permissions_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access administration pages'.
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access all views'.
  $permissions['access all views'] = array(
    'name' => 'access all views',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'views',
  );

  // Exported permission: 'access comments'.
  $permissions['access comments'] = array(
    'name' => 'access comments',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'access content'.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access content overview'.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access contextual links'.
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'contextual',
  );

  // Exported permission: 'access printer-friendly version'.
  $permissions['access printer-friendly version'] = array(
    'name' => 'access printer-friendly version',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'book',
  );

  // Exported permission: 'access rules debug'.
  $permissions['access rules debug'] = array(
    'name' => 'access rules debug',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'rules',
  );

  // Exported permission: 'access site in maintenance mode'.
  $permissions['access site in maintenance mode'] = array(
    'name' => 'access site in maintenance mode',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access site reports'.
  $permissions['access site reports'] = array(
    'name' => 'access site reports',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access site-wide contact form'.
  $permissions['access site-wide contact form'] = array(
    'name' => 'access site-wide contact form',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'contact',
  );

  // Exported permission: 'access user contact forms'.
  $permissions['access user contact forms'] = array(
    'name' => 'access user contact forms',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'contact',
  );

  // Exported permission: 'access user profiles'.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user',
  );

  // Exported permission: 'add content to books'.
  $permissions['add content to books'] = array(
    'name' => 'add content to books',
    'roles' => array(
      'admin' => 'admin',
      'modérateur documentation' => 'modérateur documentation',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'book',
  );

  // Exported permission: 'add userpoints'.
  $permissions['add userpoints'] = array(
    'name' => 'add userpoints',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'userpoints',
  );

  // Exported permission: 'administer actions'.
  $permissions['administer actions'] = array(
    'name' => 'administer actions',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer blocks'.
  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'block',
  );

  // Exported permission: 'administer book outlines'.
  $permissions['administer book outlines'] = array(
    'name' => 'administer book outlines',
    'roles' => array(
      'admin' => 'admin',
      'modérateur documentation' => 'modérateur documentation',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'book',
  );

  // Exported permission: 'administer bueditor'.
  $permissions['administer bueditor'] = array(
    'name' => 'administer bueditor',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'bueditor',
  );

  // Exported permission: 'administer comments'.
  $permissions['administer comments'] = array(
    'name' => 'administer comments',
    'roles' => array(
      'admin' => 'admin',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'administer contact forms'.
  $permissions['administer contact forms'] = array(
    'name' => 'administer contact forms',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'contact',
  );

  // Exported permission: 'administer content types'.
  $permissions['administer content types'] = array(
    'name' => 'administer content types',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer features'.
  $permissions['administer features'] = array(
    'name' => 'administer features',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'features',
  );

  // Exported permission: 'administer feeds'.
  $permissions['administer feeds'] = array(
    'name' => 'administer feeds',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'administer filters'.
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'administer flags'.
  $permissions['administer flags'] = array(
    'name' => 'administer flags',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'administer forums'.
  $permissions['administer forums'] = array(
    'name' => 'administer forums',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'forum',
  );

  // Exported permission: 'administer image styles'.
  $permissions['administer image styles'] = array(
    'name' => 'administer image styles',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'image',
  );

  // Exported permission: 'administer languages'.
  $permissions['administer languages'] = array(
    'name' => 'administer languages',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'locale',
  );

  // Exported permission: 'administer menu'.
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'menu',
  );

  // Exported permission: 'administer modules'.
  $permissions['administer modules'] = array(
    'name' => 'administer modules',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer nodes'.
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer pathauto'.
  $permissions['administer pathauto'] = array(
    'name' => 'administer pathauto',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'pathauto',
  );

  // Exported permission: 'administer permissions'.
  $permissions['administer permissions'] = array(
    'name' => 'administer permissions',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer redirects'.
  $permissions['administer redirects'] = array(
    'name' => 'administer redirects',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'redirect',
  );

  // Exported permission: 'administer rules'.
  $permissions['administer rules'] = array(
    'name' => 'administer rules',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'rules',
  );

  // Exported permission: 'administer search'.
  $permissions['administer search'] = array(
    'name' => 'administer search',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'search',
  );

  // Exported permission: 'administer site configuration'.
  $permissions['administer site configuration'] = array(
    'name' => 'administer site configuration',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer software updates'.
  $permissions['administer software updates'] = array(
    'name' => 'administer software updates',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer taxonomy'.
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'administer themes'.
  $permissions['administer themes'] = array(
    'name' => 'administer themes',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer url aliases'.
  $permissions['administer url aliases'] = array(
    'name' => 'administer url aliases',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'path',
  );

  // Exported permission: 'administer userpoints'.
  $permissions['administer userpoints'] = array(
    'name' => 'administer userpoints',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'userpoints',
  );

  // Exported permission: 'administer users'.
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer views'.
  $permissions['administer views'] = array(
    'name' => 'administer views',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'views',
  );

  // Exported permission: 'block IP addresses'.
  $permissions['block IP addresses'] = array(
    'name' => 'block IP addresses',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'bypass node access'.
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'bypass rules access'.
  $permissions['bypass rules access'] = array(
    'name' => 'bypass rules access',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'rules',
  );

  // Exported permission: 'cancel account'.
  $permissions['cancel account'] = array(
    'name' => 'cancel account',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user',
  );

  // Exported permission: 'change own username'.
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'clear planet feeds'.
  $permissions['clear planet feeds'] = array(
    'name' => 'clear planet feeds',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'create annuaire content'.
  $permissions['create annuaire content'] = array(
    'name' => 'create annuaire content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create doc content'.
  $permissions['create doc content'] = array(
    'name' => 'create doc content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create forum content'.
  $permissions['create forum content'] = array(
    'name' => 'create forum content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create new books'.
  $permissions['create new books'] = array(
    'name' => 'create new books',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'book',
  );

  // Exported permission: 'create page content'.
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create story content'.
  $permissions['create story content'] = array(
    'name' => 'create story content',
    'roles' => array(
      'admin' => 'admin',
      'contributeur annonces' => 'contributeur annonces',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create url aliases'.
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'path',
  );

  // Exported permission: 'delete any annuaire content'.
  $permissions['delete any annuaire content'] = array(
    'name' => 'delete any annuaire content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any doc content'.
  $permissions['delete any doc content'] = array(
    'name' => 'delete any doc content',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any forum content'.
  $permissions['delete any forum content'] = array(
    'name' => 'delete any forum content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any offre content'.
  $permissions['delete any offre content'] = array(
    'name' => 'delete any offre content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any page content'.
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any story content'.
  $permissions['delete any story content'] = array(
    'name' => 'delete any story content',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own annuaire content'.
  $permissions['delete own annuaire content'] = array(
    'name' => 'delete own annuaire content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'contributeur annonces' => 'contributeur annonces',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own doc content'.
  $permissions['delete own doc content'] = array(
    'name' => 'delete own doc content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own forum content'.
  $permissions['delete own forum content'] = array(
    'name' => 'delete own forum content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur forum' => 'modérateur forum',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own offre content'.
  $permissions['delete own offre content'] = array(
    'name' => 'delete own offre content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own page content'.
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own story content'.
  $permissions['delete own story content'] = array(
    'name' => 'delete own story content',
    'roles' => array(
      'admin' => 'admin',
      'contributeur annonces' => 'contributeur annonces',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete revisions'.
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in nature_poste'.
  $permissions['delete terms in nature_poste'] = array(
    'name' => 'delete terms in nature_poste',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in type_contrat'.
  $permissions['delete terms in type_contrat'] = array(
    'name' => 'delete terms in type_contrat',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_1'.
  $permissions['delete terms in vocabulary_1'] = array(
    'name' => 'delete terms in vocabulary_1',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_2'.
  $permissions['delete terms in vocabulary_2'] = array(
    'name' => 'delete terms in vocabulary_2',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_3'.
  $permissions['delete terms in vocabulary_3'] = array(
    'name' => 'delete terms in vocabulary_3',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_4'.
  $permissions['delete terms in vocabulary_4'] = array(
    'name' => 'delete terms in vocabulary_4',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_5'.
  $permissions['delete terms in vocabulary_5'] = array(
    'name' => 'delete terms in vocabulary_5',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_6'.
  $permissions['delete terms in vocabulary_6'] = array(
    'name' => 'delete terms in vocabulary_6',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_7'.
  $permissions['delete terms in vocabulary_7'] = array(
    'name' => 'delete terms in vocabulary_7',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_8'.
  $permissions['delete terms in vocabulary_8'] = array(
    'name' => 'delete terms in vocabulary_8',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit any annuaire content'.
  $permissions['edit any annuaire content'] = array(
    'name' => 'edit any annuaire content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any doc content'.
  $permissions['edit any doc content'] = array(
    'name' => 'edit any doc content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur documentation' => 'modérateur documentation',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any forum content'.
  $permissions['edit any forum content'] = array(
    'name' => 'edit any forum content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any offre content'.
  $permissions['edit any offre content'] = array(
    'name' => 'edit any offre content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any page content'.
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any story content'.
  $permissions['edit any story content'] = array(
    'name' => 'edit any story content',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own annuaire content'.
  $permissions['edit own annuaire content'] = array(
    'name' => 'edit own annuaire content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own comments'.
  $permissions['edit own comments'] = array(
    'name' => 'edit own comments',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'edit own doc content'.
  $permissions['edit own doc content'] = array(
    'name' => 'edit own doc content',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own forum content'.
  $permissions['edit own forum content'] = array(
    'name' => 'edit own forum content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own page content'.
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own story content'.
  $permissions['edit own story content'] = array(
    'name' => 'edit own story content',
    'roles' => array(
      'admin' => 'admin',
      'contributeur annonces' => 'contributeur annonces',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in nature_poste'.
  $permissions['edit terms in nature_poste'] = array(
    'name' => 'edit terms in nature_poste',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in type_contrat'.
  $permissions['edit terms in type_contrat'] = array(
    'name' => 'edit terms in type_contrat',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_1'.
  $permissions['edit terms in vocabulary_1'] = array(
    'name' => 'edit terms in vocabulary_1',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_2'.
  $permissions['edit terms in vocabulary_2'] = array(
    'name' => 'edit terms in vocabulary_2',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_3'.
  $permissions['edit terms in vocabulary_3'] = array(
    'name' => 'edit terms in vocabulary_3',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_4'.
  $permissions['edit terms in vocabulary_4'] = array(
    'name' => 'edit terms in vocabulary_4',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_5'.
  $permissions['edit terms in vocabulary_5'] = array(
    'name' => 'edit terms in vocabulary_5',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_6'.
  $permissions['edit terms in vocabulary_6'] = array(
    'name' => 'edit terms in vocabulary_6',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_7'.
  $permissions['edit terms in vocabulary_7'] = array(
    'name' => 'edit terms in vocabulary_7',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_8'.
  $permissions['edit terms in vocabulary_8'] = array(
    'name' => 'edit terms in vocabulary_8',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit userpoints'.
  $permissions['edit userpoints'] = array(
    'name' => 'edit userpoints',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'userpoints',
  );

  // Exported permission: 'import planet feeds'.
  $permissions['import planet feeds'] = array(
    'name' => 'import planet feeds',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'manage features'.
  $permissions['manage features'] = array(
    'name' => 'manage features',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'features',
  );

  // Exported permission: 'moderate userpoints'.
  $permissions['moderate userpoints'] = array(
    'name' => 'moderate userpoints',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'userpoints',
  );

  // Exported permission: 'notify of path changes'.
  $permissions['notify of path changes'] = array(
    'name' => 'notify of path changes',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'pathauto',
  );

  // Exported permission: 'post comments'.
  $permissions['post comments'] = array(
    'name' => 'post comments',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'publish any content'.
  $permissions['publish any content'] = array(
    'name' => 'publish any content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'publish any offre content'.
  $permissions['publish any offre content'] = array(
    'name' => 'publish any offre content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'publish any page content'.
  $permissions['publish any page content'] = array(
    'name' => 'publish any page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'publish editable offre content'.
  $permissions['publish editable offre content'] = array(
    'name' => 'publish editable offre content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'publish editable page content'.
  $permissions['publish editable page content'] = array(
    'name' => 'publish editable page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'publish own offre content'.
  $permissions['publish own offre content'] = array(
    'name' => 'publish own offre content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'publish own page content'.
  $permissions['publish own page content'] = array(
    'name' => 'publish own page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'revert revisions'.
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      'admin' => 'admin',
      'modérateur documentation' => 'modérateur documentation',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'search content'.
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'search',
  );

  // Exported permission: 'select account cancellation method'.
  $permissions['select account cancellation method'] = array(
    'name' => 'select account cancellation method',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user',
  );

  // Exported permission: 'skip comment approval'.
  $permissions['skip comment approval'] = array(
    'name' => 'skip comment approval',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'translate interface'.
  $permissions['translate interface'] = array(
    'name' => 'translate interface',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'locale',
  );

  // Exported permission: 'unlock planet feeds'.
  $permissions['unlock planet feeds'] = array(
    'name' => 'unlock planet feeds',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'unpublish any content'.
  $permissions['unpublish any content'] = array(
    'name' => 'unpublish any content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'unpublish any offre content'.
  $permissions['unpublish any offre content'] = array(
    'name' => 'unpublish any offre content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'unpublish any page content'.
  $permissions['unpublish any page content'] = array(
    'name' => 'unpublish any page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'unpublish editable offre content'.
  $permissions['unpublish editable offre content'] = array(
    'name' => 'unpublish editable offre content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'unpublish editable page content'.
  $permissions['unpublish editable page content'] = array(
    'name' => 'unpublish editable page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'unpublish own page content'.
  $permissions['unpublish own page content'] = array(
    'name' => 'unpublish own page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'publishcontent',
  );

  // Exported permission: 'use PHP for settings'.
  $permissions['use PHP for settings'] = array(
    'name' => 'use PHP for settings',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'php',
  );

  // Exported permission: 'use advanced search'.
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'search',
  );

  // Exported permission: 'use flag import'.
  $permissions['use flag import'] = array(
    'name' => 'use flag import',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'use text format 1'.
  $permissions['use text format 1'] = array(
    'name' => 'use text format 1',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'contributeur annonces' => 'contributeur annonces',
      'modérateur documentation' => 'modérateur documentation',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 2'.
  $permissions['use text format 2'] = array(
    'name' => 'use text format 2',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 3'.
  $permissions['use text format 3'] = array(
    'name' => 'use text format 3',
    'roles' => array(
      'admin' => 'admin',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 7'.
  $permissions['use text format 7'] = array(
    'name' => 'use text format 7',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 8'.
  $permissions['use text format 8'] = array(
    'name' => 'use text format 8',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'view own unpublished content'.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view own userpoints'.
  $permissions['view own userpoints'] = array(
    'name' => 'view own userpoints',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'userpoints',
  );

  // Exported permission: 'view revisions'.
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'contributeur annonces' => 'contributeur annonces',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'admin' => 'admin',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'system',
  );

  // Exported permission: 'view userpoints'.
  $permissions['view userpoints'] = array(
    'name' => 'view userpoints',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'userpoints',
  );

  return $permissions;
}
