<?php
// $Id: simplenews.admin.inc,v 1.5.2.54 2010/04/19 08:14:29 sutharsan Exp $

/**
 * @file
 * Newsletter admin, subscription admin, simplenews settings
 *
 * @ingroup simplenews
 */

/**
 * Menu callback: Admin form for sent and draft newsletters.
 *
 * @see simplenews_admin_news_submit()
 */
function simplenews_admin_news(&$form_state, $action = 'sent') {
  $vid = variable_get('simplenews_vid', '');
  $form = array();
  $form['simplenews_issue_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show issues from'),
    '#collapsible' => FALSE,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $header = array(t('Title'), t('Newsletter'), t('Date created'), t('Published'), t('Sent'), t('Edit'));
  list($names, $queries) = array_values(simplenews_get_filter('simplenews_newsletters_filter'));
  $filter = isset($_SESSION['simplenews_newsletters_filter']) ? $_SESSION['simplenews_newsletters_filter'] : '';
  $queryfilter = isset($queries[$filter]) ? $queries[$filter] : '';
  if ($action == 'notsent') {
    $form['simplenews_issue_filter']['filter'] = array(
      '#type' => 'select',
      '#options' => $names,
      '#default_value' => $filter,
    );
    $query = "SELECT DISTINCT n.*, s.s_status FROM {node} n INNER JOIN {simplenews_newsletters} s ON n.nid = s.nid WHERE s.s_status = %d". $queryfilter ." ORDER BY n.created DESC";
    $count_query = "SELECT COUNT(n.nid) FROM {node} n INNER JOIN {simplenews_newsletters} s ON n.nid = s.nid WHERE s.s_status = %d". $queryfilter;
  }
  else {
    $form['simplenews_issue_filter']['filter'] = array(
      '#type' => 'select',
      '#options' => $names,
      '#default_value' => $filter,
    );
    $query = "SELECT n.*, s.s_status FROM {node} n INNER JOIN {simplenews_newsletters} s ON n.nid = s.nid WHERE s.s_status > %d". $queryfilter ." ORDER BY n.created DESC";
    $count_query = "SELECT COUNT(n.nid) FROM {node} n INNER JOIN {simplenews_newsletters} s ON n.nid = s.nid WHERE s.s_status > %d". $queryfilter;
  }
  $result = pager_query(db_rewrite_sql($query, 'n', 'nid', array(SIMPLENEWS_STATUS_SEND_NOT)), 10, 0, db_rewrite_sql($count_query, 'n', 'nid', array(SIMPLENEWS_STATUS_SEND_NOT)), 0);
  $rows = array();
  while ($node = db_fetch_object($result)) {
    $terms = array_keys(taxonomy_node_get_terms_by_vocabulary($node, $vid, 'name'));
    $rows[] = array(l($node->title, 'node/'. $node->nid),
                    isset($terms[0]) ? $terms[0] : t('n/a'),
                    format_date($node->created, 'small'),
                    theme('simplenews_status', $node->status, 'published'),
                    theme('simplenews_status', $node->s_status, 'sent'),
                    l(t('edit'), 'node/'. $node->nid .'/edit', array(), drupal_get_destination()));
  }
  if ($pager = theme('pager', NULL, 10, 0)) {
    $rows[] = array(array('data' => $pager, 'colspan' => '6'));
  }
  if (!$rows) {
    $rows[] = array(array('data' => t('No newsletters available.'), 'colspan' => '6'));
  }
  $form['simplenews_issue_filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $table = theme('table', $header, $rows);

  $form['table'] = array('#value' => $table);
  return $form;
}

function simplenews_admin_news_submit($form, &$form_state) {
  _simplenews_set_filter('simplenews_newsletters_filter', $form_state['values']);
}

/**
 * Store filter values in session var
 *
 * @param $type identification string
 * @param $form_values array of values to be stored
 */
function _simplenews_set_filter($type, $values) {
  if (empty($_SESSION[$type])) {
    $_SESSION[$type] = 'all';
  }
  $op = $values['op'];
  if ($op == t('Filter') && isset($values['filter'])) {
    $_SESSION[$type] = $values['filter'];
  }
}

/**
 * Menu callback: newsletter admin form with list of available newsletter series.
 */
function simplenews_types_overview() {
  $rows = array();
  $header = array(t('Newsletter name'), t('Subscribers'), t('Operations'));

  foreach (taxonomy_get_tree(variable_get('simplenews_vid', '')) as $term) {
    $count_result = db_query('SELECT COUNT(s.snid) AS s_count FROM {simplenews_subscriptions} s INNER JOIN {simplenews_snid_tid} t ON s.snid = t.snid WHERE s.activated = %d AND t.tid = %d', 1, $term->tid);
    $count = (int) db_result($count_result);
    $rows[] = array(check_plain($term->name), $count, l(t('edit newsletter'), 'admin/content/simplenews/types/edit/'. $term->tid));
  }

  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('There are currently no newsletter series.'), 'colspan' => count($header)));
  }
  return theme('table', $header, $rows);
}

/**
 * Menu callback: newsletter admin form for newsletter add/edit.
 *
 * @see simplenews_admin_types_form_validate()
 * @see simplenews_admin_types_form_submit()
 */
