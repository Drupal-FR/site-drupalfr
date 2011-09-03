<?php
/**
 * @file
 * drupalfr_annuaire.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function drupalfr_annuaire_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'annuaire_prestataires';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'L\'annuaire des prestataires Drupal en France ';
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
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
  /* En-tête: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'text';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = FALSE;
  $handler->display->display_options['header']['text']['content'] = '<h3>Vous recherchez un prestataire en France&nbsp;?</h3>
    
    <p><strong>Nous avons aujourd\'hui XxX prestataires inscrits dans notre annuaire. Ils sont prêts à vous aider pour tirer le meilleur de votre solution Drupal&nbsp;!</strong></p>
    
    <p><cite>Remarque&nbsp;: Cet annuaire est libre d\'accès. Pensez-donc à vérifier par vous même les références et les compétences du prestataire que vous choisissez.</cite></p>';
  /* Pied de page: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'area';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['empty'] = FALSE;
  $handler->display->display_options['footer']['text']['content'] = 'Vous êtes un prestataire de service Drupal et vous voulez apparaître dans l\'annuaire ?

  <a href="node/add/annuaire">Ajoutez votre fiche</a> dès aujourd\'hui ! (vous devrez vous identifier pour cela)';
  /* Field: Contenu: Titre */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Société';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Contenu: Forme juridique */
  $handler->display->display_options['fields']['field_forme_juridique']['id'] = 'field_forme_juridique';
  $handler->display->display_options['fields']['field_forme_juridique']['table'] = 'field_data_field_forme_juridique';
  $handler->display->display_options['fields']['field_forme_juridique']['field'] = 'field_forme_juridique';
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_forme_juridique']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_forme_juridique']['field_api_classes'] = 0;
  /* Field: Contenu: Implantation géographique */
  $handler->display->display_options['fields']['field_region']['id'] = 'field_region';
  $handler->display->display_options['fields']['field_region']['table'] = 'field_data_field_region';
  $handler->display->display_options['fields']['field_region']['field'] = 'field_region';
  $handler->display->display_options['fields']['field_region']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_region']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_region']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_region']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_region']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_region']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_region']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_region']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_region']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_region']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_region']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_region']['field_api_classes'] = 0;
  /* Field: Contenu: Effectif */
  $handler->display->display_options['fields']['field_taille']['id'] = 'field_taille';
  $handler->display->display_options['fields']['field_taille']['table'] = 'field_data_field_taille';
  $handler->display->display_options['fields']['field_taille']['field'] = 'field_taille';
  $handler->display->display_options['fields']['field_taille']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_taille']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_taille']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_taille']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_taille']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_taille']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_taille']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_taille']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_taille']['field_api_classes'] = 0;
  /* Field: Contenu: Type de service */
  $handler->display->display_options['fields']['field_type_de_service']['id'] = 'field_type_de_service';
  $handler->display->display_options['fields']['field_type_de_service']['table'] = 'field_data_field_type_de_service';
  $handler->display->display_options['fields']['field_type_de_service']['field'] = 'field_type_de_service';
  $handler->display->display_options['fields']['field_type_de_service']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_type_de_service']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_type_de_service']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_type_de_service']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_type_de_service']['field_api_classes'] = 0;
  /* Sort criterion: Contenu: Titre */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Fields: Forme juridique (field_forme_juridique) */
  $handler->display->display_options['filters']['field_forme_juridique_value']['id'] = 'field_forme_juridique_value';
  $handler->display->display_options['filters']['field_forme_juridique_value']['table'] = 'field_data_field_forme_juridique';
  $handler->display->display_options['filters']['field_forme_juridique_value']['field'] = 'field_forme_juridique_value';
  $handler->display->display_options['filters']['field_forme_juridique_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['operator_id'] = 'field_forme_juridique_value_op';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['label'] = 'Forme juridique';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['operator'] = 'field_forme_juridique_value_op';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['identifier'] = 'field_forme_juridique_value';
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['field_forme_juridique_value']['expose']['reduce'] = 0;
  /* Filter criterion: Fields: Implantation géographique (field_region) */
  $handler->display->display_options['filters']['field_region_value']['id'] = 'field_region_value';
  $handler->display->display_options['filters']['field_region_value']['table'] = 'field_data_field_region';
  $handler->display->display_options['filters']['field_region_value']['field'] = 'field_region_value';
  $handler->display->display_options['filters']['field_region_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_region_value']['expose']['operator_id'] = 'field_region_value_op';
  $handler->display->display_options['filters']['field_region_value']['expose']['label'] = 'Implantation géographique';
  $handler->display->display_options['filters']['field_region_value']['expose']['operator'] = 'field_region_value_op';
  $handler->display->display_options['filters']['field_region_value']['expose']['identifier'] = 'field_region_value';
  $handler->display->display_options['filters']['field_region_value']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['field_region_value']['expose']['reduce'] = 0;
  /* Filter criterion: Fields: Effectif (field_taille) */
  $handler->display->display_options['filters']['field_taille_value']['id'] = 'field_taille_value';
  $handler->display->display_options['filters']['field_taille_value']['table'] = 'field_data_field_taille';
  $handler->display->display_options['filters']['field_taille_value']['field'] = 'field_taille_value';
  $handler->display->display_options['filters']['field_taille_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_taille_value']['expose']['operator_id'] = 'field_taille_value_op';
  $handler->display->display_options['filters']['field_taille_value']['expose']['label'] = 'Éffectif';
  $handler->display->display_options['filters']['field_taille_value']['expose']['operator'] = 'field_taille_value_op';
  $handler->display->display_options['filters']['field_taille_value']['expose']['identifier'] = 'field_taille_value';
  $handler->display->display_options['filters']['field_taille_value']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['field_taille_value']['expose']['reduce'] = 0;
  /* Filter criterion: Fields: Type de service (field_type_de_service) */
  $handler->display->display_options['filters']['field_type_de_service_value']['id'] = 'field_type_de_service_value';
  $handler->display->display_options['filters']['field_type_de_service_value']['table'] = 'field_data_field_type_de_service';
  $handler->display->display_options['filters']['field_type_de_service_value']['field'] = 'field_type_de_service_value';
  $handler->display->display_options['filters']['field_type_de_service_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['operator_id'] = 'field_type_de_service_value_op';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['label'] = 'Type de service';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['operator'] = 'field_type_de_service_value_op';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['identifier'] = 'field_type_de_service_value';
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['field_type_de_service_value']['expose']['reduce'] = 0;
  /* Filter criterion: Contenu: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'annuaire' => 'annuaire',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Contenu: Publié */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'annuaire-pro';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Annuaire';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['annuaire_prestataires'] = array(
    t('Defaults'),
    t('L\'annuaire des prestataires Drupal en France '),
    t('plus'),
    t('Apply'),
    t('Réinitialiser'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('<h3>Vous recherchez un prestataire en France&nbsp;?</h3>
    
    <p><strong>Nous avons aujourd\'hui XxX prestataires inscrits dans notre annuaire. Ils sont prêts à vous aider pour tirer le meilleur de votre solution Drupal&nbsp;!</strong></p>
    
    <p><cite>Remarque&nbsp;: Cet annuaire est libre d\'accès. Pensez-donc à vérifier par vous même les références et les compétences du prestataire que vous choisissez.</cite></p>'),
    t('Vous êtes un prestataire de service Drupal et vous voulez apparaître dans l\'annuaire ?

  <a href="node/add/annuaire">Ajoutez votre fiche</a> dès aujourd\'hui ! (vous devrez vous identifier pour cela)'),
    t('Société'),
    t('Forme juridique'),
    t('Implantation géographique'),
    t('Effectif'),
    t('Type de service'),
    t('Éffectif'),
    t('Page'),
  );
  
  $export['annuaire_prestataires'] = $view;

  return $export;
}
