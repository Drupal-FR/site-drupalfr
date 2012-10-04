<?php

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
  $data = array();
  // ----------------------------------------------------------------
  // Fields

/* ------------ Definitions for Simplenews mailspool ----------------------*/

  $data['simplenews_mail_spool']['table'] = array(
      'base' => array(
        'field' => 'msid',
        'title' => t('Simplenews mailspool'),
        'help' => t('Spool for temporary storage of newsletter emails.'),
        'weight' => 10,
        'database' => 'default',
      ),
      'group' => t('Simplenews spool'),
  );

  $data['simplenews_mail_spool']['msid'] = array(
      'title' => t('Ms ID'),
      'help' => t('The primary identifier for a mail spool record.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );

  $data['simplenews_mail_spool']['mail'] = array(
      'title' => t('E-Mail'),
      'help' => t('The formatted email address of mail message receipient.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );

  $data['simplenews_mail_spool']['nid'] = array(
      'title' => t('Node ID'),
      'help' => t('The {node}.nid of this newsletter.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'node',
        'base field' => 'nid',
        'label' => t('Node'),
      ),
    );

  $data['simplenews_mail_spool']['vid'] = array(
      'title' => t('Vid'),
      'help' => t('The {node}.vid of this newsletter.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );

  $data['simplenews_mail_spool']['tid'] = array(
      'title' => t('Term ID'),
      'help' => t('The {term_data}.tid this newsletter issue belongs to.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'term_data',
        'base field' => 'tid',
        'label' => t('Issue'),
      ),
    );

  $data['simplenews_mail_spool']['snid'] = array(
      'title' => t('Subscriber ID'),
      'help' => t('The {simplenews_subscriptions}.snid (=Subscriber) foreign key for this spool'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'simplenews_subscriptions',
        'base field' => 'snid',
        'label' => t('Subscriber'),
      ),
    );

  $data['simplenews_mail_spool']['status'] = array(
      'title' => t('Sent status'),
      'help' => t('The sent status of the email (0 = hold, 1 = pending, 2 = done).'),
      'field' => array(
        'handler' => 'simplenews_handler_field_newsletter_s_status',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'simplenews_handler_filter_newsletter_s_status',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );

  $data['simplenews_mail_spool']['error'] = array(
      'title' => t('Error'),
      'help' => t('A boolean indicating whether an error occured while sending the email.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );

  $data['simplenews_mail_spool']['timestamp'] = array(
      'title' => t('Timestamp'),
      'help' => t('The time status was set or changed.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
    );

/* ------------ Definitions for Simplenews issue ----------------------*/
  $data['simplenews_newsletters']['table']['group']  = t('Simplenews issue');

  // joins
  $data['simplenews_newsletters']['table']['join'] = array(
    //...to the node table
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['simplenews_subscriptions']['table']['join'] = array(
    //...to the users table
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['simplenews_newsletters']['tid'] = array(
      'title' => t('Term ID'),
      'help' => t('The {term_data}.tid (= newsletter series) this issue belongs to.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'term_data',
        'base field' => 'tid',
        'label' => t('Issue'),
      ),
  );

  // newsletter sent status (original file version)
  $data['simplenews_newsletters']['s_status'] = array(
    'title' => t('Sent status'), // The item it appears as on the UI,
    'help' => t('Newsletter sent status: 0: Not sent, 1: Pending (being sent or waiting for cron to run), 2: Sent.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'simplenews_handler_field_newsletter_s_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'simplenews_handler_filter_newsletter_s_status',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['simplenews_newsletters']['s_format'] = array(
      'title' => t('Format'),
      'help' => t('Format of the newsletter (plain or html).'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_newsletters']['priority'] = array(
      'title' => t('Priority'),
      'help' => t('Email priority according to RFC 2156 and RFC 5231 (0 = none; 1 = highest; 2 = high; 3 = normal; 4 = low; 5 = lowest).'),
      'field' => array(
        'handler' => 'simplenews_handler_field_newsletter_priority',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'simplenews_handler_filter_newsletter_priority',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_newsletters']['receipt'] = array(
      'title' => t('Receipt'),
      'help' => t('Boolean indicating request for email receipt confirmation according to RFC 2822.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_newsletters']['sent_subscriber_count'] = array(
      'title' => t('Subscriber count'),
      'help' => t('The count of subscribers of the newsletter at the time it was sent.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

/* ------------ Definitions for Simplenews subscriber ----------------------*/

  $data['simplenews_subscriptions']['table'] = array(
      'base' => array(
        'field' => 'snid',
        'title' => t('Simplenews subscriber'),
        'help' => t('Contains subscribers of Simplenews Newsletters.'),
        'weight' => 10,
        'database' => 'default',
      ),
      'group' => t('Simplenews subscriber'),
  );

  $data['simplenews_subscriptions']['snid'] = array(
      'title' => t('Subscriber ID'),
      'help' => t('Primary key: Unique subsciber ID.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_subscriptions']['activated'] = array(
      'title' => t('Activated'),
      'help' => t('Boolean indicating the status of the subscription.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_subscriptions']['mail'] = array(
      'title' => t('Subscriber'),
      'help' => t('The subscription email address.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_subscriptions']['uid'] = array(
      'title' => t('User'),
      'help' => t('The {users}.uid that has the same email address.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'base field' => 'uid',
        'label' => t('User'),
      ),
  );

  $data['simplenews_subscriptions']['language'] = array(
      'title' => t('Language'),
      'help' => t('Anonymous subscriber preferred language. Empty for authenticated users.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_subscriptions']['timestamp'] = array(
      'title' => t('Timestamp'),
      'help' => t('UNIX timestamp of when the user first subscribed to a newsletter.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
  );

/* ------------ Definitions for Simplenews subscription ----------------------*/

  $data['simplenews_snid_tid']['table'] = array(
    'base' => array(
       'field' => 'snid',
       'title' => t('Simplenews subscription'),
       'help' => t('Contains all Subscriptions of every Simplenews Newsletters.'),
       'weight' => 10,
       'database' => 'default',
     ),
     'group' => t('Simplenews subscription'),
  );

  $data['simplenews_snid_tid']['table']['join'] = array(
    'term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  $data['simplenews_snid_tid']['table']['join'] = array(
    'simplenews_subscriptions' => array(
      'left_field' => 'snid',
      'field' => 'snid',
    ),
  );

  $data['simplenews_snid_tid']['snid'] = array(
      'title' => t('Subscriber ID'),
      'help' => t('The {simplenews_subscriptions}.snid who is subscribed.'),
      'field' => array(
        'label' => 'TEST',
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'simplenews_subscriptions',
        'base field' => 'snid',
        'label' => t('Subscriber'),
      ),
  );

  $data['simplenews_snid_tid']['tid'] = array(
      'title' => t('Term ID'),
      'help' => t('The newsletter series ({term_data}.tid) the subscriber is subscribed to.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'term_data',
        'base field' => 'tid',
        'label' => t('Term'),
      ),
  );

  $data['simplenews_snid_tid']['status'] = array(
      'title' => t('Status'),
      'help' => t('A flag indicating whether the user is subscribed (1) or unsubscribed (0).'),
      'field' => array(
        'handler' => 'simplenews_handler_field_simplenews_snid_tid_status',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'simplenews_handler_filter_simplenews_snid_tid_status',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_snid_tid']['timestamp'] = array(
      'title' => t('Timestamp'),
      'help' => t('UNIX timestamp of when the user is (un)subscribed.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_date',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  $data['simplenews_snid_tid']['source'] = array(
      'title' => t('Source'),
      'help' => t('The source via which the user is (un)subscription.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function simplenews_views_data_alter(&$data) {
/*
  // joins
  $data['node']['table']['join']['simplenews_newsletter'] = array(
    //...to the simplenews_newsletter table
    'left_field' => 'nid',
    'field' => 'nid',
  );
*/
}

/**
 * Implements hook_views_handlers().
 */
function simplenews_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'simplenews') . '/includes/views',
    ),
    'handlers' => array(
      'simplenews_handler_field_newsletter_s_status' => array(
        'parent' => 'views_handler_field',
      ),
      'simplenews_handler_filter_newsletter_s_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'simplenews_handler_field_newsletter_priority' => array(
        'parent' => 'views_handler_field',
      ),
      'simplenews_handler_filter_newsletter_priority' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'simplenews_handler_field_simplenews_snid_tid_status' => array(
        'parent' => 'views_handler_field',
      ),
      'simplenews_handler_filter_simplenews_snid_tid_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