function simplenews_admin_types_form(&$form_state, $tid = NULL) {
  if (isset($tid)) {
    // We are editing an existing newsletter.
    $term = taxonomy_get_term($tid);

    // If the given term is not a newsletter, don't allow editing.
    if (variable_get('simplenews_vid', '') != $term->vid) {
      drupal_not_found();
      return;
    }

    // Redirect on a delete operation for posterity's sake.
    if (isset($form_state['post']['op']) && $form_state['post']['op'] == t('Delete')) {
      drupal_goto('admin/content/simplenews/types/delete/'. $term->tid);
    }

  }
  else {
    // We will create a new newsletter.
    // Provide vocabulary id for the newsletter to be added to.
    $term = (object)array('vid' => variable_get('simplenews_vid', ''));
    $term->tid = '';
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Newsletter name'),
    '#size' => 50,
    '#maxlength' => 128,
    '#description' => t('The name of this newsletter.'),
    '#required' => TRUE,
    '#default_value' => isset($term->name) ? $term->name : '',
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#cols' => 60,
    '#rows' => 5,
    '#description' => t('A description of the newsletter. To be displayed on taxonomy/term pages and RSS feeds.'),
    '#default_value' => isset($term->description) ? $term->description : '',
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 10,
    '#description' => t('Newsletters are displayed in ascending order by weight.'),
    '#required' => TRUE,
    '#default_value' => isset($term->weight) ? $term->weight : 0,
  );

  $form['simplenews_sender_information'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sender information'),
    '#collapsible' => FALSE,
  );
  $form['simplenews_sender_information']['simplenews_from_name_'. $term->tid] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('simplenews_from_name_'. $term->tid, variable_get('simplenews_from_name', variable_get('site_name', 'Drupal'))),
  );
  $form['simplenews_sender_information']['simplenews_from_address_'. $term->tid] = array(
    '#type' => 'textfield',
    '#title' => t('From email address'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => variable_get('simplenews_from_address_'. $term->tid, variable_get('simplenews_from_address', variable_get('site_mail', ini_get('sendmail_from')))),
  );
  $form['simplenews_hyperlinks'] = array('#type' => 'fieldset',
    '#title' => t('HTML to text conversion'),
    '#collapsible' => FALSE,
    '#description' => t('When your newsletter is sent as plain text, these options will determine how the conversion to text is performed.'),
  );
  $form['simplenews_hyperlinks']['simplenews_hyperlinks_'. $term->tid] = array(
    '#type' => 'radios',
    '#title' => t('Hyperlink conversion'),
    '#options' => array(
      t('Append hyperlinks as a numbered reference list'),
      t('Display hyperlinks inline with the text')
    ),
    '#default_value' => variable_get('simplenews_hyperlinks_'. $term->tid, 1),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 25,
  );
  $form['vid'] = array(
    '#type' => 'hidden',
    '#value' => isset($term->vid) ? $term->vid : variable_get('simplenews_vid', ''),
  );

  // If we are editing a newsletter term, show delete option and store tid.
  // tid is used in validation and submission functions to differentiate
  // between form actions of creating a new or editing an existing term.
  if ($term->tid) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 30,
    );
    $form['tid'] = array(
      '#type' => 'value',
      '#value' => $term->tid,
    );
  }

  // Return to Newsletter list page after submit
  $form['#redirect'] = 'admin/content/simplenews/types';

  return $form;
}

function simplenews_admin_types_form_validate($form, &$form_state) {
  $tid = $form_state['values']['tid'];
  $field_name = 'simplenews_from_address_'. $tid;

  if (!valid_email_address($form_state['values'][$field_name])) {
    form_set_error($field_name, t("The sender's email address you supplied is not valid."));
  }
}

function simplenews_admin_types_form_submit($form, &$form_state) {
    // Redirect on a delete operation for posterity's sake.
    if (isset($form_state['post']['op']) && $form_state['post']['op'] == t('Delete')) {
      drupal_goto('admin/content/simplenews/types/delete/'. $term->tid);
    }

  // Create or update taxonomy term for this newsletter.
  // When creating a term $form_state['values']['tid'] is NULL.
  if (isset($form_state['values']['tid'])) {
    $termdata = (array)taxonomy_get_term($form_state['values']['tid']);
  }
  $termdata['tid'] = $form_state['values']['tid'];
  $termdata['vid'] = $form_state['values']['vid'];
  $termdata['name'] = $form_state['values']['name'];
  $termdata['description'] = $form_state['values']['description'];
  $termdata['weight'] = $form_state['values']['weight'];
  switch (taxonomy_save_term($termdata)) {
    case SAVED_NEW:
      drupal_set_message(t('Created new term %name.', array('%name' => $form_state['values']['name'])));
      break;
    case SAVED_UPDATED:
      drupal_set_message(t('Updated term %name.', array('%name' => $form_state['values']['name'])));
      break;
  }

  // Store newsletter specific settings.
  // When editing a newsletter fieldnames have $tid suffix.
  // When creating a new newsletter field names have no suffix
  // and $form_state['values']['tid'] is NULL.
  if (isset($form_state['values']['tid'])) {
    $tid = $suffix = $form_state['values']['tid'];
  }
  else {
    // Get the tid from the database based on the form values used above to
    // create a new term.
    $tid = db_result(db_query("SELECT t.tid FROM {term_data} t WHERE t.vid = %d AND t.name = '%s' ORDER BY t.tid DESC", $form_state['values']['vid'], $form_state['values']['name']));
    $suffix = '';
  }
  variable_set('simplenews_from_name_'. $tid, $form_state['values']['simplenews_from_name_'. $suffix]);
  variable_set('simplenews_from_address_'. $tid, $form_state['values']['simplenews_from_address_'. $suffix]);
  variable_set('simplenews_hyperlinks_'. $tid, $form_state['values']['simplenews_hyperlinks_'. $suffix]);
}

/**
 * Menu callback: Delete newsletter series.
 *
 * @see simplenews_admin_types_delete_submit()
 */
