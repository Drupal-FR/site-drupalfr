<?php
/**
 * @file
 * drupalfr_planete.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_planete_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function drupalfr_planete_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
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
  drupal_alter('node_info', $items);
  return $items;
}
