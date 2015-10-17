<?php
/**
 * @file
 * drupalfr_annuaire.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function drupalfr_annuaire_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-annuaire-field_adresse'.
  $field_instances['node-annuaire-field_adresse'] = array(
    'bundle' => 'annuaire',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'Merci de renseigner votre adresse complète.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_adresse',
    'label' => 'Adresse',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 4,
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'default_value' => NULL,
        'rows' => 3,
      ),
      'type' => 'text_textarea',
      'weight' => 12,
    ),
    'widget_type' => 'text_textarea',
  );

  // Exported field_instance: 'node-annuaire-field_asso_member'.
  $field_instances['node-annuaire-field_asso_member'] = array(
    'bundle' => 'annuaire',
    'default_value' => array(
      0 => array(
        'value' => '',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 12,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_asso_member',
    'label' => 'Êtes-vous membre de l\'association Drupal France et francophonie ?',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 9,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 13,
    ),
    'widget_type' => 'optionwidgets_buttons',
  );

  // Exported field_instance: 'node-annuaire-field_description'.
  $field_instances['node-annuaire-field_description'] = array(
    'bundle' => 'annuaire',
    'default_value' => array(
      0 => array(
        'value' => '',
      ),
    ),
    'deleted' => 0,
    'description' => 'Présenter votre structure en quelques lignes. Vous êtes limité à 500 caractères.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => -3,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_description',
    'label' => 'Présentation',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => -3,
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 6,
      ),
      'type' => 'text_textarea',
      'weight' => 2,
    ),
    'widget_type' => 'text_textarea',
  );

  // Exported field_instance: 'node-annuaire-field_forme_juridique'.
  $field_instances['node-annuaire-field_forme_juridique'] = array(
    'bundle' => 'annuaire',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'La forme juridique de votre structure.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 6,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_forme_juridique',
    'label' => 'Forme juridique',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 0,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'default_value' => NULL,
      ),
      'type' => 'options_select',
      'weight' => 7,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-annuaire-field_logo'.
  $field_instances['node-annuaire-field_logo'] = array(
    'bundle' => 'annuaire',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'Vous avez la possibilité d\'insérer le logo de votre société. Merci d\'utiliser une image d\'une résolution maximum de 150x150 pixels.',
    'display' => array(
      'Full' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => -4,
      ),
      'Teaser' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => -4,
      ),
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => -4,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => 'content',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_logo',
    'label' => 'Logo',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'annuaire-pro/logo',
      'file_extensions' => 'jpg jpeg png gif',
      'max_filesize' => 0,
      'max_resolution' => '150x150',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => -4,
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 1,
    ),
    'widget_type' => 'imagefield_widget',
  );

  // Exported field_instance: 'node-annuaire-field_mail'.
  $field_instances['node-annuaire-field_mail'] = array(
    'bundle' => 'annuaire',
    'default_value' => array(
      0 => array(
        'email' => '',
      ),
    ),
    'deleted' => 0,
    'description' => 'L\'adresse e-mail à laquelle il est possible de contacter la/les personne(s) responsables des projets Drupal.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'email',
        'settings' => array(),
        'type' => 'email_default',
        'weight' => 9,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'email',
        'settings' => array(),
        'type' => 'email_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_mail',
    'label' => 'Adresse e-mail',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'weight' => 3,
    'widget' => array(
      'active' => 1,
      'module' => 'email',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'email_textfield',
      'weight' => 11,
    ),
    'widget_type' => 'email_textfield',
  );

  // Exported field_instance: 'node-annuaire-field_member'.
  $field_instances['node-annuaire-field_member'] = array(
    'bundle' => 'annuaire',
    'default_value' => array(
      0 => array(
        'value' => 'Non, pas encore',
      ),
    ),
    'deleted' => 0,
    'description' => '<a href="http://association.drupal.org/" title="Site de l\'association Drupal">L\'association Drupal</a> est une organisation dédiée au développement du  <a href="http://drupal.org/" title="Site du CMS Drupal">CMS Drupal</a>. Elle aide la communauté à organiser des événements autour de Drupal partout dans le monde.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 13,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_member',
    'label' => 'Êtes-vous membre de l\'association Drupal ?',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 10,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 14,
    ),
    'widget_type' => 'optionwidgets_buttons',
  );

  // Exported field_instance: 'node-annuaire-field_region'.
  $field_instances['node-annuaire-field_region'] = array(
    'bundle' => 'annuaire',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_region',
    'label' => 'Région',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-annuaire-field_showcase'.
  $field_instances['node-annuaire-field_showcase'] = array(
    'bundle' => 'annuaire',
    'default_value' => array(
      0 => array(
        'url' => '',
        'title' => '',
        'attributes' => 'N;',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 5,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_showcase',
    'label' => 'Showcase',
    'required' => FALSE,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_label_use_field_label' => FALSE,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'weight' => 0,
    'widget' => array(
      'active' => 1,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 6,
    ),
    'widget_type' => 'link',
  );

  // Exported field_instance: 'node-annuaire-field_siren'.
  $field_instances['node-annuaire-field_siren'] = array(
    'bundle' => 'annuaire',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'Votre numéro d\'identification SIREN tel qu\'il apparaît sur votre K-bis.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 7,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => '',
        ),
        'type' => 'number_integer',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_siren',
    'label' => 'Siren',
    'required' => FALSE,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'weight' => 1,
    'widget' => array(
      'active' => 1,
      'module' => 'number',
      'settings' => array(
        'default_value' => NULL,
      ),
      'type' => 'number',
      'weight' => 8,
    ),
    'widget_type' => 'number',
  );

  // Exported field_instance: 'node-annuaire-field_taille'.
  $field_instances['node-annuaire-field_taille'] = array(
    'bundle' => 'annuaire',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'Nombre d\'employés dans l\'entreprise ou la structure.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_taille',
    'label' => 'Effectif',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => 5,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'default_value' => NULL,
      ),
      'type' => 'options_select',
      'weight' => 5,
    ),
    'widget_type' => 'optionwidgets_select',
  );

  // Exported field_instance: 'node-annuaire-field_telephone'.
  $field_instances['node-annuaire-field_telephone'] = array(
    'bundle' => 'annuaire',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'Merci de préciser le numéro de téléphone de votre structure. Si une personne en particulier s\'occupe des projets Drupal, merci d\'indiquer son numéro.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'phone',
        'settings' => array(),
        'type' => 'phone',
        'weight' => 10,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'phone',
        'settings' => array(),
        'type' => 'phone',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_telephone',
    'label' => 'Téléphone',
    'required' => FALSE,
    'settings' => array(
      'ca_phone_parentheses' => 1,
      'ca_phone_separator' => '-',
      'phone_country_code' => 0,
      'phone_default_country_code' => 1,
      'phone_int_max_length' => 15,
      'user_register_form' => FALSE,
    ),
    'weight' => 3,
    'widget' => array(
      'active' => 1,
      'module' => 'phone',
      'settings' => array(
        'default_value' => NULL,
      ),
      'type' => 'phone_textfield',
      'weight' => 10,
    ),
    'widget_type' => 'phone_textfield',
  );

  // Exported field_instance: 'node-annuaire-field_type_de_service'.
  $field_instances['node-annuaire-field_type_de_service'] = array(
    'bundle' => 'annuaire',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Sélectionnez les prestations que votre société propose. Merci de garder à l\'esprit que les services que présentez ici doivent être en rapport direct avec Drupal.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => -2,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_type_de_service',
    'label' => 'Type de service',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'weight' => -2,
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 3,
    ),
    'widget_type' => 'optionwidgets_buttons',
  );

  // Exported field_instance: 'node-annuaire-field_url'.
  $field_instances['node-annuaire-field_url'] = array(
    'bundle' => 'annuaire',
    'default_value' => '',
    'deleted' => 0,
    'description' => 'Précisez l\'adresse de votre site Internet.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 8,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_url',
    'label' => 'Site Internet',
    'required' => FALSE,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_label_use_field_label' => FALSE,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'weight' => 2,
    'widget' => array(
      'active' => 1,
      'module' => 'link',
      'settings' => array(
        'default_value' => NULL,
      ),
      'type' => 'link_field',
      'weight' => 9,
    ),
    'widget_type' => 'link',
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('<a href="http://association.drupal.org/" title="Site de l\'association Drupal">L\'association Drupal</a> est une organisation dédiée au développement du  <a href="http://drupal.org/" title="Site du CMS Drupal">CMS Drupal</a>. Elle aide la communauté à organiser des événements autour de Drupal partout dans le monde.');
  t('Adresse');
  t('Adresse e-mail');
  t('Effectif');
  t('Forme juridique');
  t('L\'adresse e-mail à laquelle il est possible de contacter la/les personne(s) responsables des projets Drupal.');
  t('La forme juridique de votre structure.');
  t('Logo');
  t('Merci de préciser le numéro de téléphone de votre structure. Si une personne en particulier s\'occupe des projets Drupal, merci d\'indiquer son numéro.');
  t('Merci de renseigner votre adresse complète.');
  t('Nombre d\'employés dans l\'entreprise ou la structure.');
  t('Précisez l\'adresse de votre site Internet.');
  t('Présentation');
  t('Présenter votre structure en quelques lignes. Vous êtes limité à 500 caractères.');
  t('Région');
  t('Showcase');
  t('Siren');
  t('Site Internet');
  t('Sélectionnez les prestations que votre société propose. Merci de garder à l\'esprit que les services que présentez ici doivent être en rapport direct avec Drupal.');
  t('Type de service');
  t('Téléphone');
  t('Votre numéro d\'identification SIREN tel qu\'il apparaît sur votre K-bis.');
  t('Vous avez la possibilité d\'insérer le logo de votre société. Merci d\'utiliser une image d\'une résolution maximum de 150x150 pixels.');
  t('Êtes-vous membre de l\'association Drupal ?');
  t('Êtes-vous membre de l\'association Drupal France et francophonie ?');

  return $field_instances;
}
