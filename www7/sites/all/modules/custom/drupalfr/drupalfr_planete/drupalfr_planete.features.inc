<?php
/**
 * @file
 * drupalfr_planete.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_planete_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function drupalfr_planete_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function drupalfr_planete_node_info() {
  $items = array(
    'planete' => array(
      'name' => t('Article du planète'),
      'base' => 'node_content',
      'description' => t('Article provenant d\'un blog d\'un utilisateur, agrégé dans le planète.'),
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
    'planete_feed_source' => array(
      'name' => t('Planete Feed Source'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
  );
  return $items;
}
