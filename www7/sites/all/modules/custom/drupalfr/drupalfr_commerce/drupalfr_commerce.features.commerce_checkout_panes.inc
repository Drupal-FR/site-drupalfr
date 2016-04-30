<?php
/**
 * @file
 * drupalfr_commerce.features.commerce_checkout_panes.inc
 */

/**
 * Implements hook_commerce_checkout_panes_default().
 */
function drupalfr_commerce_commerce_checkout_panes_default() {
  $panes = array();

  $panes['account'] = array(
    'page' => 'checkout',
    'weight' => -5,
    'pane_id' => 'account',
    'fieldset' => TRUE,
    'collapsible' => FALSE,
    'collapsed' => FALSE,
    'enabled' => TRUE,
    'review' => TRUE,
  );

  $panes['cart_contents'] = array(
    'page' => 'checkout',
    'weight' => -10,
    'pane_id' => 'cart_contents',
    'fieldset' => TRUE,
    'collapsible' => FALSE,
    'collapsed' => FALSE,
    'enabled' => TRUE,
    'review' => TRUE,
  );

  $panes['checkout_completion_message'] = array(
    'page' => 'complete',
    'fieldset' => 0,
    'pane_id' => 'checkout_completion_message',
    'collapsible' => 0,
    'collapsed' => 0,
    'weight' => 0,
    'enabled' => 1,
    'review' => 1,
  );

  $panes['checkout_review'] = array(
    'page' => 'review',
    'fieldset' => FALSE,
    'pane_id' => 'checkout_review',
    'collapsible' => FALSE,
    'collapsed' => FALSE,
    'weight' => 0,
    'enabled' => TRUE,
    'review' => TRUE,
  );

  $panes['commerce_license'] = array(
    'page' => 'checkout',
    'fieldset' => TRUE,
    'enabled' => FALSE,
    'pane_id' => 'commerce_license',
    'collapsible' => FALSE,
    'collapsed' => FALSE,
    'weight' => 0,
    'review' => TRUE,
  );

  $panes['commerce_license_complete'] = array(
    'page' => 'complete',
    'fieldset' => TRUE,
    'enabled' => FALSE,
    'pane_id' => 'commerce_license_complete',
    'collapsible' => FALSE,
    'collapsed' => FALSE,
    'weight' => 0,
    'review' => TRUE,
  );

  $panes['commerce_payment'] = array(
    'page' => 'review',
    'weight' => 10,
    'pane_id' => 'commerce_payment',
    'fieldset' => TRUE,
    'collapsible' => FALSE,
    'collapsed' => FALSE,
    'enabled' => TRUE,
    'review' => TRUE,
  );

  $panes['commerce_payment_redirect'] = array(
    'page' => 'payment',
    'pane_id' => 'commerce_payment_redirect',
    'fieldset' => TRUE,
    'collapsible' => FALSE,
    'collapsed' => FALSE,
    'weight' => 0,
    'enabled' => TRUE,
    'review' => TRUE,
  );

  $panes['customer_profile_billing'] = array(
    'page' => 'checkout',
    'weight' => 5,
    'pane_id' => 'customer_profile_billing',
    'fieldset' => 1,
    'collapsible' => 0,
    'collapsed' => 0,
    'enabled' => 1,
    'review' => 1,
  );

  return $panes;
}
