<?php

/**
 * @file
 * dfr_emploi.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dfr_emploi_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function dfr_emploi_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function dfr_emploi_node_info() {
  $items = array(
    'offre' => array(
      'name' => t('Offre d\'emploi'),
      'base' => 'node_content',
      'description' => t('Offre d\'emploi ou de stage pour les sociétés.'),
      'has_title' => '1',
      'title_label' => t('Intitulé de l\'offre'),
      'help' => t('Décrivez votre offre d\'emploi afin qu\'elle soit publiée sur Drupalfr.org.
<br />
<strong>Les offres sont systématiquement relues avant publication, cela peut entraîner un délai de quelques jours avant que votre offre ne soit publiée.</strong>
Si après une semaine votre offre n\'est pas en ligne, merci de nous le signaler via le formulaire de contact.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
