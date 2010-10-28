<?php

/**
 * Implementation of hook_content_default_fields().
 */
function drupalfr_user_content_default_fields() {
  module_load_include('inc', 'drupalfr_user', 'drupalfr_user.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalfr_user_content_default_fields', $args);
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function drupalfr_user_imagecache_default_presets() {
  module_load_include('inc', 'drupalfr_user', 'drupalfr_user.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalfr_user_imagecache_default_presets', $args);
}

/**
 * Implementation of hook_node_info().
 */
function drupalfr_user_node_info() {
  module_load_include('inc', 'drupalfr_user', 'drupalfr_user.features.node');
  $args = func_get_args();
  return call_user_func_array('_drupalfr_user_node_info', $args);
}

/**
 * Implementation of hook_user_default_permissions().
 */
function drupalfr_user_user_default_permissions() {
  module_load_include('inc', 'drupalfr_user', 'drupalfr_user.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalfr_user_user_default_permissions', $args);
}
