<?php

/**
 * @file
 * drupalfr_media.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function drupalfr_media_taxonomy_default_vocabularies() {
  return array(
    'scald_authors' => array(
      'name' => 'Authors',
      'machine_name' => 'scald_authors',
      'description' => 'Scald authors vocabulary',
      'hierarchy' => 0,
      'module' => 'scald',
      'weight' => -5,
    ),
    'scald_tags' => array(
      'name' => 'Scald tags',
      'machine_name' => 'scald_tags',
      'description' => 'Scald tags vocabulary',
      'hierarchy' => 0,
      'module' => 'scald',
      'weight' => -5,
    ),
  );
}
