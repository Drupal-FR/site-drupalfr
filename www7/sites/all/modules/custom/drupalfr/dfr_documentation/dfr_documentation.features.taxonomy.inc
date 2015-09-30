<?php
/**
 * @file
 * dfr_documentation.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function dfr_documentation_taxonomy_default_vocabularies() {
  return array(
    'vocabulary_1' => array(
      'name' => 'Forum',
      'machine_name' => 'vocabulary_1',
      'description' => 'Catégorie de vocabulaire pour les forums.',
      'hierarchy' => 0,
      'module' => 'forum',
      'weight' => -10,
    ),
    'vocabulary_2' => array(
      'name' => 'Image Galleries',
      'machine_name' => 'vocabulary_2',
      'description' => '',
      'hierarchy' => 1,
      'module' => 'image',
      'weight' => -6,
    ),
    'vocabulary_3' => array(
      'name' => 'En page d\'accueil',
      'machine_name' => 'vocabulary_3',
      'description' => 'Categories des posts de pages d\'accueil',
      'hierarchy' => 1,
      'module' => 'taxonomy',
      'weight' => -8,
    ),
    'vocabulary_4' => array(
      'name' => 'Version de Drupal',
      'machine_name' => 'vocabulary_4',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -9,
    ),
    'vocabulary_6' => array(
      'name' => 'faq',
      'machine_name' => 'vocabulary_6',
      'description' => 'Vocabulaire spécifique à l\'affichage des questions fréquemment posées.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -7,
    ),
    'vocabulary_7' => array(
      'name' => 'Newsletter',
      'machine_name' => 'vocabulary_7',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'simplenews',
      'weight' => -5,
    ),
    'vocabulary_8' => array(
      'name' => 'Userpoints',
      'machine_name' => 'vocabulary_8',
      'description' => 'Automatically created by the userpoints module',
      'hierarchy' => 1,
      'module' => 'userpoints',
      'weight' => 0,
    ),
  );
}
