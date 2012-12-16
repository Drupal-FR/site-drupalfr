<?php
/**
 * @file
 * dfr_documentation.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dfr_documentation_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function dfr_documentation_node_info() {
  $items = array(
    'doc' => array(
      'name' => t('Documentation'),
      'base' => 'node_content',
      'description' => t('Une nouvelle page de documentation. Cette page pourra être placée dans un livre par un des éditeurs du site.'),
      'has_title' => '1',
      'title_label' => t('Titre'),
      'help' => '',
    ),
  );
  return $items;
}
