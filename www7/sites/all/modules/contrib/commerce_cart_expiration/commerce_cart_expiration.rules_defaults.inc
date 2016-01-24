<?php

/**
 * @file
 * Default rule configurations for Commerce Cart Expiration.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_cart_expiration_default_rules_configuration() {
  $rules = array();

  // Add a reaction rule to delete expired carts.
  $rule = rules_reaction_rule();

  $rule->label = t('Delete expired carts');
  $rule->active = TRUE;

  $rule
    ->event('commerce_cart_expiration_cron')
    ->action('commerce_cart_expiration_delete_orders', array(
      'interval' => 172800,
      'limit' => 100,
    ));

  $rules['commerce_cart_expiration_delete_expired_carts'] = $rule;

  // Add a reaction rule to delete expired carts.
  $reset_rule = rules_reaction_rule();

  $reset_rule->label = t('Reset status of expired carts');
  $reset_rule->active = FALSE;

  $reset_rule
    ->event('commerce_cart_expiration_cron')
    ->action('commerce_cart_expiration_reset_orders', array(
      'interval' => 172800,
      'limit' => 100,
      'status' => 'cart',
    ));
  $rules['commerce_cart_expiration_reset_expired_carts'] = $reset_rule;

  return $rules;
}
