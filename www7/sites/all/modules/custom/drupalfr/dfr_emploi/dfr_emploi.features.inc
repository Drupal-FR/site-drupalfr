<?php
/**
 * @file
 * dfr_emploi.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dfr_emploi_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function dfr_emploi_node_info() {
  $items = array(
    'offre' => array(
      'name' => t('Offre d\'emploi'),
      'base' => 'node_content',
      'description' => t('Une offre d\'emploi ou un stage Drupal pour les francophones.'),
      'has_title' => '1',
      'title_label' => t('Intitulé'),
      'help' => t('Totalement facultatif'),
    ),
  );
  return $items;
}
