<?php

/**
 * @file
 * Documents hooks provided by the Commerce PayPal Checkout module.
 */

/**
 * Allows modules to alter the request body before the create order API call is
 * made to PayPal.
 *
 * @param array $request_body
 *   The request body.
 * @param $order
 *   The order.
 */
function hook_commerce_paypal_checkout_create_order_request_alter(&$request_body, $order) {
 // No example.
}

/**
 * Allows modules to alter the request body before the update order API call is
 * made to PayPal.
 *
 * @param array $request_body
 *   The request body.
 * @param $order
 *   The order.
 */
function hook_commerce_paypal_checkout_update_order_request_alter(&$request_body, $order) {
  // No example.
}
