<?php
// $Id: admin_menu.inc,v 1.11.2.20.2.30 2010/03/11 03:28:47 sun Exp $

/**
 * @file
 * Menu builder functions for Administration menu.
 */

/**
 * Build the administration menu as renderable menu links.
 *
 * @param $tree
 *   A data structure representing the administration menu tree as returned from
 *   menu_tree_all_data().
 *
 * @return
 *   The complete administration menu, suitable for theme_admin_menu_links().
 *
 * @see theme_admin_menu_links()
 * @see admin_menu_menu_alter()
 */
function admin_menu_links_menu($tree) {
  $links = array();
  foreach ($tree as $data) {
    // Skip menu callbacks (mostly dynamic items).
    if (isset($data['link']['type']) && $data['link']['type'] == MENU_CALLBACK) {
      continue;
    }
    // Hide 'Administer' and make child links appear on this level.
    // @todo Make this configurable.
    if ($data['link']['router_path'] == 'admin') {
      if ($data['below']) {
        $links = array_merge($links, admin_menu_links_menu($data['below']));
      }
      continue;
    }
    // Omit alias lookups.
    $data['link']['localized_options']['alias'] = TRUE;
    // Remove description to prevent mouseover tooltip clashes.
    unset($data['link']['localized_options']['attributes']['title']);

    $links[$data['link']['mlid']] = array(
      '#title' => $data['link']['title'],
      '#href' => $data['link']['href'],
      '#options' => $data['link']['localized_options'],
      '#weight' => $data['link']['weight'],
    );
    if ($data['below']) {
      $links[$data['link']['mlid']] += admin_menu_links_menu($data['below']);
    }
  }
  return $links;
}

/**
 * Build icon menu links; mostly containing maintenance helpers.
 *
 * @see theme_admin_menu_links()
 */
function admin_menu_links_icon() {
  $destination = drupal_get_destination();

  $links = array(
    '#theme' => 'admin_menu_links',
    '#weight' => -100,
  );
  $links['icon'] = array(
    '#title' => theme('admin_menu_icon'),
    '#attributes' => array('class' => array('admin-menu-icon')),
    '#href' => '<front>',
    '#options' => array(
      'html' => TRUE,
    ),
  );
  // Add link to manually run cron.
  $links['icon']['cron'] = array(
    '#title' => t('Run cron'),
    '#weight' => 50,
    '#access' => user_access('administer site configuration'),
    '#href' => 'admin/reports/status/run-cron',
  );
  // Add link to run update.php.
  $links['icon']['update'] = array(
    '#title' => t('Run updates'),
    '#weight' => 50,
    '#access' => ($GLOBALS['user']->uid == 1 || !empty($GLOBALS['update_free_access'])),
    '#href' => base_path() . 'update.php',
    '#options' => array(
      'external' => TRUE,
    ),
  );
  // Add link to drupal.org.
  $links['icon']['drupal.org'] = array(
    '#title' => 'Drupal.org',
    '#weight' => 100,
    '#access' => user_access('display drupal links'),
    '#href' => 'http://drupal.org',
  );

  // Add links to project issue queues.
  // Set defaults for module info, in case hook_system_info_alter() is invoked.
  $defaults = array(
    'dependencies' => array(),
    'dependents' => array(),
    'description' => '',
    'version' => NULL,
    'php' => DRUPAL_MINIMUM_PHP,
  );
  $cvs_deploy_exists = module_exists('cvs_deploy');
  foreach (module_list(FALSE, FALSE, TRUE) as $module) {
    $path = drupal_get_path('module', $module);
    $info = drupal_parse_info_file($path . '/' . $module . '.info');
    // Invoke hook_system_info_alter() for CVS Deploy module.
    if ($cvs_deploy_exists) {
      // @see file_scan_directory()
      $file = new stdClass;
      $file->filename = $path . '/' . $module . '.module';
      $file->basename = basename($file->filename);
      $file->name = $module;
      // @see module_rebuild_cache()
      $info += $defaults;
      drupal_alter('system_info', $info, $file);
    }
    if (empty($info['project']) || isset($links['icon']['drupal.org'][$info['project']])) {
      continue;
    }
    $links['icon']['drupal.org'][$info['project']] = array(
      '#title' => t('@project issue queue', array('@project' => $info['name'])),
      '#weight' => ($info['project'] == 'drupal' ? -10 : 0),
      '#href' => 'http://drupal.org/project/issues/' . $info['project'],
      '#options' => array(
        'query' => 'version=' . (isset($info['core']) ? $info['core'] : 'All'),
      ),
    );
  }

  // Add items to flush caches.
  $links['icon']['flush-cache'] = array(
    '#title' => t('Flush all caches'),
    '#weight' => 20,
    // @todo Add permission to flush cashes.
    '#access' => user_access('administer site configuration'),
    '#href' => 'admin_menu/flush-cache',
    '#options' => array(
      'query' => $destination,
    ),
  );
  $caches = array(
    'admin_menu' => t('Administration menu'),
    'cache' => t('Cache tables'),
    'menu' => t('Menu'),
    'requisites' => t('Page requisites'),
    'theme' => t('Theme registry'),
  );
  foreach ($caches as $arg => $title) {
    $links['icon']['flush-cache'][$arg] = array(
      '#title' => $title,
      '#href' => 'admin_menu/flush-cache/' . $arg,
      '#options' => array(
        'query' => $destination,
      ),
    );
  }
  // Add link to toggle developer modules (performance).
  $saved_state = variable_get('admin_menu_devel_modules_enabled', NULL);
  $links['icon']['toggle-modules'] = array(
    '#title' => isset($saved_state) ? t('Enable developer modules') : t('Disable developer modules'),
    '#weight' => 88,
    '#access' => user_access('administer site configuration'),
    '#href' => 'admin_menu/toggle-modules',
    '#options' => array(
      'query' => $destination,
    ),
  );

  // Add Devel module links.
  if (module_exists('devel')) {
    // Add variable editor.
    $links['icon']['devel-variables'] = array(
      '#title' => t('Variable editor'),
      '#weight' => 20,
      '#access' => user_access('access devel information'),
      '#href' => 'devel/variable',
    );
  }

  return $links;
}

