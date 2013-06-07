<?php

/**
 * Implementation of hook_views_data().
 */
function notifications_views_views_data() {
  $data = array();

  $data['notifications_subscription']['table']['group'] = t('Notifications');

  $data['notifications_subscription']['table']['base'] = array(
        'field' => 'sid',
        'title' => t('Notifications'),
        'help' => t('Notifications subscriptions.'),
        'weight' => 0,
  );

  // sid field
  $data['notifications_subscription']['sid'] = array(
    'title' => t('SID'),
    'help' => t('Subscription ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // uid field
  $data['notifications_subscription']['uid'] = array(
    'title' => t('Subscriber UID'),
    'help' => t('The UID of the subscriber.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('subscriber'),
    ),
  );

  $data['notifications_subscription']['type'] = array(
    'title' => t('Type'),
    'help' => t('Subscription type.'),
    'field' => array(
      'handler' => 'notifications_views_handler_field_subscription_type',
    ),
    'filter' => array(
      'title' => t('Type'),
      'handler' => 'notifications_views_handler_filter_subscription_type',
    ),
    'argument' => array(
      'handler' => 'notifications_views_handler_argument_subscription_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['notifications_subscription']['send_method'] = array(
    'title' => t('Send method'),
    'help' => t('Subscription send method.'),
    'field' => array(
      'handler' => 'notifications_views_handler_field_subscription_send_method',
    ),
    'filter' => array(
      'title' => t('Method'),
      'handler' => 'notifications_views_handler_filter_subscription_send_method',
    ),
    'argument' => array(
      'handler' => 'notifications_views_handler_argument_subscription_send_method',
    ),
  );

  $data['notifications_subscription']['send_interval'] = array(
    'title' => t('Send Interval'),
    'help' => t('Subscription send interval.'),
    'field' => array(
      'handler' => 'notifications_views_handler_field_subscription_interval',
    ),
    'filter' => array(
      'title' => t('Send Interval'),
      'handler' => 'notifications_views_handler_filter_subscription_interval',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['notifications_subscription']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the subscription was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['notifications_subscription']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the subscription was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['notifications_subscription']['edit_subscription'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the subscription.'),
      'handler' => 'notifications_views_handler_field_subscription_link_edit',
    ),
  );

  $data['notifications_subscription']['delete_subscription'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the subscription.'),
      'handler' => 'notifications_views_handler_field_subscription_link_delete',
    ),
  );

  $data['notifications_subscription']['uid_current'] = array(
    'title' => t('Subscription by the current user'),
    'help' => t('Filter the view to the currently logged in user.'),
    'filter' => array(
      'real field' => 'uid',
      'title' => t('Current'),
      'handler' => 'views_handler_filter_user_current',
    ),
  );

  // -------------------------------------------------------------
  // notifications_subscription_fields joins to users, node, and notifications
  // -------------------------------------------------------------

  $data['notifications_subscription_fields']['table']['group'] = t('Notifications Fields');
  $data['notifications_subscription_fields']['table']['join'] = array(
    'users' => array(
      'field' => 'usid',
    ),
    'node' => array(
      'field' => 'value',
      'left_field' => 'nid',
      'extra' => array(
        'field' => 'type',
        'value' => 'node:nid'
      )
    ),
    'notifications_subscription' => array(
      'field' => 'sid',
      'left_field' => 'sid',
    ),
  );

  // Relationships between subscriptions and nodes.
  $data['notifications_subscription_fields']['subscription_to_node'] = array(
    'title' => t('Content'),
    'help' => t('Content that is associated with this subscription.'),
    'real field' => 'value',
    'relationship' => array(
      'title' => t('Content'),
      'label' => t('Content'),
      'base' => 'node',
      'base field' => 'nid',
      'relationship field' => 'value',
      'extra' => array(
        array(
          'table' => 'notifications_subscription_fields',
          'field' => 'type',
          'operator' => '=',
          'value' => 'node:nid',
        ),
      ),
    ),
  );

  //filter by the author to which people are subscribed
  $data['notifications_subscription_fields']['author'] = array(
    'title' => t('Subscribed Author'),
    'help' => t('The UID of the subscribed author.'), // The help that appears on the UI,
    // Information for accepting an author as an argument
    'argument' => array(
      'real field' => 'value',
      'handler' => 'views_handler_argument_user_uid',
      'parent' => 'views_handler_argument_numeric', // make sure parent is included
      // 'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'extra' => array(
        array('field' => 'field', 'value' => 'author'),
      ),
    ),
    //link to the author
    'relationship' => array(
      'base' => 'users',
      'real field' => 'value',
      'handler' => 'notifications_views_handler_relationship',
      'label' => t('Author'),
      'extra' => array(
        array('field' => 'field', 'value' => 'author', 'table' => 'notifications_subscription_fields'),
      ),
    ),
  );

  // Relationships between subscriptions and taxonomy terms.
  $data['notifications_subscription_fields']['subscription_to_term'] = array(
    'title' => t('Taxonomy'),
    'help' => t('Taxonomy term that is associated with this subscription.'),
    'real field' => 'value',
    'relationship' => array(
      'title' => t('Taxonomy term'),
      'label' => t('Taxonomy term'),
      'base' => 'taxonomy_term_data',
      'base field' => 'tid',
      'relationship field' => 'value',
      'extra' => array(
        array(
          'table' => 'notifications_subscription_fields',
          'field' => 'type',
          'operator' => '=',
          'value' => 'term:tid',
        ),
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function notifications_views_views_data_alter(&$data) {
  $data['users']['name2'] = array(
    'title' => t('User name autocomplete filter'),
    'real field' => 'name',
    'help' => t('Filters by first part of username, used for autocompleting users.'),
    'argument' => array(
      'handler' => 'notifications_views_handler_argument_autocomplete',
    ),
  );
}
