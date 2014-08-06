<?php
/**
 * @file
 * drupalfr_edito.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_edito_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