/**
 * Build user/action links; mostly account information and links.
 *
 * @see theme_admin_menu_links()
 */
function admin_menu_links_user() {
  $links = array(
    '#theme' => 'admin_menu_links',
    '#weight' => 100,
  );
  // Add link to show current authenticated/anonymous users.
  $links['user-counter'] = array(
    '#title' => admin_menu_get_user_count(),
    '#description' => t('Current anonymous / authenticated users'),
    '#weight' => -90,
    '#attributes' => array('class' => array('admin-menu-action', 'admin-menu-users')),
    '#href' => (user_access('administer users') ? 'admin/user/user' : 'user'),
  );
  $links['account'] = array(
    '#title' => $GLOBALS['user']->name,
    '#weight' => -99,
    '#attributes' => array('class' => array('admin-menu-action', 'admin-menu-account')),
    '#href' => 'user/' . $GLOBALS['user']->uid,
  );
  $links['logout'] = array(
    '#title' => t('Log out'),
    '#weight' => -100,
    '#attributes' => array('class' => array('admin-menu-action')),
    '#href' => 'logout',
  );

  // Add Devel module switch user links.
  $switch_links = module_invoke('devel', 'switch_user_list');
  if (!empty($switch_links) && count($switch_links) > 1) {
    foreach ($switch_links as $uid => $link) {
      $links['account'][$link['title']] = array(
        '#title' => $link['title'],
        '#description' => $link['attributes']['title'],
        '#href' => $link['href'],
        '#options' => array(
          'query' => $link['query'],
          'html' => !empty($link['html']),
        ),
      );
    }
  }

  return $links;
}

/**
 * Form builder function for module settings.
 */
