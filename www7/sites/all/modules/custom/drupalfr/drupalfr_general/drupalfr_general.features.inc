<?php
/**
 * @file
 * drupalfr_general.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_general_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function drupalfr_general_views_api() {
  return array("version" => "3.0");
}
