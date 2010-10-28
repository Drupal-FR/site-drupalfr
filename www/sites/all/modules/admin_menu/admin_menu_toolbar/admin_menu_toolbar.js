/* $Id: admin_menu_toolbar.js,v 1.1.2.3 2009/09/26 06:25:47 davereid Exp $ */
(function($) {

Drupal.admin = Drupal.admin || {};
Drupal.admin.behaviors = Drupal.admin.behaviors || {};

/**
 * @ingroup admin_behaviors
 * @{
 */

/**
 * Add neat shadow.
 */
Drupal.admin.behaviors.toolbarShadow = function (context, settings, $adminMenu) {
  $('#admin-menu-wrapper', $adminMenu).append('<div id="admin-menu-shadow"></div>');
};

/**
 * Apply active trail highlighting based on current path.
 *
 * @todo Not limited to toolbar; move into core?
 */
Drupal.admin.behaviors.toolbarActiveTrail = function (context, settings, $adminMenu) {
  if (settings.admin_menu.toolbar && settings.admin_menu.toolbar.activeTrail) {
    $adminMenu.find('div > ul > li > a[href=' + settings.admin_menu.toolbar.activeTrail + ']').addClass('active-trail');
  }
};

/**
 * @} End of "defgroup admin_behaviors".
 */

})(jQuery);
