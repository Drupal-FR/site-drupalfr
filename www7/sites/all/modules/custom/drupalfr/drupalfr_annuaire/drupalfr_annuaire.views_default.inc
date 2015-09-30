<?php
/**
 * @file
 * drupalfr_annuaire.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function drupalfr_annuaire_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'annuaire_prestataires';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'L\'annuaire des prestataires Drupal en France ';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Appliquer';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Trier par';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '6';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Éléments par page';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Tout -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Décalage';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« premier';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ précédent';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'suivant ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'dernier »';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
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
  );
  $handler->display->display_options['style_options']['override'] = FALSE;
  /* Entête: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<h3>Vous recherchez un prestataire en France&nbsp;?</h3>
  
  <p><strong>Nous avons aujourd\'hui <?php print $GLOBALS[\'pager_total_items\'][0]; ?> prestataires inscrits dans notre annuaire. Ils sont prêts à vous aider pour tirer le meilleur de votre solution Drupal&nbsp;!</strong></p>
  
  <p><cite>Remarque&nbsp;: Cet annuaire est libre d\'accès. Pensez donc à vérifier par vous-même les références et compétences du prestataire que vous choisissez.</cite></p>
  
  ';
  $handler->display->display_options['header']['text']['format'] = '2';
  /* Pied de page: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'text';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['content'] = 'Vous êtes un prestataire de service Drupal et vous voulez apparaître dans l\'annuaire ?
<a href="node/add/annuaire">Ajoutez votre fiche</a> dès aujourd\'hui ! (vous devrez vous identifier pour cela)';
  $handler->display->display_options['footer']['text']['format'] = '1';
  /* Champ: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Société';
  /* Critère de filtrage: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'annuaire' => 'annuaire',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

  /* Display: Annuaire */
  $handler = $view->new_display('page', 'Annuaire', 'page_2');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Champ: Content: Logo */
  $handler->display->display_options['fields']['field_logo']['id'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['table'] = 'field_data_field_logo';
  $handler->display->display_options['fields']['field_logo']['field'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_logo']['settings'] = array(
    'image_style' => 'large',
    'image_link' => '',
  );
  /* Champ: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Société';
  /* Champ: Content: Type de service */
  $handler->display->display_options['fields']['field_type_de_service']['id'] = 'field_type_de_service';
  $handler->display->display_options['fields']['field_type_de_service']['table'] = 'field_data_field_type_de_service';
  $handler->display->display_options['fields']['field_type_de_service']['field'] = 'field_type_de_service';
  /* Champ: Content: Région */
  $handler->display->display_options['fields']['field_region']['id'] = 'field_region';
  $handler->display->display_options['fields']['field_region']['table'] = 'field_data_field_region';
  $handler->display->display_options['fields']['field_region']['field'] = 'field_region';
  /* Champ: Content: Effectif */
  $handler->display->display_options['fields']['field_taille']['id'] = 'field_taille';
  $handler->display->display_options['fields']['field_taille']['table'] = 'field_data_field_taille';
  $handler->display->display_options['fields']['field_taille']['field'] = 'field_taille';
  /* Champ: Content: Forme juridique */
  $handler->display->display_options['fields']['field_forme_juridique']['id'] = 'field_forme_juridique';
  $handler->display->display_options['fields']['field_forme_juridique']['table'] = 'field_data_field_forme_juridique';
  $handler->display->display_options['fields']['field_forme_juridique']['field'] = 'field_forme_juridique';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Critère de filtrage: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'annuaire' => 'annuaire',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Critère de filtrage: Content: Effectif (field_taille) */
  $handler->display->display_options['filters']['field_taille_value']['id'] = 'field_taille_value';
  $handler->display->display_options['filters']['field_taille_value']['table'] = 'field_data_field_taille';
  $handler->display->display_options['filters']['field_taille_value']['field'] = 'field_taille_value';
  $handler->display->display_options['filters']['field_taille_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_taille_value']['expose']['operator_id'] = 'field_taille_value_op';
  $handler->display->display_options['filters']['field_taille_value']['expose']['label'] = 'Effectif';
  $handler->display->display_options['filters']['field_taille_value']['expose']['operator'] = 'field_taille_value_op';
  $handler->display->display_options['filters']['field_taille_value']['expose']['identifier'] = 'field_taille_value';
  $handler->display->display_options['filters']['field_taille_value']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    9 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
  );
  /* Critère de filtrage: Content: Forme juridique (field_forme_juridique) */
  $handler->display->display_options['filters']['field_forme_juridique_value']['id'] = 'field_forme_juridique_value';
  $handler->display->display_options['filters']['field_forme_juridique_value']['table'] = 'field_data_field_forme_juridique';
  $handler->display->display_options['filters']['field_forme_juridique_value']['field'] = 'field_forme_juridique_value';
  $handler->display->display_options['filters']['field_forme_juridique_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['operator_id'] = 'field_forme_juridique_value_op';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['label'] = 'Forme juridique';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['operator'] = 'field_forme_juridique_value_op';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['identifier'] = 'field_forme_juridique_value';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    9 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
  );
  /* Critère de filtrage: Content: Région (field_region) */
  $handler->display->display_options['filters']['field_region_value']['id'] = 'field_region_value';
  $handler->display->display_options['filters']['field_region_value']['table'] = 'field_data_field_region';
  $handler->display->display_options['filters']['field_region_value']['field'] = 'field_region_value';
  $handler->display->display_options['filters']['field_region_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_region_value']['expose']['operator_id'] = 'field_region_value_op';
  $handler->display->display_options['filters']['field_region_value']['expose']['label'] = 'Région';
  $handler->display->display_options['filters']['field_region_value']['expose']['operator'] = 'field_region_value_op';
  $handler->display->display_options['filters']['field_region_value']['expose']['identifier'] = 'field_region_value';
  $handler->display->display_options['filters']['field_region_value']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    9 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
  );
  /* Critère de filtrage: Content: Type de service (field_type_de_service) */
  $handler->display->display_options['filters']['field_type_de_service_value']['id'] = 'field_type_de_service_value';
  $handler->display->display_options['filters']['field_type_de_service_value']['table'] = 'field_data_field_type_de_service';
  $handler->display->display_options['filters']['field_type_de_service_value']['field'] = 'field_type_de_service_value';
  $handler->display->display_options['filters']['field_type_de_service_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['operator_id'] = 'field_type_de_service_value_op';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['label'] = 'Type de service';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['operator'] = 'field_type_de_service_value_op';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['identifier'] = 'field_type_de_service_value';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    9 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
  );
  $handler->display->display_options['path'] = 'annuaire-pro';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Annuaire';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['annuaire_prestataires'] = array(
    t('Defaults'),
    t('L\'annuaire des prestataires Drupal en France '),
    t('plus'),
    t('Appliquer'),
    t('Réinitialiser'),
    t('Trier par'),
    t('Asc'),
    t('Desc'),
    t('Éléments par page'),
    t('- Tout -'),
    t('Décalage'),
    t('« premier'),
    t('‹ précédent'),
    t('suivant ›'),
    t('dernier »'),
    t('<h3>Vous recherchez un prestataire en France&nbsp;?</h3>
  
  <p><strong>Nous avons aujourd\'hui <?php print $GLOBALS[\'pager_total_items\'][0]; ?> prestataires inscrits dans notre annuaire. Ils sont prêts à vous aider pour tirer le meilleur de votre solution Drupal&nbsp;!</strong></p>
  
  <p><cite>Remarque&nbsp;: Cet annuaire est libre d\'accès. Pensez donc à vérifier par vous-même les références et compétences du prestataire que vous choisissez.</cite></p>
  
  '),
    t('Vous êtes un prestataire de service Drupal et vous voulez apparaître dans l\'annuaire ?
<a href="node/add/annuaire">Ajoutez votre fiche</a> dès aujourd\'hui ! (vous devrez vous identifier pour cela)'),
    t('Société'),
    t('Annuaire'),
    t('more'),
    t('Logo'),
    t('Type de service'),
    t('Région'),
    t('Effectif'),
    t('Forme juridique'),
  );
  $export['annuaire_prestataires'] = $view;

  return $export;
}
