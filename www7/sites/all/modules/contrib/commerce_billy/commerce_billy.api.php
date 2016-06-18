<?php

/**
 * @file
 * Commerce billy API Documentation.
 */

/**
 * One can change list of commerce order bundles.
 *
 * One can change list of commerce order bundles to which invoice fields will be
 * added. Maybe you want to add this fields only for some commerce order
 * bundles.
 *
 * @param array $order_types
 *   By reference. The order types to which invoice fields will be added.
 */
function hook_commerce_billy_order_types_alter(&$order_types) {

  // For example, invoice fields will be added only to 'invoice' bundle.
  $order_types = array(
    'invoice',
  );
}
