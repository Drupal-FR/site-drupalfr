Description
===========

This module provides a time-based cart expiration feature.


Requirements
============

- Drupal Commerce (http://drupal.org/project/commerce): 7.x-1.0 or newer
- Rules (http://drupal.org/project/rules): 7.x-2.0 or newer
- Token (http://drupal.org/project/token): 7.x-1.0 or newer


Installation / Configuration
============================

The module provides a default rule that can be configured from the Rules
administration screen, located at: admin/config/workflow/rules
(rules_admin must be enabled)

If you want to change the default rule configuration, you need to implement the
hook_default_rules_configuration_alter() and override the current configuration.

Example:

/**
 * Implements hook_default_rules_configuration_alter().
 */
function HOOK_default_rules_configuration_alter(&$configs) {
  if (isset($configs['commerce_cart_expiration_delete_expired_carts'])) {
    foreach ($configs['commerce_cart_expiration_delete_expired_carts']->actions() as $action) {
      if ($action->getElementName() == 'commerce_cart_expiration_delete_orders') {
        $action->settings['interval'] = 3600;
      }
    }
  }
}


The module also provides a block which displays the time remaining. Its
content gets refreshed by javascript just like a countdown.
To show up the block, add the 'Cart expiration' block to your prefered
region under admin/structure/block.
You can configure this blocks content on the block configuration page. Use the
token [commerce-order:expiration-formatted] to get a format that can be
manipulated by javascript.

If the user has the permission 'Ajax order expiration', javascript will delete
the cart automatically as soon as the cart expired. This makes cart expiration
independent from cron if the customer has javascript enabled.
After a successful ajax deletion the customer gets redirected to 'cart/expired'.
You can set the content of that page at: admin/commerce/config/commerce_cart_expiration


There is also a screencast available which shows you how to use this module:
http://commerceguys.com/blog/commerce-module-tuesday-commerce-cart-expiration

Drush integration
=================

This module provides a Drush (http://drupal.org/project/drush) command for
deleting expired cart orders.
Use 'drush help commerce-cart-expiration-clean-orders' for more information.


Maintainers
===========

Developed and maintained by Andrei Mateescu (amateescu) - http://drupal.org/user/729614


Credits
=======

jgalletta - http://drupal.org/user/1187850
Pedro Cambra (pcambra) - http://drupal.org/user/122101
