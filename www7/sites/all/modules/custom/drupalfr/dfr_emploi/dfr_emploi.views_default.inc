<?php
/**
 * @file
 * dfr_emploi.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function dfr_emploi_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'offres';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Offres';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Offres d\'emploi';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'Toutes les offres';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« premier';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ précédent';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'suivant ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'dernier »';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_type_contrat' => 'field_type_contrat',
    'field_societe' => 'field_societe',
  );
  $handler->display->display_options['row_options']['separator'] = ' - ';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Field: Contenu: Titre */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['max_length'] = '30';
  $handler->display->display_options['fields']['title']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_type'] = 'h3';
  /* Field: Contenu: Type de contrat */
  $handler->display->display_options['fields']['field_type_contrat']['id'] = 'field_type_contrat';
  $handler->display->display_options['fields']['field_type_contrat']['table'] = 'field_data_field_type_contrat';
  $handler->display->display_options['fields']['field_type_contrat']['field'] = 'field_type_contrat';
  $handler->display->display_options['fields']['field_type_contrat']['label'] = '';
  $handler->display->display_options['fields']['field_type_contrat']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_type_contrat']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Contenu: Société */
  $handler->display->display_options['fields']['field_societe']['id'] = 'field_societe';
  $handler->display->display_options['fields']['field_societe']['table'] = 'field_data_field_societe';
  $handler->display->display_options['fields']['field_societe']['field'] = 'field_societe';
  $handler->display->display_options['fields']['field_societe']['label'] = '';
  $handler->display->display_options['fields']['field_societe']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_societe']['type'] = 'node_reference_plain';
  /* Sort criterion: Contenu: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Contenu: Publié */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Contenu: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'offre' => 'offre',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Contenu: Publié */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Contenu: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'offre' => 'offre',
  );
  /* Filter criterion: Contenu: Implantation géographique (field_region) */
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
  /* Filter criterion: Contenu: Type de contrat (field_type_contrat) */
  $handler->display->display_options['filters']['field_type_contrat_tid']['id'] = 'field_type_contrat_tid';
  $handler->display->display_options['filters']['field_type_contrat_tid']['table'] = 'field_data_field_type_contrat';
  $handler->display->display_options['filters']['field_type_contrat_tid']['field'] = 'field_type_contrat_tid';
  $handler->display->display_options['filters']['field_type_contrat_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_type_contrat_tid']['expose']['operator_id'] = 'field_type_contrat_tid_op';
  $handler->display->display_options['filters']['field_type_contrat_tid']['expose']['label'] = 'Type de contrat';
  $handler->display->display_options['filters']['field_type_contrat_tid']['expose']['operator'] = 'field_type_contrat_tid_op';
  $handler->display->display_options['filters']['field_type_contrat_tid']['expose']['identifier'] = 'field_type_contrat_tid';
  $handler->display->display_options['filters']['field_type_contrat_tid']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    9 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
  );
  $handler->display->display_options['filters']['field_type_contrat_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_type_contrat_tid']['vocabulary'] = 'type_contrat';
  $handler->display->display_options['path'] = 'emploi/tous';

  /* Display: Bloc */
  $handler = $view->new_display('block', 'Bloc', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Dernières offres d\'emploi';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $translatables['offres'] = array(
    t('Master'),
    t('Offres d\'emploi'),
    t('Toutes les offres'),
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
    t('Page'),
    t('plus'),
    t('Région'),
    t('Type de contrat'),
    t('Bloc'),
    t('Dernières offres d\'emploi'),
  );
  $export['offres'] = $view;

  return $export;
}
