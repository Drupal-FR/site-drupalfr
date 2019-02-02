<?php

/**
 * @file
 * drupalfr_commerce.features.inc
 */

/**
 * Implements hook_commerce_line_item_default_types().
 */
function drupalfr_commerce_commerce_line_item_default_types() {
  $items = array(
    'individual_membership' => array(
      'name' => 'Adhésion individuelle',
      'description' => 'A customizable product line item type.',
      'product' => TRUE,
      'add_form_submit_value' => 'Add product',
      'base' => 'commerce_product_line_item',
    ),
  );
  return $items;
}

/**
 * Implements hook_commerce_product_default_types().
 */
function drupalfr_commerce_commerce_product_default_types() {
  $items = array(
    'individual_membership' => array(
      'type' => 'individual_membership',
      'name' => 'Adhésion individuelle',
      'description' => '',
      'help' => '',
      'revision' => 1,
    ),
    'product' => array(
      'type' => 'product',
      'name' => 'Product',
      'description' => 'A basic product type.',
      'help' => '',
      'revision' => 1,
    ),
  );
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_commerce_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function drupalfr_commerce_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