function simplenews_admin_types_delete(&$form_state, $tid = NULL) {
  if (!isset($tid)) {
    drupal_not_found();
    return;
  }
  $term = taxonomy_get_term($tid);
  $form = array();
  $form['tid'] = array(
    '#type' => 'value',
    '#value' => $tid,
  );
  $form['notice'] = array(
    '#value' => '<p><strong>'. t('Note: All subscriptions associated with this newsletter will be lost.') .'</strong></p>',
  );
  $form['#redirect'] = 'admin/content/simplenews/types';

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $term->name)),
    'admin/content/simplenews/types',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function simplenews_admin_types_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $tid = $form_state['values']['tid'];
    $term = taxonomy_get_term($tid);
    taxonomy_del_term($tid);
    variable_del('simplenews_from_name_'. $tid);
    variable_del('simplenews_from_address_'. $tid);
    drupal_set_message(t('Newsletter %title has been deleted.', array('%title' => $term->name)));
  }
}

/**
 * Menu callback: Delete multiple subscriptions.
 *
 * @see simplenews_subscription_multiple_delete_confirm_submit()
 */
function simplenews_subscription_multiple_delete_confirm() {
  // Subscriptions to be deleted are passed via session var.
  $snids = $_SESSION['simplenews_subscriptions_delete'];

  $form['snids'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach (array_filter($snids) as $snid => $value) {
    $mail = db_result(db_query('SELECT mail FROM {simplenews_subscriptions} WHERE snid = %d', $snid));
    $form['snids'][$snid] = array(
      '#type' => 'hidden',
      '#value' => $snid,
      '#prefix' => '<li>',
      '#suffix' => check_plain($mail) ."</li>\n"
    );
  }

  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => 'delete',
  );
  $form['#submit'][] = 'simplenews_subscription_multiple_delete_confirm_submit';

  return confirm_form($form,
                      t('Are you sure you want to delete these subscriptions?'),
                      'admin/content/simplenews/users',
                      t('This action cannot be undone.'),
                      t('Delete all'),
                      t('Cancel')
                      );
}

function simplenews_subscription_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['snids'] as $snid => $value) {
      simplenews_delete_subscription($snid);
    }
    drupal_set_message(t('The subscriptions have been deleted.'));
  }
  return '';
}

/**
 * Menu callback: Mass subscribe to newsletters.
 *
 * @see simplenews_subscription_list_add_submit()
 */
function simplenews_subscription_list_add() {
  global $language;

  $form['emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Email addresses'),
    '#cols' => 60,
    '#rows' => 5,
    '#description' => t('Email addresses must be separated by comma, space or newline.'),
  );
  $newsletters = array();
  foreach (taxonomy_get_tree(variable_get('simplenews_vid', '')) as $newsletter) {
    $newsletters[$newsletter->tid] = check_plain($newsletter->name);
  }
  $form['newsletters'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Subscribe to'),
    '#options' => $newsletters,
    '#required' => TRUE,
  );

  // Include language selection when the site is multilingual.
  // Default value is the empty string which will result in receiving emails
  // in the site's default language.
  if (variable_get('language_count', 1) > 1) {
    $options[''] = t('Site default language');
    $languages = language_list('enabled');
    foreach ($languages[1] as $langcode => $item) {
      $name = t($item->name);
      $options[$langcode] = $name . ($item->native != $name ? ' ('. $item->native .')' : '');
    }
    $form['language'] = array(
      '#type' => 'radios',
      '#title' => t('Anonymous user preferred language'),
      '#default_value' => '',
      '#options' => $options,
      '#description' => t('Anonymous users will be subscribed with the selected preferred language. They will receive newsletters in this language if available. Registered users will be subscribed with their preferred language as set on their account page.'),
    );
  }
  else {
    $form['language'] = array('#type' => 'value', '#value' => '');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe'),
  );
  return $form;
}

function simplenews_subscription_list_add_submit($form, &$form_state) {
  $tree = taxonomy_get_tree(variable_get('simplenews_vid', ''));
  $added = array();
  $invalid = array();
  $checked_newsletters = array_filter($form_state['values']['newsletters']);
  $langcode = $form_state['values']['language'];

  $emails = preg_split("/[\s,]+/", $form_state['values']['emails']);
  foreach ($emails as $email) {
    $email = trim($email);
    if (valid_email_address($email)) {
      foreach ($checked_newsletters as $tid) {
        $newsletter = taxonomy_get_term($tid);
        simplenews_subscribe_user($email, $newsletter->tid, FALSE, $langcode);
        $added[] = $email;
      }
    }
    else {
      $invalid[] = $email;
    }
  }
  if ($added) {
    $added = implode(", ", $added);
    drupal_set_message(t('The following addresses were added or updated: %added.', array('%added' => $added)));

    $newsletter_names = array();
    foreach ($checked_newsletters as $tid) {
      $newsletter = taxonomy_get_term($tid);
      $newsletter_names[] = $newsletter->name;
    }
    drupal_set_message(t('The addresses were subscribed to the following newsletters: %newsletters.', array('%newsletters' => implode(', ', $newsletter_names))));
  }
  else {
    drupal_set_message(t('No addresses were added.'));
  }
  if ($invalid) {
    $invalid = implode(", ", $invalid);
    drupal_set_message(t('The following addresses were invalid: %invalid.', array('%invalid' => $invalid)), 'error');
  }
}

/**
 * Menu callback: Export email address of subscriptions.
 *
 * @see simplenews_admin_export_after_build()
 */
