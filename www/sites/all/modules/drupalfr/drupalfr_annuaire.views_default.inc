<?php

/**
 * Implementation of hook_views_default_views().
 */
function drupalfr_annuaire_views_default_views() {
  $view = new view;
  $view->name = 'annuaire_prestataires';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Société',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_type_de_service_value' => array(
      'label' => 'Type de service',
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_type_de_service_value',
      'table' => 'node_data_field_type_de_service',
      'field' => 'field_type_de_service_value',
      'relationship' => 'none',
    ),
    'field_region_value' => array(
      'label' => 'Implantation géographique',
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_region_value',
      'table' => 'node_data_field_region',
      'field' => 'field_region_value',
      'relationship' => 'none',
    ),
    'field_forme_juridique_value' => array(
      'id' => 'field_forme_juridique_value',
      'table' => 'node_data_field_forme_juridique',
      'field' => 'field_forme_juridique_value',
      'label' => 'Forme juridique',
    ),
    'field_taille_value' => array(
      'id' => 'field_taille_value',
      'table' => 'node_data_field_taille',
      'field' => 'field_taille_value',
      'label' => 'Effectif',
    ),
  ));
  $handler->override_option('sorts', array(
    'title' => array(
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'annuaire' => 'annuaire',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'field_type_de_service_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_type_de_service_value_many_to_one_op',
        'identifier' => 'field_type_de_service_value_many_to_one',
        'label' => 'Type de service',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'field_type_de_service_value_many_to_one',
      'table' => 'node_data_field_type_de_service',
      'field' => 'field_type_de_service_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_region_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_region_value_many_to_one_op',
        'identifier' => 'field_region_value_many_to_one',
        'label' => 'Implantation géographique',
        'optional' => 1,
        'single' => 1,
        'remember' => 1,
        'reduce' => 0,
      ),
      'id' => 'field_region_value_many_to_one',
      'table' => 'node_data_field_region',
      'field' => 'field_region_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_forme_juridique_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_forme_juridique_value_many_to_one_op',
        'identifier' => 'field_forme_juridique_value_many_to_one',
        'label' => 'Forme juridique',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'field_forme_juridique_value_many_to_one',
      'table' => 'node_data_field_forme_juridique',
      'field' => 'field_forme_juridique_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_taille_value_many_to_one' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'field_taille_value_many_to_one_op',
        'identifier' => 'field_taille_value_many_to_one',
        'label' => 'Effectif',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'field_taille_value_many_to_one',
      'table' => 'node_data_field_taille',
      'field' => 'field_taille_value_many_to_one',
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('title', 'L\'annuaire des prestataires Drupal en France ');
  $handler->override_option('header', '<h3>Vous recherchez un prestataire en France&nbsp;?</h3>
  
  <p><strong>Nous avons aujourd\'hui <?php print $GLOBALS[\'pager_total_items\'][0]; ?> prestataires inscrits dans notre annuaire. Ils sont prêts à vous aider pour tirer le meilleur de votre solution Drupal&nbsp;!</strong></p>
  
  <p><cite>Remarque&nbsp;: Cet annuaire est libre d\'accès. Pensez-donc à vérifier par vous même les références et les compétences du prestataire que vous choisissez.</cite></p>
  
  ');
  $handler->override_option('header_format', '2');
  $handler->override_option('footer', '<p>Vous êtes un prestataire de service Drupal et vous voulez apparaître dans l\'annuaire&nbsp;?</p>
  <p><a href="node/add/annuaire">Ajoutez votre fiche</a> dès aujourd\'hui&nbsp;! (vous devrez vous identifier pour cela)');
  $handler->override_option('footer_format', '1');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', '50');
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'columns' => array(),
    'default' => 'title',
    'info' => array(
      'title' => array(
        'sortable' => TRUE,
      ),
      'field_region_value' => array(
        'sortable' => TRUE,
      ),
      'field_forme_juridique_value' => array(
        'sortable' => TRUE,
      ),
      'field_taille_value' => array(
        'sortable' => TRUE,
      ),
    ),
    'override' => FALSE,
    'order' => 'asc',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'annuaire-pro');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Annuaire',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => '0',
  ));
  $views[$view->name] = $view;

  return $views;
}



