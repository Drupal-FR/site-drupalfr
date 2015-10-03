<?php

/**
 * @file
 * Drush integration for the Commerce cart expiration module.
 */

/**
 * Implements hook_drush_command().
 */
function commerce_cart_expiration_drush_command() {
  $items = array();

  $items['commerce-cart-expiration-clean-orders'] = array(
    'description' => dt('Deletes commerce orders that are in a cart state.'),
    'arguments' => array(
      'interval' =>  dt('Time span (in seconds) until shopping carts are considered expired. Defaults to the interval specified in the default Rule provided by this module.'),
    ),
    'options' => array(
      'limit' =>  dt("Number of orders to delete at a time. Set to 'default' to use the limit specified in the default Rule provided by this module."),
    ),
    'examples' => array(
      'drush cce-co' => "Delete all cart orders that weren't modified in the last time interval specified in the default Rule provided by this module.",
      'drush cce-co 3600' => "Delete all cart orders that weren't modified in the last hour.",
      'drush cce-co 3600 --limit=200' => "Delete 200 cart orders that weren't modified in the last hour.",
      'drush cce-co 3600 --limit=default' => "Delete X cart orders that weren't modified in the last hour when X is the value specified in the default Rule provided by this module.",
    ),
    'aliases' => array('cce-co'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function commerce_cart_expiration_drush_help($section) {
  switch ($section) {
    case 'commerce-cart-expiration-clean-orders':
      return dt("This command will delete all commerce orders that are in a cart state and their last change timestamp is higher than the provided argument.");
  }
}

/**
 * Deletes expired orders.
 *
 * @param int $interval
 *   Time span (in seconds) until shopping carts are considered expired.
 */
function drush_commerce_cart_expiration_clean_orders($interval = 0) {
  module_load_include('inc', 'commerce_cart_expiration', 'commerce_cart_expiration.rules');

  if ($interval === 0) {
    $interval = _commerce_cart_expiration_get_interval();

    // Bail out with a helpful message if there is no interval available.
    if ($interval === 0) {
      drush_log(dt('Unable to find a default order expiration delay, please provide one.'), 'error');
      return;
    }
  }

  $limit = drush_get_option('limit', 0);
  if ($limit === 'default') {
    // Get the limit configured in Rules, if any.
    $config = rules_config_load('commerce_cart_expiration_delete_expired_carts');
    foreach ($config->actions() as $action) {
      if ($action->getElementName() == 'commerce_cart_expiration_delete_orders') {
        $limit = $action->settings['limit'];
      }
    }

    // Bail out with a helpful message if there is no default limit available.
    if ($limit === 'default') {
      drush_log(dt('Unable to find a default number of orders to delete, please use another option.'), 'warning');
      return;
    }
  }

  // We have all the required information, ask for user confirmation.
  drush_print(dt('@limit orders in the cart state that were not changed in the last @interval will be deleted.', array('@limit' => ($limit === 0) ? 'All' : 'A maximum of ' . $limit, '@interval' => format_interval($interval))));
  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_user_abort();
  }

  // The user is now accountable for his decision, let's delete the orders.
  commerce_cart_expiration_delete_orders($interval, $limit);
}
