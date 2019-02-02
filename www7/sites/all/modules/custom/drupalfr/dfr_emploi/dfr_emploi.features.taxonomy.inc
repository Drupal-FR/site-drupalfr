<?php

/**
 * @file
 * dfr_emploi.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dfr_emploi_taxonomy_default_vocabularies() {
  return array(
    'nature_poste' => array(
      'name' => 'Nature du poste',
      'machine_name' => 'nature_poste',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'type_contrat' => array(
      'name' => 'Type de contrat',
      'machine_name' => 'type_contrat',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
