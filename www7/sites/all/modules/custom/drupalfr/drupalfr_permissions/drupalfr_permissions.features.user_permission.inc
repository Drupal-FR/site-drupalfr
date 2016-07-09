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

  // Exported permission: 'access administration menu'.
  $permissions['access administration menu'] = array(
    'name' => 'access administration menu',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'admin_menu',
  );

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

  // Exported permission: 'access checkout'.
  $permissions['access checkout'] = array(
    'name' => 'access checkout',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_checkout',
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

  // Exported permission: 'access job offer comments'.
  $permissions['access job offer comments'] = array(
    'name' => 'access job offer comments',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
    ),
    'module' => 'dfr_emploi',
  );

  // Exported permission: 'access mailchimp signup pages'.
  $permissions['access mailchimp signup pages'] = array(
    'name' => 'access mailchimp signup pages',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'mailchimp_signup',
  );

  // Exported permission: 'access mollom statistics'.
  $permissions['access mollom statistics'] = array(
    'name' => 'access mollom statistics',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'mollom',
  );

  // Exported permission: 'access printer-friendly version'.
  $permissions['access printer-friendly version'] = array(
    'name' => 'access printer-friendly version',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'book',
  );

  // Exported permission: 'access private fields'.
  $permissions['access private fields'] = array(
    'name' => 'access private fields',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
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

  // Exported permission: 'administer checkout'.
  $permissions['administer checkout'] = array(
    'name' => 'administer checkout',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_checkout',
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

  // Exported permission: 'administer commerce_customer_profile entities'.
  $permissions['administer commerce_customer_profile entities'] = array(
    'name' => 'administer commerce_customer_profile entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'administer commerce_order entities'.
  $permissions['administer commerce_order entities'] = array(
    'name' => 'administer commerce_order entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'administer commerce_product entities'.
  $permissions['administer commerce_product entities'] = array(
    'name' => 'administer commerce_product entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
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

  // Exported permission: 'administer contexts'.
  $permissions['administer contexts'] = array(
    'name' => 'administer contexts',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'context_ui',
  );

  // Exported permission: 'administer customer profile types'.
  $permissions['administer customer profile types'] = array(
    'name' => 'administer customer profile types',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'administer facets'.
  $permissions['administer facets'] = array(
    'name' => 'administer facets',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'facetapi',
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

  // Exported permission: 'administer field permissions'.
  $permissions['administer field permissions'] = array(
    'name' => 'administer field permissions',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'administer fieldgroups'.
  $permissions['administer fieldgroups'] = array(
    'name' => 'administer fieldgroups',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field_group',
  );

  // Exported permission: 'administer fields'.
  $permissions['administer fields'] = array(
    'name' => 'administer fields',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'field',
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

  // Exported permission: 'administer google analytics'.
  $permissions['administer google analytics'] = array(
    'name' => 'administer google analytics',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'googleanalytics',
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

  // Exported permission: 'administer licenses'.
  $permissions['administer licenses'] = array(
    'name' => 'administer licenses',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_license',
  );

  // Exported permission: 'administer line item types'.
  $permissions['administer line item types'] = array(
    'name' => 'administer line item types',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_line_item',
  );

  // Exported permission: 'administer line items'.
  $permissions['administer line items'] = array(
    'name' => 'administer line items',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_line_item',
  );

  // Exported permission: 'administer mailchimp'.
  $permissions['administer mailchimp'] = array(
    'name' => 'administer mailchimp',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'mailchimp',
  );

  // Exported permission: 'administer mailchimp signup entities'.
  $permissions['administer mailchimp signup entities'] = array(
    'name' => 'administer mailchimp signup entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'mailchimp_signup',
  );

  // Exported permission: 'administer menu'.
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'menu',
  );

  // Exported permission: 'administer menu_trail_by_path'.
  $permissions['administer menu_trail_by_path'] = array(
    'name' => 'administer menu_trail_by_path',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'menu_trail_by_path',
  );

  // Exported permission: 'administer meta tags'.
  $permissions['administer meta tags'] = array(
    'name' => 'administer meta tags',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'metatag',
  );

  // Exported permission: 'administer module filter'.
  $permissions['administer module filter'] = array(
    'name' => 'administer module filter',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'module_filter',
  );

  // Exported permission: 'administer modules'.
  $permissions['administer modules'] = array(
    'name' => 'administer modules',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer mollom'.
  $permissions['administer mollom'] = array(
    'name' => 'administer mollom',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'mollom',
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

  // Exported permission: 'administer payment methods'.
  $permissions['administer payment methods'] = array(
    'name' => 'administer payment methods',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: 'administer payments'.
  $permissions['administer payments'] = array(
    'name' => 'administer payments',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: 'administer permissions'.
  $permissions['administer permissions'] = array(
    'name' => 'administer permissions',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer product pricing'.
  $permissions['administer product pricing'] = array(
    'name' => 'administer product pricing',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product_pricing_ui',
  );

  // Exported permission: 'administer product types'.
  $permissions['administer product types'] = array(
    'name' => 'administer product types',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
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

  // Exported permission: 'administer scald'.
  $permissions['administer scald'] = array(
    'name' => 'administer scald',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'administer scald atoms'.
  $permissions['administer scald atoms'] = array(
    'name' => 'administer scald atoms',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'administer search'.
  $permissions['administer search'] = array(
    'name' => 'administer search',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'search',
  );

  // Exported permission: 'administer search_api'.
  $permissions['administer search_api'] = array(
    'name' => 'administer search_api',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'search_api',
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

  // Exported permission: 'administer twitter block'.
  $permissions['administer twitter block'] = array(
    'name' => 'administer twitter block',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'twitter_block',
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

  // Exported permission: 'administer userprotect'.
  $permissions['administer userprotect'] = array(
    'name' => 'administer userprotect',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'userprotect',
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

  // Exported permission: 'ajax order expiration'.
  $permissions['ajax order expiration'] = array(
    'name' => 'ajax order expiration',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_cart_expiration',
  );

  // Exported permission: 'block IP addresses'.
  $permissions['block IP addresses'] = array(
    'name' => 'block IP addresses',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'system',
  );

  // Exported permission: 'bypass atom access restrictions'.
  $permissions['bypass atom access restrictions'] = array(
    'name' => 'bypass atom access restrictions',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'bypass mollom protection'.
  $permissions['bypass mollom protection'] = array(
    'name' => 'bypass mollom protection',
    'roles' => array(
      'admin' => 'admin',
      'modérateur documentation' => 'modérateur documentation',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'mollom',
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

  // Exported permission: 'bypass restricted forums'.
  $permissions['bypass restricted forums'] = array(
    'name' => 'bypass restricted forums',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'drupalfr_forums',
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

  // Exported permission: 'change own e-mail'.
  $permissions['change own e-mail'] = array(
    'name' => 'change own e-mail',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'userprotect',
  );

  // Exported permission: 'change own openid'.
  $permissions['change own openid'] = array(
    'name' => 'change own openid',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'userprotect',
  );

  // Exported permission: 'change own password'.
  $permissions['change own password'] = array(
    'name' => 'change own password',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'userprotect',
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

  // Exported permission: 'configure order settings'.
  $permissions['configure order settings'] = array(
    'name' => 'configure order settings',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'configure store'.
  $permissions['configure store'] = array(
    'name' => 'configure store',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce',
  );

  // Exported permission: 'context ajax block access'.
  $permissions['context ajax block access'] = array(
    'name' => 'context ajax block access',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'context_ui',
  );

  // Exported permission: 'create annuaire content'.
  $permissions['create annuaire content'] = array(
    'name' => 'create annuaire content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create atom of any type'.
  $permissions['create atom of any type'] = array(
    'name' => 'create atom of any type',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'create atom of audio type'.
  $permissions['create atom of audio type'] = array(
    'name' => 'create atom of audio type',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'create atom of file type'.
  $permissions['create atom of file type'] = array(
    'name' => 'create atom of file type',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'create commerce_customer_profile entities'.
  $permissions['create commerce_customer_profile entities'] = array(
    'name' => 'create commerce_customer_profile entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'create commerce_customer_profile entities of bundle billing'.
  $permissions['create commerce_customer_profile entities of bundle billing'] = array(
    'name' => 'create commerce_customer_profile entities of bundle billing',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'create commerce_order entities'.
  $permissions['create commerce_order entities'] = array(
    'name' => 'create commerce_order entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'create commerce_order entities of bundle commerce_order'.
  $permissions['create commerce_order entities of bundle commerce_order'] = array(
    'name' => 'create commerce_order entities of bundle commerce_order',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'create commerce_product entities'.
  $permissions['create commerce_product entities'] = array(
    'name' => 'create commerce_product entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'create commerce_product entities of bundle individual_membership'.
  $permissions['create commerce_product entities of bundle individual_membership'] = array(
    'name' => 'create commerce_product entities of bundle individual_membership',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'create commerce_product entities of bundle product'.
  $permissions['create commerce_product entities of bundle product'] = array(
    'name' => 'create commerce_product entities of bundle product',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'create doc content'.
  $permissions['create doc content'] = array(
    'name' => 'create doc content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
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

  // Exported permission: 'create field_message_moderation'.
  $permissions['create field_message_moderation'] = array(
    'name' => 'create field_message_moderation',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
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

  // Exported permission: 'create payments'.
  $permissions['create payments'] = array(
    'name' => 'create payments',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: 'create planete content'.
  $permissions['create planete content'] = array(
    'name' => 'create planete content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create planete_feed_source content'.
  $permissions['create planete_feed_source content'] = array(
    'name' => 'create planete_feed_source content',
    'roles' => array(
      'admin' => 'admin',
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

  // Exported permission: 'delete any atom'.
  $permissions['delete any atom'] = array(
    'name' => 'delete any atom',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
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

  // Exported permission: 'delete any planete content'.
  $permissions['delete any planete content'] = array(
    'name' => 'delete any planete content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any planete_feed_source content'.
  $permissions['delete any planete_feed_source content'] = array(
    'name' => 'delete any planete_feed_source content',
    'roles' => array(
      'admin' => 'admin',
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

  // Exported permission: 'delete own atom'.
  $permissions['delete own atom'] = array(
    'name' => 'delete own atom',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
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

  // Exported permission: 'delete own planete content'.
  $permissions['delete own planete content'] = array(
    'name' => 'delete own planete content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own planete_feed_source content'.
  $permissions['delete own planete_feed_source content'] = array(
    'name' => 'delete own planete_feed_source content',
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

  // Exported permission: 'delete payments'.
  $permissions['delete payments'] = array(
    'name' => 'delete payments',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_payment',
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

  // Exported permission: 'delete terms in scald_authors'.
  $permissions['delete terms in scald_authors'] = array(
    'name' => 'delete terms in scald_authors',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in scald_tags'.
  $permissions['delete terms in scald_tags'] = array(
    'name' => 'delete terms in scald_tags',
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

  // Exported permission: 'display drupal links'.
  $permissions['display drupal links'] = array(
    'name' => 'display drupal links',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'admin_menu',
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

  // Exported permission: 'edit any atom'.
  $permissions['edit any atom'] = array(
    'name' => 'edit any atom',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'edit any commerce_customer_profile entity'.
  $permissions['edit any commerce_customer_profile entity'] = array(
    'name' => 'edit any commerce_customer_profile entity',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'edit any commerce_customer_profile entity of bundle billing'.
  $permissions['edit any commerce_customer_profile entity of bundle billing'] = array(
    'name' => 'edit any commerce_customer_profile entity of bundle billing',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'edit any commerce_order entity'.
  $permissions['edit any commerce_order entity'] = array(
    'name' => 'edit any commerce_order entity',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'edit any commerce_order entity of bundle commerce_order'.
  $permissions['edit any commerce_order entity of bundle commerce_order'] = array(
    'name' => 'edit any commerce_order entity of bundle commerce_order',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'edit any commerce_product entity'.
  $permissions['edit any commerce_product entity'] = array(
    'name' => 'edit any commerce_product entity',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'edit any commerce_product entity of bundle individual_membership'.
  $permissions['edit any commerce_product entity of bundle individual_membership'] = array(
    'name' => 'edit any commerce_product entity of bundle individual_membership',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'edit any commerce_product entity of bundle product'.
  $permissions['edit any commerce_product entity of bundle product'] = array(
    'name' => 'edit any commerce_product entity of bundle product',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
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

  // Exported permission: 'edit any planete content'.
  $permissions['edit any planete content'] = array(
    'name' => 'edit any planete content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any planete_feed_source content'.
  $permissions['edit any planete_feed_source content'] = array(
    'name' => 'edit any planete_feed_source content',
    'roles' => array(
      'admin' => 'admin',
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

  // Exported permission: 'edit field_message_moderation'.
  $permissions['edit field_message_moderation'] = array(
    'name' => 'edit field_message_moderation',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
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

  // Exported permission: 'edit meta tags'.
  $permissions['edit meta tags'] = array(
    'name' => 'edit meta tags',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'metatag',
  );

  // Exported permission: 'edit own account'.
  $permissions['edit own account'] = array(
    'name' => 'edit own account',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'contributeur annonces' => 'contributeur annonces',
      'modérateur documentation' => 'modérateur documentation',
      'modérateur emploi' => 'modérateur emploi',
      'modérateur forum' => 'modérateur forum',
      'node administrateurs' => 'node administrateurs',
    ),
    'module' => 'userprotect',
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

  // Exported permission: 'edit own atom'.
  $permissions['edit own atom'] = array(
    'name' => 'edit own atom',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'edit own comments'.
  $permissions['edit own comments'] = array(
    'name' => 'edit own comments',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'edit own commerce_customer_profile entities'.
  $permissions['edit own commerce_customer_profile entities'] = array(
    'name' => 'edit own commerce_customer_profile entities',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'edit own commerce_customer_profile entities of bundle billing'.
  $permissions['edit own commerce_customer_profile entities of bundle billing'] = array(
    'name' => 'edit own commerce_customer_profile entities of bundle billing',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'edit own commerce_order entities'.
  $permissions['edit own commerce_order entities'] = array(
    'name' => 'edit own commerce_order entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'edit own commerce_order entities of bundle commerce_order'.
  $permissions['edit own commerce_order entities of bundle commerce_order'] = array(
    'name' => 'edit own commerce_order entities of bundle commerce_order',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'edit own commerce_product entities'.
  $permissions['edit own commerce_product entities'] = array(
    'name' => 'edit own commerce_product entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'edit own commerce_product entities of bundle individual_membership'.
  $permissions['edit own commerce_product entities of bundle individual_membership'] = array(
    'name' => 'edit own commerce_product entities of bundle individual_membership',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'edit own commerce_product entities of bundle product'.
  $permissions['edit own commerce_product entities of bundle product'] = array(
    'name' => 'edit own commerce_product entities of bundle product',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
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

  // Exported permission: 'edit own field_message_moderation'.
  $permissions['edit own field_message_moderation'] = array(
    'name' => 'edit own field_message_moderation',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
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

  // Exported permission: 'edit own planete content'.
  $permissions['edit own planete content'] = array(
    'name' => 'edit own planete content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own planete_feed_source content'.
  $permissions['edit own planete_feed_source content'] = array(
    'name' => 'edit own planete_feed_source content',
    'roles' => array(
      'admin' => 'admin',
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

  // Exported permission: 'edit terms in scald_authors'.
  $permissions['edit terms in scald_authors'] = array(
    'name' => 'edit terms in scald_authors',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in scald_tags'.
  $permissions['edit terms in scald_tags'] = array(
    'name' => 'edit terms in scald_tags',
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

  // Exported permission: 'fetch any atom'.
  $permissions['fetch any atom'] = array(
    'name' => 'fetch any atom',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'fetch own atom'.
  $permissions['fetch own atom'] = array(
    'name' => 'fetch own atom',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'flag accepted_answer'.
  $permissions['flag accepted_answer'] = array(
    'name' => 'flag accepted_answer',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flush caches'.
  $permissions['flush caches'] = array(
    'name' => 'flush caches',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'generate features'.
  $permissions['generate features'] = array(
    'name' => 'generate features',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'features',
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

  // Exported permission: 'opt-in or out of tracking'.
  $permissions['opt-in or out of tracking'] = array(
    'name' => 'opt-in or out of tracking',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'googleanalytics',
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

  // Exported permission: 'publish editable content'.
  $permissions['publish editable content'] = array(
    'name' => 'publish editable content',
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

  // Exported permission: 'rename features'.
  $permissions['rename features'] = array(
    'name' => 'rename features',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'features',
  );

  // Exported permission: 'report to mollom'.
  $permissions['report to mollom'] = array(
    'name' => 'report to mollom',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'mollom',
  );

  // Exported permission: 'restrict atom access'.
  $permissions['restrict atom access'] = array(
    'name' => 'restrict atom access',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'scald',
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

  // Exported permission: 'unflag accepted_answer'.
  $permissions['unflag accepted_answer'] = array(
    'name' => 'unflag accepted_answer',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
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

  // Exported permission: 'unpublish editable content'.
  $permissions['unpublish editable content'] = array(
    'name' => 'unpublish editable content',
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

  // Exported permission: 'update payments'.
  $permissions['update payments'] = array(
    'name' => 'update payments',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_payment',
  );

  // Exported permission: 'use PHP for tracking visibility'.
  $permissions['use PHP for tracking visibility'] = array(
    'name' => 'use PHP for tracking visibility',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'googleanalytics',
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

  // Exported permission: 'use ctools import'.
  $permissions['use ctools import'] = array(
    'name' => 'use ctools import',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'ctools',
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

  // Exported permission: 'view any atom'.
  $permissions['view any atom'] = array(
    'name' => 'view any atom',
    'roles' => array(
      'admin' => 'admin',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'view any commerce_customer_profile entity'.
  $permissions['view any commerce_customer_profile entity'] = array(
    'name' => 'view any commerce_customer_profile entity',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'view any commerce_customer_profile entity of bundle billing'.
  $permissions['view any commerce_customer_profile entity of bundle billing'] = array(
    'name' => 'view any commerce_customer_profile entity of bundle billing',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'view any commerce_order entity'.
  $permissions['view any commerce_order entity'] = array(
    'name' => 'view any commerce_order entity',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'view any commerce_order entity of bundle commerce_order'.
  $permissions['view any commerce_order entity of bundle commerce_order'] = array(
    'name' => 'view any commerce_order entity of bundle commerce_order',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'view any commerce_product entity'.
  $permissions['view any commerce_product entity'] = array(
    'name' => 'view any commerce_product entity',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'view any commerce_product entity of bundle individual_membership'.
  $permissions['view any commerce_product entity of bundle individual_membership'] = array(
    'name' => 'view any commerce_product entity of bundle individual_membership',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'view any commerce_product entity of bundle product'.
  $permissions['view any commerce_product entity of bundle product'] = array(
    'name' => 'view any commerce_product entity of bundle product',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'view any unpublished annuaire content'.
  $permissions['view any unpublished annuaire content'] = array(
    'name' => 'view any unpublished annuaire content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished content'.
  $permissions['view any unpublished content'] = array(
    'name' => 'view any unpublished content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished doc content'.
  $permissions['view any unpublished doc content'] = array(
    'name' => 'view any unpublished doc content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished forum content'.
  $permissions['view any unpublished forum content'] = array(
    'name' => 'view any unpublished forum content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished offre content'.
  $permissions['view any unpublished offre content'] = array(
    'name' => 'view any unpublished offre content',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished page content'.
  $permissions['view any unpublished page content'] = array(
    'name' => 'view any unpublished page content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished planete content'.
  $permissions['view any unpublished planete content'] = array(
    'name' => 'view any unpublished planete content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished planete_feed_source content'.
  $permissions['view any unpublished planete_feed_source content'] = array(
    'name' => 'view any unpublished planete_feed_source content',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'view_unpublished',
  );

  // Exported permission: 'view any unpublished story content'.
  $permissions['view any unpublished story content'] = array(
    'name' => 'view any unpublished story content',
    'roles' => array(
      'admin' => 'admin',
    ),
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

  // Exported permission: 'view field_message_moderation'.
  $permissions['view field_message_moderation'] = array(
    'name' => 'view field_message_moderation',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
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

  // Exported permission: 'view own atom'.
  $permissions['view own atom'] = array(
    'name' => 'view own atom',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'scald',
  );

  // Exported permission: 'view own commerce_customer_profile entities'.
  $permissions['view own commerce_customer_profile entities'] = array(
    'name' => 'view own commerce_customer_profile entities',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'view own commerce_customer_profile entities of bundle billing'.
  $permissions['view own commerce_customer_profile entities of bundle billing'] = array(
    'name' => 'view own commerce_customer_profile entities of bundle billing',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_customer',
  );

  // Exported permission: 'view own commerce_order entities'.
  $permissions['view own commerce_order entities'] = array(
    'name' => 'view own commerce_order entities',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'view own commerce_order entities of bundle commerce_order'.
  $permissions['view own commerce_order entities of bundle commerce_order'] = array(
    'name' => 'view own commerce_order entities of bundle commerce_order',
    'roles' => array(
      'admin' => 'admin',
      'authenticated user' => 'authenticated user',
      'bureau' => 'bureau',
    ),
    'module' => 'commerce_order',
  );

  // Exported permission: 'view own commerce_product entities'.
  $permissions['view own commerce_product entities'] = array(
    'name' => 'view own commerce_product entities',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'view own commerce_product entities of bundle individual_membership'.
  $permissions['view own commerce_product entities of bundle individual_membership'] = array(
    'name' => 'view own commerce_product entities of bundle individual_membership',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
  );

  // Exported permission: 'view own commerce_product entities of bundle product'.
  $permissions['view own commerce_product entities of bundle product'] = array(
    'name' => 'view own commerce_product entities of bundle product',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_product',
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
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_message_moderation'.
  $permissions['view own field_message_moderation'] = array(
    'name' => 'view own field_message_moderation',
    'roles' => array(
      'admin' => 'admin',
      'modérateur emploi' => 'modérateur emploi',
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

  // Exported permission: 'view own licenses'.
  $permissions['view own licenses'] = array(
    'name' => 'view own licenses',
    'roles' => array(
      'adhérent' => 'adhérent',
      'admin' => 'admin',
    ),
    'module' => 'commerce_license',
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

  // Exported permission: 'view payments'.
  $permissions['view payments'] = array(
    'name' => 'view payments',
    'roles' => array(
      'admin' => 'admin',
    ),
    'module' => 'commerce_payment',
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