function admin_menu_theme_settings() {
  $form['admin_menu_margin_top'] = array(
    '#type' => 'checkbox',
    '#title' => t('Adjust top margin'),
    '#default_value' => variable_get('admin_menu_margin_top', 1),
    '#description' => t('If enabled, the site output is shifted down approximately 20 pixels from the top of the viewport to display the administration menu. If disabled, some absolute- or fixed-positioned page elements may be covered by the administration menu.'),
  );
  $form['admin_menu_position_fixed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep menu at top of page'),
    '#default_value' => variable_get('admin_menu_position_fixed', 0),
    '#description' => t('If enabled, the administration menu is always displayed at the top of the browser viewport (even after the page is scrolled). <strong>Note: In some browsers, this setting results in a malformed page, an invisible cursor, non-selectable elements in forms, or other issues. Disable this option if these issues occur.</strong>'),
  );
  $form['tweaks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
  );
  $form['tweaks']['admin_menu_tweak_modules'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapse fieldsets on modules page'),
    '#default_value' => variable_get('admin_menu_tweak_modules', 0),
    '#description' => t('If enabled, fieldsets on the <a href="!modules-url">modules</a> page are automatically collapsed when loading the page.', array('!modules-url' => url('admin/build/modules'))),
  );
  if (module_exists('util')) {
    $form['tweaks']['admin_menu_tweak_modules']['#description'] .= '<br /><strong>' . t('If the Utility module was installed for this purpose, it can be safely disabled and uninstalled.') . '</strong>';
  }
  $form['tweaks']['admin_menu_tweak_permissions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapse modules on permissions page'),
    '#default_value' => variable_get('admin_menu_tweak_permissions', 0),
    '#description' => t('Collapses permissions by module on the <a href="@permissions-url">permissions</a> page.', array('@permissions-url' => url('admin/user/permissions'))),
  );
  $form['tweaks']['admin_menu_tweak_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Move local tasks into menu'),
    '#default_value' => variable_get('admin_menu_tweak_tabs', 0),
    '#description' => t('If enabled, the tabs on the current page are moved into the administration menu. This feature is only available in themes that use the CSS classes <code>tabs primary</code> and <code>tabs secondary</code> for tabs.'),
  );

  // Fetch all available modules manually, since module_list() only returns
  // currently enabled modules, which makes this setting pointless if developer
  // modules are currently disabled.
  $all_modules = array();
  $result = db_query("SELECT name, filename FROM {system} WHERE type = 'module' ORDER BY name ASC");
  while ($module = db_fetch_object($result)) {
    if (file_exists($module->filename)) {
      $all_modules[$module->name] = $module->name;
    }
  }
  $devel_modules = variable_get('admin_menu_devel_modules', _admin_menu_developer_modules());
  $devel_modules = array_intersect($all_modules, $devel_modules);
  $form['tweaks']['admin_menu_devel_modules_skip'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Developer modules to keep enabled'),
    '#default_value' => variable_get('admin_menu_devel_modules_skip', array()),
    '#options' => $devel_modules,
    '#access' => !empty($devel_modules),
    '#description' => t('Enabled developer modules in this list will not be disabled when the link <em>Disable developer modules</em> in the menu below the icon is invoked.'),
  );

  return system_settings_form($form);
}

/**
 * Implementation of hook_form_FORM_ID_alter().
 *
 * Extends Devel module with Administration menu developer settings.
 */
function _admin_menu_form_devel_admin_settings_alter(&$form, $form_state) {
  // Shift system_settings_form buttons.
  $weight = isset($form['buttons']['#weight']) ? $form['buttons']['#weight'] : 0;
  $form['buttons']['#weight'] = $weight + 1;

  $form['admin_menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Administration menu settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $display_options = array('mid', 'weight', 'pid');
  $display_options = array(0 => t('None'), 'mlid' => t('Menu link ID'), 'weight' => t('Weight'), 'plid' => t('Parent link ID'));
  $form['admin_menu']['admin_menu_display'] = array(
    '#type' => 'radios',
    '#title' => t('Display additional data for each menu item'),
    '#default_value' => variable_get('admin_menu_display', 0),
    '#options' => $display_options,
    '#description' => t('Display the selected items next to each menu item link.'),
  );
  $form['admin_menu']['admin_menu_show_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display all menu items'),
    '#default_value' => variable_get('admin_menu_show_all', 0),
    '#description' => t('If enabled, all menu items are displayed regardless of your site permissions. <em>Note: Do not enable on a production site.</em>'),
  );
}

/**
 * Menu callback; Enable/disable developer modules.
 *
 * This can save up to 150ms on each uncached page request.
 */