function simplenews_subscription_list_export() {
  $form['states'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Status'),
    '#options' => array('active' => t('Active users'), 'inactive' => t('Inactive users')),
    '#description' => t('Subscriptions matching the selected states will be exported.'),
    '#required' => TRUE,
  );
  $newsletters = array();
  foreach (taxonomy_get_tree(variable_get('simplenews_vid', '')) as $newsletter) {
    $newsletters[$newsletter->tid] = check_plain($newsletter->name);
  }
  $form['newsletters'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Subscribed to'),
    '#options' => $newsletters,
    '#description' => t('Subscriptions matching the selected newsletters will be exported.'),
    '#required' => TRUE,
  );

  // Emails item is initially empty. It serves as place holder. Data is added by
  // simplenews_admin_export_after_build().
  $form['emails'] = array();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );
  $form['#after_build'] = array('simplenews_admin_export_after_build');
  $form['#redirect'] = FALSE;
  return $form;
}

function simplenews_admin_export_after_build($form, $form_element) {
  if (isset($form_element['values']['op']) && $form_element['values']['op'] == t('Export')) {
    $states = array_filter($form_element['values']['states']);
    $newsletters = array_filter($form_element['values']['newsletters']);

    // Build where clause for active/inactive state and newsletter selection.
    if (isset($states['active'])) {
      $where[] = 's.activated = 1';
    }
    if (isset($states['inactive'])) {
      $where[] = 's.activated = 0';
    }
    $where = isset($where) ? implode(' OR ', $where) : NULL;
    if ($tree = taxonomy_get_tree(variable_get('simplenews_vid', ''))) {
      foreach ($tree as $newsletter) {
        if (isset($newsletters[$newsletter->tid])) {
          $where_tid[] = 't.tid = '. $newsletter->tid;
        }
      }
    }
    $where_tid = isset($where_tid) ? implode(' OR ', $where_tid) : NULL;

    // Get subscription data
    if (isset($where) && isset($where_tid)) {
      $query = 'SELECT DISTINCT s.mail FROM {simplenews_subscriptions} s INNER JOIN {simplenews_snid_tid} t ON s.snid = t.snid WHERE ('. $where .') AND ('. $where_tid .')';
      $result = db_query($query);
      while ($mail = db_fetch_object($result)) {
        $mails[] = $mail->mail;
      }
    }

    // Build form field containing exported emails.
    // The field will be included in the form where at the ['emails'] place holder.
    if (isset($mails)) {
      $exported_mails = implode(", ", $mails);
    }
    else {
      $exported_mails = t('No addresses were found.');
    }
    $form['emails'] = array(
      '#type' => 'textarea',
      '#title' => t('Export results'),
      '#cols' => 60,
      '#rows' => 5,
      '#value' => $exported_mails,
    );
  }
  return $form;
}

/**
 * Menu callback: Mass subscribe to newsletters.
 *
 * @see simplenews_subscription_list_remove_submit()
 */
function simplenews_subscription_list_remove() {
  $form['emails'] = array(
    '#type' => 'textarea',
    '#title' => t('Email addresses'),
    '#cols' => 60,
    '#rows' => 5,
    '#description' => t('Email addresses must be separated by comma, space or newline. Email addresses which are no longer subscribed to any newsletter, will be removed from the database.'),
  );
  $newsletters = array();
  foreach (taxonomy_get_tree(variable_get('simplenews_vid', '')) as $newsletter) {
    $newsletters[$newsletter->tid] = check_plain($newsletter->name);
  }
  $form['newsletters'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Unsubscribe from'),
    '#options' => $newsletters,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unsubscribe'),
  );
  return $form;
}

function simplenews_subscription_list_remove_submit($form, &$form_state) {
  $tree = taxonomy_get_tree(variable_get('simplenews_vid', ''));
  $removed = array();
  $invalid = array();
  $checked_newsletters = array_filter($form_state['values']['newsletters']);

  $emails = preg_split("/[\s,]+/", $form_state['values']['emails']);
  foreach ($emails as $email) {
    $email = trim($email);
    if (valid_email_address($email)) {
      foreach ($checked_newsletters as $tid) {
        $newsletter = taxonomy_get_term($tid);
        simplenews_unsubscribe_user($email, $newsletter->tid, FALSE);
        $removed[] = $email;
      }
    }
    else {
      $invalid[] = $email;
    }
  }
  if ($removed) {
    $removed = implode(", ", $removed);
    drupal_set_message(t('The following addresses were unsubscribed or removed: %removed.', array('%removed' => $removed)));

    $newsletter_names = array();
    foreach ($checked_newsletters as $tid) {
      $newsletter = taxonomy_get_term($tid);
      $newsletter_names[] = $newsletter->name;
    }
    drupal_set_message(t('The addresses were unsubscribed from the following newsletters: %newsletters.', array('%newsletters' => implode(', ', $newsletter_names))));
  }
  else {
    drupal_set_message(t('No addresses were removed.'));
  }
  if ($invalid) {
    $invalid = implode(", ", $invalid);
    drupal_set_message(t('The following addresses were invalid: %invalid.', array('%invalid' => $invalid)), 'error');
  }
}

/**
 * Menu callback: subscription administration.
 */
function simplenews_subscription_admin($form_state) {
  // Delete subscriptions requires delete confirmation. This is handled with a different form
  if (isset($form_state['post']['operation']) && $form_state['post']['operation'] == 'delete' && isset($form_state['post']['snids'])) {
    $destination = drupal_get_destination();
    $_SESSION['simplenews_subscriptions_delete'] = $form_state['post']['snids'];
    // Note: we redirect from admin/content/simplenews/users to admin/content/simplenews/subscriptions/delete to make the tabs disappear.
    drupal_goto("admin/content/simplenews/subscriptions/delete", $destination);
  }

  $form = simplenews_subscription_filter_form();
  $form['admin'] = simplenews_subscription_list_form();

  return $form;
}

