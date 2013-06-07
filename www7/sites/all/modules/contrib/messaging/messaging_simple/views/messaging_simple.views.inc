<?php

/**
 * @file
 * Contains functions handling views integration.
 */

/**
 * Implementation of hook_views_handlers().
 */
function messaging_simple_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'messaging_simple') . '/views',
    ),
    'handlers' => array(
      // fields
      'messaging_simple_handler_field_body' => array(
        'parent' => 'views_handler_field',
      ),
      //'messaging_simple_handler_field_delete_link' => array(
      //  'parent' => 'views_handler_field',
      //),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function messaging_simple_views_data() {

  $data = array();

  // ---------------------------
  // Messaging_simple base table
  // ---------------------------
  $data['messaging_simple'] = array(
    // Table info.
    'table' => array(
      'group' => t('Messaging'),
      'base' => array(
        'field' => 'mqid',
        'title' => t('Messages (simple)'),
        'help' => t('Messages delivered using the simple delivery method.'),
        'weight' => 0,
      ),
    ),
  );

  $data['messaging_simple']['msid'] = array(
    'title' => t('MSID'),
    'help' => t('The ID of the message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['messaging_simple']['mqid'] = array(
    'title' => t('MQID'),
    'help' => t('The queue ID of the message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['messaging_simple']['subject'] = array(
    'title' => t('Subject'),
    'help' => t('The subject of the message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['messaging_simple']['body'] = array(
    'title' => t('Message'),
    'help' => t('The body of the message.'),
    'field' => array(
      'handler' => 'messaging_simple_handler_field_body',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['messaging_simple']['sent'] = array(
    'title' => t('Sent date'),
    'help' => t('The date the message was sent.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // @TODO: enable once delete has been reworked to not use form submission.
  // $data['messaging_simple']['delete_link'] = array(
  //   'title' => t('Delete link'),
  //   'help' => t('Provides a link to delete the message.'),
  //   'field' => array(
  //     'handler' => 'messaging_simple_handler_field_delete_link',
  //   ),
  // );

  $data['messaging_simple']['sender'] = array(
    'title' => t('Sender'),
    'help' => t('The user ID of the message sender.'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Sender'),
    ),
  );

  $data['messaging_simple']['uid'] = array(
    'title' => t('Receiver'),
    'help' => t('The user ID of the message receiver.'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Receiver'),
    ),
  );

  return $data;

}
