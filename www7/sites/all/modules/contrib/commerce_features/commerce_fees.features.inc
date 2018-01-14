<?php

/**
 * @file
 * Features integration on behalf of Commerce Fees.
 */

/**
 * Implements hook_features_export().
 */
function commerce_fees_type_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  // Add required dependencies.
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['commerce_fees'] = 'commerce_features';

  foreach ($data as $type) {
    $export['features']['commerce_fees_type'][$type] = $type;

    $fields = field_info_instances('commerce_fees', $type);
    foreach ($fields as $field) {
      $pipe['field'][] = "commerce_fees-{$field['bundle']}-{$field['field_name']}";
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function commerce_fees_type_features_export_options() {
  $feature_types = array();
  $fees_types = commerce_fees_types();
  if (!empty($fees_types)) {
    foreach ($fees_types as $key => $type) {
      $feature_types[$key] = $type['display_title'];
    }
  }
  return $feature_types;
}

/**
 * Implements hook_features_export_render().
 */
function commerce_fees_type_features_export_render($module, $data, $export = NULL) {
  $info = commerce_fees_types();
  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $type) {
    if (isset($info[$type]) && $fees_type = $info[$type]) {
      $output[] = "    '{$type}' => " . features_var_export($fees_type, '    ') . ",";
    }
  }
  $output[] = '  );';
  $output[] = '  return $items;';
  $output = implode("\n", $output);
  return array('commerce_fees_default_types' => $output);
}

/**
 * Implements hook_features_revert().
 */
function commerce_fees_type_features_revert($module = NULL) {
  // Get default fees types
  if (module_hook($module, 'commerce_fees_default_types')) {
    $default_types = module_invoke($module, 'commerce_fees_default_types');
    // Read the fees types from the database instead of calling all
    $existing_types = commerce_fees_types();
    foreach ($default_types as $key => $type) {
      // Add or update.
      if (!isset($existing_types[$key])) {
        $type['is_new'] = TRUE;
      }
      commerce_fees_save($type, TRUE, TRUE);
    }
  }
  else {
    drupal_set_message(t('Could not load default fees types.'), 'error');
    return FALSE;
  }

  // Reset the static cache.
  commerce_fees_reset();
  // Schedule a menu rebuild.
  variable_set('menu_rebuild_needed', TRUE);

  return TRUE;
}

/**
 * Implements hook_features_rebuild().
 */
function commerce_fees_type_features_rebuild($module) {
  return commerce_fees_type_features_revert($module);
}