/**
 * Build the form for admin subscription.
 *
 * Form consists of a filter fieldset, an operation fieldset and a list of
 * subscriptions maching the filter criteria.
 *
 * @see simplenews_subscription_list_form_validate()
 * @see simplenews_subscription_list_form_submit()
 */
function simplenews_subscription_list_form() {
  // Table header. Used as tablesort default
  $header = array(
    array('data' => t('Email'), 'field' => 'ss.mail', 'sort' => 'asc'),
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Status'), 'field' => 'ss.activated'),
    t('Operations')
  );

  // Data collection with filter and sorting applied
  $filter = simplenews_build_subscription_filter_query();
  $query = 'SELECT DISTINCT ss.*, u.name FROM {simplenews_subscriptions} ss LEFT JOIN {users} u ON ss.uid = u.uid INNER JOIN {simplenews_snid_tid} s ON ss.snid = s.snid'. $filter['where'];

  // $count_query used to count distinct records only
  $count_query = preg_replace('/SELECT.*FROM /', 'SELECT COUNT(DISTINCT ss.mail) FROM ', $query);
  $query .= tablesort_sql($header);
  $result = pager_query($query, 30, 0, $count_query);

  // Update options
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => array(
      'activate' => t('Activate'),
      'inactivate' => t('Inactivate'),
      'delete' => t('Delete'),
    ),
    '#default_value' => 'activate');
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('simplenews_subscription_list_form_submit'),
    '#validate' => array('simplenews_subscription_list_form_validate'),
  );

  $snids = array();
  // Subscription table and table pager
  while ($subscription = db_fetch_object($result)) {
    $snids[$subscription->snid] = '';
    $form['mail'][$subscription->snid] = array('#value' => $subscription->mail);
    $form['name'][$subscription->snid] =  array('#value' => isset($subscription->uid) ? l($subscription->name, 'user/'. $subscription->uid) : $subscription->name);
    $form['status'][$subscription->snid] = array('#value' => theme('simplenews_status', $subscription->activated, 'activated'));
    $form['operations'][$subscription->snid] = array('#value' => l(t('edit'), 'admin/content/simplenews/users/edit/'. $subscription->snid, array(), drupal_get_destination()));
  }
  $form['snids'] = array('#type' => 'checkboxes', '#options' => $snids);
  $form['pager'] = array('#value' => theme('pager', NULL, 30, 0));
  $form['#theme'] = 'simplenews_subscription_list';
  return $form;
}

function simplenews_subscription_list_form_validate($form, &$form_state) {
  if (isset($form_state['values']['operation'])) {
    $snids = array_filter($form_state['values']['snids']);
    if (empty($snids)) {
      form_set_error('', t('No items selected.'));
    }
  }
}

function simplenews_subscription_list_form_submit($form, &$form_state) {
  if (isset($form_state['values']['operation'])) {
    $snids = array_filter($form_state['values']['snids']);
    $args = array($snids);
    switch ($form_state['values']['operation']) {
      case 'activate':
        call_user_func_array('simplenews_activate_subscription', $args);
        drupal_set_message(t('The update has been performed.'));
        break;
      case 'inactivate':
        call_user_func_array('simplenews_inactivate_subscription', $args);
        drupal_set_message(t('The update has been performed.'));
        break;
    }
  }
}

/**
 * Menu callback: Simplenews admin settings - General.
 */
function simplenews_admin_settings(&$form_state) {
  $vid = variable_get('simplenews_vid', '');
  $address_default = variable_get('site_mail', ini_get('sendmail_from'));
  $form = array();

  $form['simplenews_general_settings']['simplenews_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#required' => TRUE,
    '#options' => array_map('check_plain', node_get_types('names')),
    '#default_value' => variable_get('simplenews_content_types', array('simplenews' => 'simplenews')),
    '#description' => t('Content types to be used as newsletter.'),
  );

  $vocabulary_options = array('' => t('<select>'));
  if ($vocabularies = taxonomy_get_vocabularies()) {
    foreach ($vocabularies as $key => $object) {
      $vocabulary_options[$key] = $object->name;
    }
  }
  $form['simplenews_general_settings']['simplenews_vid'] = array(
    '#type' => 'select',
    '#title' => t('Newsletter vocabulary'),
    '#options' =>  $vocabulary_options,
    '#required' => TRUE,
    '#description' => t('This vocabulary identifies newsletter series. Note that changing this vocabulary will require existing newsletters to be re-tagged and activated newsletter blocks to be re-activated.'),
    '#default_value' => variable_get('simplenews_vid', ''),
  );

  // Node type settings require pre-processing.
  // simplenews_admin_settings_submit() is called before system_settings_form_submit().
  $form['#submit'][] = 'simplenews_admin_settings_submit';
  return system_settings_form($form);
}

function simplenews_admin_settings_submit($form, &$form_state) {
  // Newsletter vocabulary is updated using node type settings.
  $vocabulary = (array)taxonomy_vocabulary_load($form_state['values']['simplenews_vid']);
  $form_state['values']['simplenews_content_types'] = array_filter($form_state['values']['simplenews_content_types']);
  $vocabulary['nodes'] = $form_state['values']['simplenews_content_types'];

  // Set vocabulary required values
  $vocabulary['multiple'] = FALSE;
  $vocabulary['required'] = TRUE;
  $vocabulary['tags'] = FALSE;

  taxonomy_save_vocabulary($vocabulary);
  variable_set('simplenews_content_types', $form_state['values']['simplenews_content_types']);

  // We do not need a simplenews_content_types variable.
  unset($form_state['values']['simplenews_content_types']);

  drupal_set_message(t('Updated vocabulary %name.', array('%name' => $vocabulary['name'])));
  watchdog('simplenews', 'Updated vocabulary %name.', array('%name' => $vocabulary['name']), WATCHDOG_NOTICE, l(t('edit'), 'admin/content/taxonomy/edit/vocabulary/'. $form_state['values']['vid']));
}

