<?php
/**
 * @file
 * drupalfr_user.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_user_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}
