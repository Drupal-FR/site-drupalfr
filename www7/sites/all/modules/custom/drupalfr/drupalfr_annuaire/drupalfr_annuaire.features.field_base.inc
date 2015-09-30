<?php
/**
 * @file
 * drupalfr_annuaire.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function drupalfr_annuaire_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_adresse'
  $field_bases['field_adresse'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_adresse',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => '',
      'max_length' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text_long',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_asso_member'
  $field_bases['field_asso_member'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_asso_member',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Oui' => 'Oui',
        'Non, pas encore' => 'Non, pas encore',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => NULL,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_description'
  $field_bases['field_description'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_description',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'allowed_values' => '',
      'allowed_values_php' => NULL,
      'max_length' => 500,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'text',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_forme_juridique'
  $field_bases['field_forme_juridique'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_forme_juridique',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Association' => 'Association',
        'Indépendant' => 'Indépendant',
        'Libéral' => 'Libéral',
        'SARL' => 'SARL',
        'SAS/SA' => 'SAS/SA',
        'Autre' => 'Autre',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_logo'
  $field_bases['field_logo'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_logo',
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => NULL,
      'description_field' => 0,
      'list_default' => 1,
      'list_field' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'image',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_mail'
  $field_bases['field_mail'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_mail',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'email',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'email',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_member'
  $field_bases['field_member'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_member',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Oui' => 'Oui',
        'Non, pas encore' => 'Non, pas encore',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => NULL,
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_region'
  $field_bases['field_region'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_region',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        '01 Île de France' => '01 Île de France',
        '02 Nord Ouest' => '02 Nord Ouest',
        '03 Nord Est' => '03 Nord Est',
        '04 Sud Est' => '04 Sud Est',
        '05 Sud Ouest' => '05 Sud Ouest',
        'Outre mer' => 'Outre mer',
        'Belgique' => 'Belgique',
        'Suisse' => 'Suisse',
        'Autre' => 'Autre',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_showcase'
  $field_bases['field_showcase'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_showcase',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => '_blank',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 0,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_siren'
  $field_bases['field_siren'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_siren',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_php' => '',
      'append_position' => NULL,
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
    ),
    'translatable' => 0,
    'type' => 'number_integer',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_taille'
  $field_bases['field_taille'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_taille',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Une personne' => 'Une personne',
        'Entre 2 et 4' => 'Entre 2 et 4',
        'Entre 5 et 10' => 'Entre 5 et 10',
        'Entre 11 et 50' => 'Entre 11 et 50',
        'Entre 51 et 499' => 'Entre 51 et 499',
        'Plus de 500' => 'Plus de 500',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_telephone'
  $field_bases['field_telephone'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_telephone',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'phone',
    'settings' => array(
      'country' => 'fr',
      'phone_country_code' => 0,
    ),
    'translatable' => 0,
    'type' => 'phone',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_type_de_service'
  $field_bases['field_type_de_service'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_type_de_service',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Hébergement' => 'Hébergement',
        'Développement' => 'Développement',
        'Design' => 'Design',
        'Conseil/Architecture' => 'Conseil/Architecture',
        'Formation' => 'Formation',
        'Accessibilité' => 'Accessibilité',
        'cabinet' => 'Cabinet de recrutement',
        'Autres' => 'Autres',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
      'text_processing' => 0,
    ),
    'translatable' => 0,
    'type' => 'list_text',
    'type_name' => 'annuaire',
  );

  // Exported field_base: 'field_url'
  $field_bases['field_url'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_url',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => NULL,
      'title' => 'none',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
    'type_name' => 'annuaire',
  );

  return $field_bases;
}