/**
 * Menu callback: Simplenews admin settings - Newsletter.
 */
function simplenews_admin_settings_newsletter(&$form_state) {
  $vid = variable_get('simplenews_vid', '');
  $address_default = variable_get('site_mail', ini_get('sendmail_from'));
  $form = array();

  $form['simplenews_default_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default newsletter options'),
    '#collapsible' => FALSE,
    '#description' => t('These options will be the defaults for new newsletters, but can be overridden in the newsletter editing form.'),
  );
  $description = t('Default newsletter format.');
  if (!module_exists('mimemail')) {
        $description .= ' '. t('Install <a href="@mime_mail_url">Mime Mail</a> module to send newsletters in HTML format.', array('@mime_mail_url' => 'http://drupal.org/project/mimemail'));
  }
  $form['simplenews_default_options']['simplenews_format'] = array(
    '#type' => 'select',
    '#title' => t('Format'),
    '#options' => _simplenews_format_options(),
    '#description' => $description,
    '#default_value' => variable_get('simplenews_format', 'plain'),
  );
  $form['simplenews_default_options']['simplenews_priority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#options' => array(0 => t('none'), 1 => t('highest'), 2 => t('high'), 3 => t('normal'), 4 => t('low'), 5 => t('lowest')),
    '#description' => t('Note that email priority is ignored by a lot of email programs.'),
    '#default_value' => variable_get('simplenews_priority', 0),
  );
  $form['simplenews_default_options']['simplenews_receipt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Request receipt'),
    '#default_value' => variable_get('simplenews_receipt', 0),
    '#description' => t('Request a Read Receipt from your newsletters. A lot of email programs ignore these so it is not a definitive indication of how many people have read your newsletter.'),
  );
  $form['simplenews_default_options']['simplenews_send'] = array(
    '#type' => 'radios',
    '#title' => t('Default send action'),
    '#options' => array(0 => t("Don't send now"), 2 => t('Send one test newsletter to the test address'), 1 => t('Send newsletter')),
    '#default_value' => variable_get('simplenews_send', 0),
  );
  $form['simplenews_test_address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test addresses'),
    '#collapsible' => FALSE,
    '#description' => t('Supply a comma-separated list of email addresses to be used as test addresses. The override function allows to override these addresses in the newsletter editing form.'),
  );
  $form['simplenews_test_address']['simplenews_test_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('simplenews_test_address', $address_default),
  );
  $form['simplenews_test_address']['simplenews_test_address_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow test address override'),
    '#default_value' => variable_get('simplenews_test_address_override', 0),
  );
  $form['simplenews_sender_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sender information'),
    '#collapsible' => FALSE,
    '#description' => t('Default sender address that will only be used for confirmation emails. You can specify sender information for each newsletter separately on the newsletter\'s settings page.'),
  );
  $form['simplenews_sender_info']['simplenews_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#size' => 60,
    '#maxlength' => 128,
    '#default_value' => variable_get('simplenews_from_name', variable_get('site_name', 'Drupal')),
  );
  $form['simplenews_sender_info']['simplenews_from_address'] = array(
    '#type' => 'textfield',
    '#title' => t('From email address'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => variable_get('simplenews_from_address', $address_default),
  );

  return system_settings_form($form);
}

function simplenews_admin_settings_newsletter_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['simplenews_from_address'])) {
    form_set_error($field_name, t("The sender's email address you supplied is not valid."));
  }
}

/**
 * Menu callback: Simplenews admin settings - Email.
 */
function simplenews_admin_settings_mail(&$form_state) {
  $vid = variable_get('simplenews_vid', '');
  $address_default = variable_get('site_mail', ini_get('sendmail_from'));
  $form = array();

  $form['simplenews_mail_backend']['simplenews_use_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cron to send newsletters'),
    '#default_value' => variable_get('simplenews_use_cron', TRUE),
    '#description' => t('When checked cron will be used to send newsletters (recommended). Test newsletters and confirmation emails will be sent immediately. Leave unchecked for testing purposes.'),
  );
  $throttle = drupal_map_assoc(array(1, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000));
  $throttle[999999] = t('Unlimited');
  if (function_exists('getrusage')) {
    $description_extra = '<br />'. t('Cron execution must not exceed the PHP maximum execution time of %max seconds. You find the time spend to send emails in the !recent_logs.', array('%max' => ini_get('max_execution_time'), '!recent_logs' => l(t('Recent log entries'), 'admin/reports/dblog')));
  }
  else {
    $description_extra = '<br />'. t('Cron execution must not exceed the PHP maximum execution time of %max seconds.', array('%max' => ini_get('max_execution_time')));
  }
  $form['simplenews_mail_backend']['simplenews_throttle'] = array(
    '#type' => 'select',
    '#title' => t('Cron throttle'),
    '#options' => $throttle,
    '#default_value' => variable_get('simplenews_throttle', 20),
    '#description' => t('Sets the numbers of newsletters sent per cron run. Failure to send will also be counted.') . $description_extra,
  );
  $form['simplenews_mail_backend']['simplenews_spool_expire'] = array(
    '#type' => 'select',
    '#title' => t('Mail spool expiration'),
    '#options' => array(0 => t('Immediate'), 1 => t('1 day'), 7 => t('1 week'), 14 => t('2 weeks')),
    '#default_value' => variable_get('simplenews_spool_expire', 0),
    '#description' => t('Newsletter mails are spooled. How long must messages be retained in the spool after successfull sending. Keeping the message in the spool allows mail statistics (which is not yet implemented). If cron is not used, immediate expiration is advised.'),
  );
  $form['simplenews_mail_backend']['simplenews_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log emails'),
    '#default_value' => variable_get('simplenews_debug', FALSE),
    '#description' => t('When checked all outgoing simplenews emails are logged in the system log. A logged email does not guarantee that it is send or will be delivered. It only indicates that a message is send to the PHP mail() function. No status information is available of delivery by the PHP mail() function.'),
  );

  return system_settings_form($form);
}

