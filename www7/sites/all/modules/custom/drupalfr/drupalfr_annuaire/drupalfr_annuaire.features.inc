<?php
/**
 * @file
 * drupalfr_annuaire.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_annuaire_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function drupalfr_annuaire_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function drupalfr_annuaire_node_info() {
  $items = array(
    'annuaire' => array(
      'name' => t('Société'),
      'base' => 'node_content',
      'description' => t('Une fiche vous permettant de vous présenter, vous ou votre société. Cette fiche doit être renseignée avec soin, elle sera votre vitrine pour la communauté des utilisateurs Drupal en France.'),
      'has_title' => '1',
      'title_label' => t('Raison sociale'),
      'help' => t('Votre fiche sera immédiatement publiée dans l\'annuaire, elle doit être renseignée avec soin, elle sera votre vitrine pour la communauté des utilisateurs Drupal en France.

Vous êtes responsable de la véracité des informations qu\'elle contient. L\'équipe du site se réserve le droit de retirer toute fiche mensongère qui porterait atteinte à la qualité de ce service d\'annuaire.
 
En cas de problème, contactez-nous.
'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