function admin_menu_toggle_modules() {
  $rebuild = FALSE;
  $saved_state = variable_get('admin_menu_devel_modules_enabled', NULL);
  if (isset($saved_state)) {
    // Re-enable modules that were enabled before.
    module_enable($saved_state);
    variable_del('admin_menu_devel_modules_enabled');
    drupal_set_message(t('Enabled these modules: !module-list.', array('!module-list' => implode(', ', $saved_state))));
    $rebuild = TRUE;
  }
  else {
    // Allow site admins to override this variable via settings.php.
    $devel_modules = variable_get('admin_menu_devel_modules', _admin_menu_developer_modules());
    // Store currently enabled modules in a variable.
    $devel_modules = array_intersect(module_list(FALSE, FALSE), $devel_modules);
    $devel_modules = array_diff($devel_modules, variable_get('admin_menu_devel_modules_skip', array()));
    if (!empty($devel_modules)) {
      variable_set('admin_menu_devel_modules_enabled', $devel_modules);
      // Disable developer modules.
      module_disable($devel_modules);
      drupal_set_message(t('Disabled these modules: !module-list.', array('!module-list' => implode(', ', $devel_modules))));
      $rebuild = TRUE;
    }
    else {
      drupal_set_message(t('No developer modules are enabled.'));
    }
  }
  if ($rebuild) {
    // Make sure everything is rebuilt, basically a combination of the calls
    // from system_modules() and system_modules_submit().
    drupal_rebuild_theme_registry();
    menu_rebuild();
    cache_clear_all('schema', 'cache');
    cache_clear_all();
    drupal_clear_css_cache();
    drupal_clear_js_cache();
    // Synchronize to catch any actions that were added or removed.
    actions_synchronize();
    // Finally, flush admin_menu's cache.
    admin_menu_flush_caches();
  }
  drupal_goto(referer_uri());
}

/**
 * Helper function to return a default list of developer modules.
 */
function _admin_menu_developer_modules() {
  return array(
    'cache_disable',
    'coder',
    'content_copy',
    'debug',
    'delete_all',
    'demo',
    'devel',
    'devel_node_access',
    'devel_themer',
    'macro',
    'form_controller',
    'imagecache_ui',
    'journal',
    'rules_admin',
    'stringoverrides',
    'trace',
    'upgrade_status',
    'user_display_ui',
    'util',
    'views_ui',
    'views_theme_wizard',
  );
}

/**
 * Flush all caches or a specific one.
 *
 * @param $name
 *   (optional) Name of cache to flush.
 */
function admin_menu_flush_cache($name = NULL) {
  switch ($name) {
    case 'admin_menu':
      // Flush cached menu tree.
      db_query("DELETE FROM {menu_links} WHERE menu_name = 'admin_menu' AND customized = 0");
      // Fall-through.

    case 'menu':
      menu_rebuild();
      break;

    case 'cache':
      // Don't clear cache_form - in-progress form submissions may break.
      // Ordered so clearing the page cache will always be the last action.
      // @see drupal_flush_all_caches()
      $core = array('cache', 'cache_block', 'cache_filter', 'cache_page');
      $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
      foreach ($cache_tables as $table) {
        cache_clear_all('*', $table, TRUE);
      }
      break;

    case 'requisites':
      // Change query-strings on css/js files to enforce reload for all users.
      _drupal_flush_css_js();

      drupal_clear_css_cache();
      drupal_clear_js_cache();
      break;

    case 'theme':
      module_invoke('system', 'theme_data');
      drupal_rebuild_theme_registry();
      break;

    default:
      // Flush all caches; no need to re-implement this.
      module_load_include('inc', 'system', 'system.admin');
      $form = $form_state = array();
      system_clear_cache_submit($form, $form_state);
      break;
  }
  drupal_goto();
}

/**
 * Render an icon to display in the administration menu.
 *
 * @ingroup themeable
 */
function theme_admin_menu_icon() {
  return '<img class="admin-menu-icon" src="' . (theme_get_setting('toggle_favicon') ? theme_get_setting('favicon') : base_path() . 'misc/favicon.ico') . '" width="16" height="16" alt="' . t('Home') . '" />';
}

