<?php

/**
 * @file
 * Rules integration for Commerce Cart Expiration.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_cart_expiration_rules_action_info() {
  $actions = array();

  $actions['commerce_cart_expiration_delete_orders'] = array(
    'label' => t('Remove all expired cart orders'),
    'parameter' => array(
      'interval' => array(
        'type' => 'duration',
        'label' => t('Cart expiration interval'),
        'description' => t('Time span until shopping carts are considered expired.'),
      ),
      'limit' => array(
        'type' => 'integer',
        'label' => t('Number of shopping carts to process'),
        'description' => t('Number of shopping carts to process at a time. Useful for sites having a considerable amount of orders. Set to 0 for unlimited.'),
      ),
    ),
    'group' => t('Commerce Cart'),
    'callbacks' => array(
      'execute' => 'commerce_cart_expiration_delete_orders',
    ),
  );

  return $actions;
}

/**
 * Implements hook_rules_event_info().
 */
function commerce_cart_expiration_rules_event_info() {
  $events = array();

  $events['commerce_cart_expiration_cron'] = array(
    'label' => t('Commerce Cart Expiration cron is executed'),
    'group' => t('Commerce Cart'),
    'access callback' => 'commerce_order_rules_access',
  );

  $events['commerce_cart_expiration_delete_order'] = array(
    'label' => t('Before deleting an expired cart'),
    'group' => t('Commerce Cart'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Expired cart', array(), array('context' => 'a drupal commerce order')),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Rules action: deletes expired cart orders based on the provided interval and limit.
 */
function commerce_cart_expiration_delete_orders($interval = 0, $limit = 0) {
  if ($interval > 0) {
    $expired_order_ids = commerce_cart_expiration_get_expired_carts($interval, $limit);
    if (!empty($expired_order_ids)) {
      $expired_orders = commerce_order_load_multiple($expired_order_ids);
      foreach ($expired_orders as $order) {
        // Invoke a Rules event for deleting an expired cart order.
        rules_invoke_all('commerce_cart_expiration_delete_order', $order);
      }
      commerce_order_delete_multiple($expired_order_ids);
    }
  }
}

/**
 * @}
 */
