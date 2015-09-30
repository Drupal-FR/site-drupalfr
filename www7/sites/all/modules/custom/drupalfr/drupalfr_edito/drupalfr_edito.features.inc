<?php
/**
 * @file
 * drupalfr_edito.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_edito_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
