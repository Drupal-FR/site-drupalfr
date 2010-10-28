<?php
// $Id: admin_menu.api.php,v 1.1.2.4 2010/03/12 23:05:57 sun Exp $

/**
 * @file
 * API documentation for Administration menu.
 */

/**
 * Alter content in Administration menu bar before it is rendered.
 *
 * @param $content
 *   A structured array suitable for drupal_render(), at the very least
 *   containing the keys 'menu' and 'links'.  Most implementations likely want
 *   to alter or add to 'links'.
 *
 * $content['menu'] contains the HTML representation of the 'admin_menu' menu
 * tree.
 * @see admin_menu_menu_alter()
 *
 * $content['links'] contains additional top-level links in the Administration
 * menu, such as the icon menu or the logout link. You can add more items here
 * or play with the #weight attribute to customize them.
 * @see theme_admin_menu_links()
 * @see admin_menu_links_icon()
 * @see admin_menu_links_user()
 */
function hook_admin_menu_output_alter(&$content) {
  // Add new top-level item.
  $content['menu']['myitem'] = array(
    '#title' => t('My item'),
    // #attributes are used for list items (LI). Note the special syntax for
    // the 'class' attribute.
    '#attributes' => array('class' => array('mymodule-myitem')),
    '#href' => 'mymodule/path',
    // #options are passed to l(). Note that you can apply 'attributes' for
    // links (A) here.
    '#options' => array(
      'query' => drupal_get_destination(),
    ),
    // #weight controls the order of links in the resulting item list.
    '#weight' => 50,
  );
  // Add link to manually run cron.
  $content['menu']['myitem']['cron'] = array(
    '#title' => t('Run cron'),
    '#access' => user_access('administer site configuration'),
    '#href' => 'admin/reports/status/run-cron',
  );
}

