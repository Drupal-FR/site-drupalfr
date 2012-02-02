<?php
// $Id: fullcalendar.views_default.inc,v 1.2.2.1 2011/02/13 18:38:43 timplunkett Exp $

/**
 * @file
 * Provides a default view on behalf of the FullCalendar module.
 */

/**
 * Implements hook_views_default_views().
 */
function fullcalendar_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'fullcalendar';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE;

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'fullcalendar';
  $handler->display->display_options['style_options']['modules']['fc_theme'] = 1;
  $handler->display->display_options['row_plugin'] = 'fullcalendar_node';
  /* Empty text: FullCalendar: Emtpy calendar */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'fullcalendar';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  /* Field: Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Node: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  /* Filter: Node: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: FullCalendar */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'fullcalendar';

  $views[$view->name] = $view;
  return $views;
}
