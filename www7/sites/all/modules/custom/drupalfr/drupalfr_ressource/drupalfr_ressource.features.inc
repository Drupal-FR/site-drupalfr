<?php
/**
 * @file
 * drupalfr_ressource.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_ressource_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function drupalfr_ressource_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function drupalfr_ressource_node_info() {
  $items = array(
    'ressource' => array(
      'name' => t('Ressource'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
