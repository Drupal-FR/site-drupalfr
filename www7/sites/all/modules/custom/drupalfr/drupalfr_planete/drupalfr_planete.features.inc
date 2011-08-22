<?php
/**
 * @file
 * drupalfr_planete.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function drupalfr_planete_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_views_api().
 */
function drupalfr_planete_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => 3.0);
  }
}

/**
 * Implementation of hook_node_info().
 */
function drupalfr_planete_node_info() {
  $items = array(
    'blog_planete' => array(
      'name' => t('Blog planète'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Nom du blog'),
      'help' => '',
    ),
    'blog_post' => array(
      'name' => t('Billet de blog'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Titre de l\'article'),
      'help' => '',
    ),
  );
  return $items;
}
