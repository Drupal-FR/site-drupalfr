<?php
// $Id: simplenews.views.inc,v 1.1.2.4 2009/01/02 11:59:34 sutharsan Exp $

/**
 * @file
 * Views interface for simplenews.
 */

/**
 * Implementation of hook_views_data().
 */
function simplenews_views_data() {
  // Basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['simplenews_newsletters']['table']['group']  = t('Simplenews');

  $data['simplenews_newsletters']['table']['base'] = array(
    'field' => 'nid',
    'title' => t('Node'),
    'help' => t('Using nodes as newsletters.'),
  );

  // joins
  $data['simplenews_newsletters']['table']['join'] = array(
    //...to the node table
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // ----------------------------------------------------------------
  // Fields

  // newsletter sent status
  $data['simplenews_newsletters']['s_status'] = array(
    'title' => t('Sent status'), // The item it appears as on the UI,
    'help' => t('Newsletter sent status: Not sent, Pending (being sent or waiting for cron to run), Sent.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'simplenews_handler_field_newsletter_s_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'simplenews_handler_filter_newsletter_s_status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}

function simplenews_views_handlers() {
  return array(
    'handlers' => array(
      'simplenews_handler_field_newsletter_s_status' => array(
        'parent' => 'views_handler_field',
      ),
      'simplenews_handler_filter_newsletter_s_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