/**
 * Menu callback: Simplenews admin settings - Subscription.
 */
function simplenews_admin_settings_subscription(&$form_state) {
  $vid = variable_get('simplenews_vid', '');
  $address_default = variable_get('site_mail', ini_get('sendmail_from'));
  $form = array();

  $form['simplenews_subscription']['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('User account'),
    '#collapsible' => FALSE,
  );
  $form['simplenews_subscription']['account']['simplenews_sync_account'] = array(
    '#type' => 'checkbox',
    '#title' => t('Synchronize with account'),
    '#default_value' => variable_get('simplenews_sync_account', TRUE),
    '#description' => t('When checked subscriptions will be synchronized with site accounts. When accounts are deleted, subscriptions with the same email address will be removed. When site accounts are blocked/unblocked, subscriptions will be deactivated/activated. When not checked subscriptions will be unchanged when associated accounts are deleted or blocked.'),
  );

  // These email tokens are shared for all settings, so just define
  // the list once to help ensure they stay in sync.
  $email_token_help = t('Available variables are:') .' !site, !uri, !uri_brief, !mailto, !date, !newsletter_name, !confirm_subscribe_url, !confirm_unsubscribe_url.';

  $form['simplenews_subscription']['subscription_mail'] = array(
    '#type' => 'fieldset',
    '#title' => t('Confirmation emails'),
    '#collapsible' => FALSE,
    '#description' => t('Customize the confimation email which is sent to (un)subscribing visitors.') .' '. $email_token_help,
  );
  $form['simplenews_subscription']['subscription_mail']['simplenews_confirm_subscribe_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => _simplenews_subscription_confirmation_text('subscribe_subject'),
    '#maxlength' => 180,
  );
  $form['simplenews_subscription']['subscription_mail']['simplenews_confirm_subscribe_unsubscribed'] = array(
    '#type' => 'textarea',
    '#title' => t('Body text of subscribe email'),
    '#default_value' => _simplenews_subscription_confirmation_text('subscribe_unsubscribed'),
    '#rows' => 5,
  );
  $form['simplenews_subscription']['subscription_mail']['simplenews_confirm_subscribe_subscribed'] = array(
    '#type' => 'textarea',
    '#title' => t('Body text for already subscribed visitor'),
    '#default_value' => _simplenews_subscription_confirmation_text('subscribe_subscribed'),
    '#rows' => 5,
  );
  $form['simplenews_subscription']['subscription_mail']['simplenews_confirm_unsubscribe_subscribed'] = array(
    '#type' => 'textarea',
    '#title' => t('Body text of unsubscribe email'),
    '#default_value' => _simplenews_subscription_confirmation_text('unsubscribe_subscribed'),
    '#rows' => 5,
  );
  $form['simplenews_subscription']['subscription_mail']['simplenews_confirm_unsubscribe_unsubscribed'] = array(
    '#type' => 'textarea',
    '#title' => t('Body text for not yet subscribed visitor'),
    '#default_value' => _simplenews_subscription_confirmation_text('unsubscribe_unsubscribed'),
    '#rows' => 5,
  );

  return system_settings_form($form);
}

/**
 * Callback function to activate the specified subscriptions.
 *
 * @param $snid array of snid's
 */
function simplenews_activate_subscription($snid) {
  db_query('UPDATE {simplenews_subscriptions} SET activated = %d WHERE snid IN('. db_placeholders($snid, 'int') .')', array_merge(array('1'), $snid));
}

/**
 * Callback function to inactivate the specified subscriptions.
 *
 * @param $snid array of snid's
 */
function simplenews_inactivate_subscription($snid) {
  db_query('UPDATE {simplenews_subscriptions} SET activated = %d WHERE snid IN('. db_placeholders($snid, 'int') .')', array_merge(array('0'), $snid));
}

/**
 * Built filter selection box options and filter query where clause
 *
 * @param $type identification string
 * @param $na TRUE for orphaned newsletters
 *
 * @return array of filter selection box options and related query where clause
 */
function simplenews_get_filter($type, $na = TRUE) {
  //Default data
  $names['all'] = t('all newsletters');
  $queries['all'] = '';
  if ($na) {
    $names['na'] = t('orphaned newsletters');
    $queries['na'] = ' AND s.tid = 0';
  }
  // Data for each newsletter
  foreach (taxonomy_get_tree(variable_get('simplenews_vid', '')) as $newsletter) {
    $names[$newsletter->tid] = $newsletter->name;
    $queries[$newsletter->tid] = ' AND s.tid = '. $newsletter->tid;
  }
  return array($names, $queries);
}

/**
 * Generate subscription filters
 */
function simplenews_subscription_filters() {
  // Newsletter filter
  $filters['newsletter'] = array(
    'title' => t('Subscribed to'),
    'options' => array(
      'all'   => t('All newsletters'),
    ),
  );
  foreach (taxonomy_get_tree(variable_get('simplenews_vid', '')) as $newsletter) {
    $filters['newsletter']['options']['tid-'. $newsletter->tid] = $newsletter->name;
  }

  // Email filter
  $filters['email'] = array(
    'title' => t('Email address'),
  );

  return $filters;
}

