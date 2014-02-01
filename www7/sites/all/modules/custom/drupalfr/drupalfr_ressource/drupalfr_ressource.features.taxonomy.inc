<?php
/**
 * @file
 * drupalfr_ressource.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function drupalfr_ressource_taxonomy_default_vocabularies() {
  return array(
    'licence' => array(
      'name' => 'Licence‎',
      'machine_name' => 'licence',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'niveau' => array(
      'name' => 'Niveau',
      'machine_name' => 'niveau',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'public_cible' => array(
      'name' => 'Public cible',
      'machine_name' => 'public_cible',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'ressource_type' => array(
      'name' => 'Type de ressource',
      'machine_name' => 'ressource_type',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'thematique' => array(
      'name' => 'Thématique',
      'machine_name' => 'thematique',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
