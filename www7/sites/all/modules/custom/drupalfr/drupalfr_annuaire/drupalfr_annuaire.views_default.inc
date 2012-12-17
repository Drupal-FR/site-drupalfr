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
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
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
  /* En-tête: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['content'] = '<h3>Vous recherchez un prestataire en France&nbsp;?</h3>
  
  <p><strong>Nous avons aujourd\'hui <?php print $GLOBALS[\'pager_total_items\'][0]; ?> prestataires inscrits dans notre annuaire. Ils sont prêts à vous aider pour tirer le meilleur de votre solution Drupal&nbsp;!</strong></p>
  
  <p><cite>Remarque&nbsp;: Cet annuaire est libre d\'accès. Pensez-donc à vérifier par vous même les références et les compétences du prestataire que vous choisissez.</cite></p>
  
  ';
  $handler->display->display_options['header']['text']['format'] = '2';
  /* Pied de page: Global: Text area */
  $handler->display->display_options['footer']['text']['id'] = 'area';
  $handler->display->display_options['footer']['text']['table'] = 'views';
  $handler->display->display_options['footer']['text']['field'] = 'area';
  $handler->display->display_options['footer']['text']['content'] = 'Vous êtes un prestataire de service Drupal et vous voulez apparaître dans l\'annuaire ?

<a href="node/add/annuaire">Ajoutez votre fiche</a> dès aujourd\'hui ! (vous devrez vous identifier pour cela)';
  $handler->display->display_options['footer']['text']['format'] = '1';
  /* Field: Contenu: Titre */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Société';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_type_de_service_value']['id'] = 'field_type_de_service_value';
  $handler->display->display_options['fields']['field_type_de_service_value']['table'] = 'node_data_field_type_de_service';
  $handler->display->display_options['fields']['field_type_de_service_value']['field'] = 'field_type_de_service_value';
  $handler->display->display_options['fields']['field_type_de_service_value']['label'] = 'Type de service';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_region_value']['id'] = 'field_region_value';
  $handler->display->display_options['fields']['field_region_value']['table'] = 'node_data_field_region';
  $handler->display->display_options['fields']['field_region_value']['field'] = 'field_region_value';
  $handler->display->display_options['fields']['field_region_value']['label'] = 'Implantation géographique';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_forme_juridique_value']['id'] = 'field_forme_juridique_value';
  $handler->display->display_options['fields']['field_forme_juridique_value']['table'] = 'node_data_field_forme_juridique';
  $handler->display->display_options['fields']['field_forme_juridique_value']['field'] = 'field_forme_juridique_value';
  $handler->display->display_options['fields']['field_forme_juridique_value']['label'] = 'Forme juridique';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_taille_value']['id'] = 'field_taille_value';
  $handler->display->display_options['fields']['field_taille_value']['table'] = 'node_data_field_taille';
  $handler->display->display_options['fields']['field_taille_value']['field'] = 'field_taille_value';
  $handler->display->display_options['fields']['field_taille_value']['label'] = 'Effectif';
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  /* Filter criterion: Contenu: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'annuaire' => 'annuaire',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['id'] = 'field_type_de_service_value_many_to_one';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['table'] = 'node_data_field_type_de_service';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['field'] = 'field_type_de_service_value_many_to_one';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['operator'] = 'or';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['value'] = array();
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['group'] = '0';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['expose']['operator_id'] = 'field_type_de_service_value_many_to_one_op';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['expose']['label'] = 'Type de service';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['expose']['operator'] = 'field_type_de_service_value_many_to_one_op';
  $handler->display->display_options['filters']['field_type_de_service_value_many_to_one']['expose']['identifier'] = 'field_type_de_service_value_many_to_one';
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_region_value_many_to_one']['id'] = 'field_region_value_many_to_one';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['table'] = 'node_data_field_region';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['field'] = 'field_region_value_many_to_one';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['operator'] = 'or';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['value'] = array();
  $handler->display->display_options['filters']['field_region_value_many_to_one']['group'] = '0';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_region_value_many_to_one']['expose']['operator_id'] = 'field_region_value_many_to_one_op';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['expose']['label'] = 'Implantation géographique';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['expose']['operator'] = 'field_region_value_many_to_one_op';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['expose']['identifier'] = 'field_region_value_many_to_one';
  $handler->display->display_options['filters']['field_region_value_many_to_one']['expose']['remember'] = TRUE;
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['id'] = 'field_forme_juridique_value_many_to_one';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['table'] = 'node_data_field_forme_juridique';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['field'] = 'field_forme_juridique_value_many_to_one';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['operator'] = 'or';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['value'] = array();
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['group'] = '0';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['expose']['operator_id'] = 'field_forme_juridique_value_many_to_one_op';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['expose']['label'] = 'Forme juridique';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['expose']['operator'] = 'field_forme_juridique_value_many_to_one_op';
  $handler->display->display_options['filters']['field_forme_juridique_value_many_to_one']['expose']['identifier'] = 'field_forme_juridique_value_many_to_one';
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['id'] = 'field_taille_value_many_to_one';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['table'] = 'node_data_field_taille';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['field'] = 'field_taille_value_many_to_one';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['operator'] = 'or';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['value'] = array();
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['group'] = '0';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['expose']['operator_id'] = 'field_taille_value_many_to_one_op';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['expose']['label'] = 'Effectif';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['expose']['operator'] = 'field_taille_value_many_to_one_op';
  $handler->display->display_options['filters']['field_taille_value_many_to_one']['expose']['identifier'] = 'field_taille_value_many_to_one';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'annuaire-pro';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Annuaire';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Annuaire */
  $handler = $view->new_display('page', 'Annuaire', 'page_2');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Contenu: Titre */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Société';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Contenu: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'annuaire' => 'annuaire',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'annuairelogo';
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
    t('« premier'),
    t('‹ précédent'),
    t('suivant ›'),
    t('dernier »'),
    t('<h3>Vous recherchez un prestataire en France&nbsp;?</h3>
  
  <p><strong>Nous avons aujourd\'hui <?php print $GLOBALS[\'pager_total_items\'][0]; ?> prestataires inscrits dans notre annuaire. Ils sont prêts à vous aider pour tirer le meilleur de votre solution Drupal&nbsp;!</strong></p>
  
  <p><cite>Remarque&nbsp;: Cet annuaire est libre d\'accès. Pensez-donc à vérifier par vous même les références et les compétences du prestataire que vous choisissez.</cite></p>
  
  '),
    t('Vous êtes un prestataire de service Drupal et vous voulez apparaître dans l\'annuaire ?

<a href="node/add/annuaire">Ajoutez votre fiche</a> dès aujourd\'hui ! (vous devrez vous identifier pour cela)'),
    t('Société'),
    t('Type de service'),
    t('Implantation géographique'),
    t('Forme juridique'),
    t('Effectif'),
    t('Page'),
    t('Annuaire'),
  );
  $export['annuaire_prestataires'] = $view;

  return $export;
}