/**
 * Return form for subscription filters.
 *
 * @see simplenews_subscription_filter_form_submit()
 */
function simplenews_subscription_filter_form() {
  // Current filter selections in $session var; stored at form submission
  // Example: array('newsletter' => 'all', 'email' => 'hotmail')
  $session = isset($_SESSION['simplenews_subscriptions_filter']) ? $_SESSION['simplenews_subscriptions_filter'] : '';
  $session = is_array($session) ? $session : _simplenews_subscription_filter_default();
  $filters = simplenews_subscription_filters();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscription filters'),
    '#collapsible' => FALSE,
    '#prefix' => '<div class="simplenews-subscription-filter">',
    '#suffix' => '</div>',
  );

  // Filter values are default
  $form['filters']['newsletter'] = array(
    '#type' => 'select',
    '#title' => $filters['newsletter']['title'],
    '#options' => $filters['newsletter']['options'],
    '#default_value' => $session['newsletter'],
  );
  $form['filters']['email'] = array(
    '#type' => 'textfield',
    '#title' => $filters['email']['title'],
    '#default_value' => $session['email'],
  );
  $form['filters']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#prefix' => '<span class="spacer" />',
  );
  // Add Reset button if filter is in use
  if ($session != _simplenews_subscription_filter_default()) {
    $form['filters']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }
  $form['#submit'][] = 'simplenews_subscription_filter_form_submit';

  return $form;
}

/**
 * Helper function: returns subscription filter default settings
 */
function _simplenews_subscription_filter_default() {
  return array('newsletter' => 'all', 'email' => '');
}

function simplenews_subscription_filter_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {
    case t('Filter'):
      $_SESSION['simplenews_subscriptions_filter'] = array(
        'newsletter' => $form_state['values']['newsletter'],
        'email' => $form_state['values']['email'],
      );
      break;
    case t('Reset'):
      $_SESSION['simplenews_subscriptions_filter'] = _simplenews_subscription_filter_default();
      break;
  }
}

/**
 * Build query for subscription filters based on session var content.
 *
 * @return array of SQL query parts
 *   array('where' => $where, 'join' => $join, 'args' => $args)
 */
function simplenews_build_subscription_filter_query() {
  // Variables $args and $join are currently not used but left in for future extensions
  $where = $args = array();
  $join = '';

  // Build query
  if (isset($_SESSION['simplenews_subscriptions_filter'])) {
    foreach ($_SESSION['simplenews_subscriptions_filter'] as $key => $value) {
    switch ($key) {
        case 'newsletter':
          if ($value != 'all') {
            list($key, $value) = explode('-', $value, 2);
            $where[] = 's.'. $key .' = '. $value;
          }
          break;
        case 'email':
          if (!empty($value)) {
            $where[] = "ss.mail LIKE '%%". db_escape_string($value) ."%%'";
          }
          break;
      }
      $args[] = $value;
    }
  }

  // All conditions are combined with AND
  $where = empty($where) ? '' : ' AND '. implode(' AND ', $where);

  return array('where' => $where, 'join' => $join, 'args' => $args);
}

/**
 * Theme subscription administration overview.
 */
function theme_simplenews_subscription_list($form) {
  // Subscription table header
  $header = array(
    theme('table_select_header_cell'),
    array('data' => t('Email'), 'field' => 'ss.mail', 'sort' => 'asc'),
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Status'), 'field' => 'ss.activated'),
    t('Operations')
  );

  // Subscription table
  $output = drupal_render($form['options']);
  if (isset($form['mail']) && is_array($form['mail'])) {
    foreach (element_children($form['mail']) as $key) {
      $row = array();
      $row[] = drupal_render($form['snids'][$key]);
      $row[] = drupal_render($form['mail'][$key]);
      $row[] = drupal_render($form['name'][$key]);
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
    }
  }
  else  {
    $rows[] = array(array('data' => t('No subscriptions available.'), 'colspan' => '4'));
  }

  // Render table header, pager and form
  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }
  $output .= drupal_render($form);

  return $output;
}

/**
 * Process variables to format the simplenews status indication.
 *
 * $variables contains:
 * - $source
 * - $status
 *
 * @see simplenews-status.tpl.php
 * @see theme_simplenews_status()
 */
function template_preprocess_simplenews_status(&$variables) {
  switch ($variables['source']) {
    case 'published':
      $images = array(0 => 'sn-saved.png', 1 => 'sn-sent.png');
      $title = array(0 => t('Not published'), 1 => t('Published'));
      break;
    case 'activated':
      $images = array(0 => 'sn-saved.png', 1 => 'sn-sent.png');
      $title = array(0 => t('Inactive: no newsletters will be sent'), 1 => t('Active: user will receive newsletters'));
      break;
    case 'sent':
      $images = array(
        SIMPLENEWS_STATUS_SEND_NOT => 'sn-saved.png',
        SIMPLENEWS_STATUS_SEND_PENDING => 'sn-cron.png',
        SIMPLENEWS_STATUS_SEND_READY => 'sn-sent.png'
      );
      $title = array(
        SIMPLENEWS_STATUS_SEND_NOT => t('Not sent'),
        SIMPLENEWS_STATUS_SEND_PENDING => t('Currently sending by cron'),
        SIMPLENEWS_STATUS_SEND_READY => t('Sent')
      );
      break;
  }

  // Build the variables
  $variables['image'] = base_path() . drupal_get_path('module', 'simplenews') .'/'. $images[$variables['status']];
  $variables['alt'] = $variables['title'] = $title[$variables['status']];
}
