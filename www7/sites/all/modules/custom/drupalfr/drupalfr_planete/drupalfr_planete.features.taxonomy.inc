<?php
/**
 * @file
 * drupalfr_planete.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function drupalfr_planete_taxonomy_default_vocabularies() {
  return array(
    'vocabulary_5' => array(
      'name' => 'Tags',
      'machine_name' => 'vocabulary_5',
      'description' => '',
      'hierarchy' => '1',
      'module' => 'taxonomy',
      'weight' => '-4',
    ),
  );
}
