<?php
// $Id: webform.emails.inc,v 1.9.2.9 2010/08/19 03:00:45 quicksketch Exp $

/**
 * @file
 * Provides interface and database handling for e-mail settings of a webform.
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

/**
 * Overview form of all components for this webform.
 */
function webform_emails_form($form_state, $node) {
  module_load_include('inc', 'webform', 'includes/webform.components');

  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
    'components' => array(),
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  foreach ($node->webform['emails'] as $eid => $email) {
    $email_addresses = array_filter(explode(',', check_plain($email['email'])));
    foreach ($email_addresses as $key => $email_address) {
      $email_addresses[$key] = webform_format_email_address($email_address, NULL, $node, NULL, FALSE);
    }

    $form['emails'][$eid]['email'] = array(
      '#value' => implode('<br />', $email_addresses),
    );
    $form['emails'][$eid]['subject'] = array(
      '#value' => check_plain(webform_format_email_subject($email['subject'], $node)),
    );
    $form['emails'][$eid]['from'] = array(
      '#value' => check_plain(webform_format_email_address($email['from_address'], $email['from_name'], $node, NULL, FALSE)),
    );
  }

  $form['add'] = array(
    '#theme' => 'webform_email_add_form',
    '#tree' => FALSE,
  );

  $form['add']['email_option'] = array(
    '#type' => 'radios',
    '#options' => array(
      'custom' => t('Address'),
      'component' => t('Component value'),
    ),
    '#default_value' => 'custom',
  );

  $form['add']['email_custom'] = array(
    '#type' => 'textfield',
    '#size' => 24,
    '#maxlength' => 500,
  );

  $form['add']['email_component'] = array(
    '#type' => 'select',
    '#options' => webform_component_list($node, 'email_address', FALSE),
  );

  if (empty($form['add']['email_component']['#options'])) {
    $form['add']['email_component']['#options'][''] = t('No available components');
    $form['add']['email_component']['#disabled'] = TRUE;
  }

  $form['add_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#weight' => 45,
  );

  $form['#validate'] = array('webform_email_address_validate');

  return $form;
}

/**
 * Theme the node components form. Use a table to organize the components.
 *
 * @param $form
 *   The form array.
 * @return
 *   Formatted HTML form, ready for display.
 */
function theme_webform_emails_form($form) {
  // Add CSS to display submission info. Don't preprocess because this CSS file is used rarely.
  drupal_add_css(drupal_get_path('module', 'webform') . '/css/webform-admin.css', 'theme', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'webform') . '/js/webform-admin.js', 'module', 'header', FALSE, TRUE, FALSE);

  $node = $form['#node'];

  $header = array(t('E-mail to'), t('Subject'), t('From'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();

  if (!empty($form['emails'])) {
    foreach (element_children($form['emails']) as $eid) {
      // Add each component to a table row.
      $rows[] = array(
        drupal_render($form['emails'][$eid]['email']),
        drupal_render($form['emails'][$eid]['subject']),
        drupal_render($form['emails'][$eid]['from']),
        l(t('Edit'), 'node/' . $node->nid . '/webform/emails/' . $eid),
        l(t('Delete'), 'node/' . $node->nid . '/webform/emails/' . $eid . '/delete'),
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('Currently not sending e-mails, add an e-mail recipient below.'), 'colspan' => 5));
  }

  // Add a row containing form elements for a new item.
  $row_data = array(
    array('colspan' => 3, 'data' => drupal_render($form['add'])),
    array('colspan' => 2, 'data' => drupal_render($form['add_button'])),
  );
  $rows[] = array('data' => $row_data, 'class' => 'webform-add-form');

  $output = '';
  $output .= theme('table', $header, $rows, array('id' => 'webform-emails'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme the add new e-mail settings form on the node/x/webform/emails page.
 */
function theme_webform_email_add_form($form) {
  // Add a default value to the custom e-mail textfield.
  $form['email_custom']['#attributes']['rel'] = t('email@example.com');
  $form['email_custom']['#attributes']['class'] = 'webform-set-active webform-default-value';
  $form['email_option']['custom']['#title'] = $form['email_option']['custom']['#title'] . ': ' . drupal_render($form['email_custom']);

  // Render the component value.
  $form['email_component']['#attributes']['class'] = 'webform-set-active';
  $form['email_option']['component']['#title'] = $form['email_option']['component']['#title'] . ': ' . drupal_render($form['email_component']);

  // For spacing consistency, every option is wrapped in webform-container-inline.
  foreach (element_children($form['email_option']) as $option) {
    $form['email_option'][$option]['#prefix'] = '<div class="webform-container-inline">';
    $form['email_option'][$option]['#suffix'] = '</div>';
  }

  return drupal_render($form);
}

/**
 * Submit handler for webform_emails_form().
 */
function webform_emails_form_submit($form, &$form_state) {
  if ($form_state['values']['email_option'] == 'custom') {
    $email = $form_state['values']['email_custom'];
  }
  else {
    $email = $form_state['values']['email_component'];
  }
  $form_state['redirect'] = array('node/' . $form['#node']->nid . '/webform/emails/new', 'option=' . urlencode($form_state['values']['email_option']) . '&email=' . urlencode(trim($email)));
}

/**
 * Form for configuring an e-mail setting and template.
 */
function webform_email_edit_form($form_state, $node, $email = array()) {
  module_load_include('inc', 'webform', 'includes/webform.components');

  $form = array(
    '#tree' => TRUE,
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['eid'] = array(
    '#type' => 'value',
    '#value' => isset($email['eid']) ? $email['eid'] : NULL,
  );

  // All these fields work essentially the same, with a radio button set,
  // a textfield for custom values, and a select list for a component.
  foreach (array('email', 'subject', 'from_address', 'from_name') as $field) {
    switch ($field) {
      case 'email':
        $default_value = NULL;
        $title = t('E-mail to address');
        $description = t('Form submissions will be e-mailed to this address. Any email, select, or hidden form element may be selected as the recipient address. Multiple e-mail addresses may be separated by commas.');
        break;
      case 'subject':
        $default_value = _webform_filter_values(webform_variable_get('webform_default_subject'), $node);
        $title = t('E-mail subject');
        $description = t('Any textfield, select, or hidden form element may be selected as the subject for e-mails.');
        break;
      case 'from_address':
        $default_value = _webform_filter_values(webform_variable_get('webform_default_from_address'), $node);
        $title = t('E-mail from address');
        $description = t('Any email, select, or hidden form element may be selected as the sender\'s e-mail address.');
        break;
      case 'from_name':
        $default_value = _webform_filter_values(webform_variable_get('webform_default_from_name'), $node);
        $title = t('E-mail from name');
        $description = t('Any textfield, select, or hidden form element may be selected as the sender\'s name for e-mails.');
        break;
    }

    $form[$field . '_option'] = array(
      '#title' => $title,
      '#type' => 'radios',
      '#default_value' => is_numeric($email[$field]) ? 'component' : ((empty($default_value) || ($email[$field] != 'default' && isset($email[$field]))) ? 'custom' : 'default'),
      '#description' => $description,
    );
    if (!empty($default_value)) {
      $form[$field . '_option']['#options']['default'] = $default_value;
    }
    $form[$field . '_option']['#options']['custom'] = 'custom';
    $form[$field . '_option']['#options']['component'] = 'component';

    $form[$field . '_custom'] = array(
      '#type' => 'textfield',
      '#size' => 40,
      '#default_value' => (!is_numeric($email[$field]) && $email[$field] != 'default') ? $email[$field] : NULL,
      '#maxlength' => $field == 'email' ? 500 : 255,
    );
    $options = webform_component_list($node, $field == 'from_address' || $field == 'email' ? 'email_address' : 'email_name', FALSE);
    $form[$field . '_component'] = array(
      '#type' => 'select',
      '#default_value' =>  is_numeric($email[$field]) ? $email[$field] : NULL,
      '#options' => empty($options) ? array('' => t('No available components')) : $options,
      '#disabled' => empty($options) ? TRUE : FALSE,
      '#weight' => 6,
    );
  }

  // Do not show the "E-mail from name" if using the short e-mail format.
  if (variable_get('webform_email_address_format', 'long') == 'short') {
    $form['from_name_option']['#access'] = FALSE;
    $form['from_name_custom']['#access'] = FALSE;
    $form['from_name_component']['#access'] = FALSE;
  }

  // Add the template fieldset.
  $form['template'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail template'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($email['cid']) && empty($email['template']),
    '#description' => t('An e-mail template can customize the display of e-mails.'),
    '#weight' => 15,
    '#tree' => FALSE,
    '#attributes' => array('id' => 'webform-template-fieldset'),
  );

  $default_template = theme(array('webform_mail_' . $node->nid, 'webform_mail', 'webform_mail_message'), $node, NULL, 'default');
  $template = $email['template'] == 'default' ? $default_template : $email['template'];
  $form['template']['template'] = array(
    '#type' => 'textarea',
    '#rows' => max(10, min(20, count(explode("\n", $template)))),
    '#description' => theme('webform_token_help', $node),
    '#default_value' => $template,
  );

  $form['template']['components'] = array(
    '#type' => 'select',
    '#title' => t('Included e-mail values'),
    '#options' => webform_component_list($node, 'email', TRUE),
    '#default_value' => array_diff(array_keys($node->webform['components']), $email['excluded_components']),
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('The selected components will be included in the %email_values token. Individual values may still be printed if explicitly specified as a %email[key] in the template.'),
    '#process' => array('webform_component_select'),
  );

  $form['template']['template_option'] = array(
    '#type' => 'select',
    '#options' => array(
      'default' => t('Default template'),
      'custom' => t('Custom template'),
    ),
    '#description' => t('Warning: Changing will immediately update the textarea above.'),
  );

  // TODO: Allow easy re-use of existing templates.
  $form['templates']['#tree'] = TRUE;
  $form['templates']['default'] = array(
    '#type' => 'textarea',
    '#value' => $default_template,
    '#resizable' => FALSE,
    '#weight' => 19,
  );

  // Add the submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save e-mail settings'),
    '#weight' => 20,
  );

  $form['#validate'] = array('webform_email_address_validate', 'webform_email_edit_form_validate');

  return $form;
}

/**
 * Theme the Webform mail settings section of the node form.
 */
function theme_webform_email_edit_form($form) {
  drupal_add_css(drupal_get_path('module', 'webform') . '/css/webform-admin.css', 'theme', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'webform') . '/js/webform-admin.js', 'module', 'header', FALSE, TRUE, FALSE);

  // Loop through fields, rendering them into radio button options.
  foreach (array('email', 'subject', 'from_address', 'from_name') as $field) {
    foreach (array('custom' => t('Custom'), 'component' => t('Component')) as $option => $title) {
      $form[$field . '_' . $option]['#attributes']['class'] = 'webform-set-active';
      $form[$field . '_option'][$option]['#title'] = $title . ': ' . drupal_render($form[$field . '_' . $option]);
    }
    // For spacing consistency, every option is wrapped in webform-container-inline.
    foreach (element_children($form[$field . '_option']) as $option) {
      $form[$field . '_option'][$option]['#prefix'] = '<div class="webform-container-inline">';
      $form[$field . '_option'][$option]['#suffix'] = '</div>';
    }
    // Wrap the default option in a placeholder tag..
    if (isset($form[$field . '_option']['#options']['default'])) {
      $form[$field . '_option']['default']['#title'] = t('Default') . ': ' . theme('placeholder', $form[$field . '_option']['default']['#title']);
    }
  }

  $details = '';
  $details .= drupal_render($form['subject_option']);
  $details .= drupal_render($form['from_address_option']);
  $details .= drupal_render($form['from_name_option']);
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail header details'),
    '#weight' => 10,
    '#children' => $details,
    '#collapsible' => FALSE,
  );

  // Ensure templates are completely hidden.
  $form['templates']['#prefix'] = '<div id="webform-email-templates" style="display: none">';
  $form['templates']['#suffix'] = '</div>';

  $form['template']['template']['#description'] .= drupal_render($form['template']['components']);

  return drupal_render($form);
}

/**
 * Validate handler for webform_email_edit_form() and webform_emails_form().
 */
function webform_email_address_validate($form, &$form_state) {
  if ($form_state['values']['email_option'] == 'custom') {
    $email = trim($form_state['values']['email_custom']);
    if (empty($email)) {
      form_set_error('email_custom', t('When adding a new custom e-mail, the e-mail field is required.'));
    }
    else {
      $emails = array_filter(explode(',', $email));
      foreach ($emails as $email) {
        if (!valid_email_address(trim($email))) {
          form_set_error('email_custom', t('The entered e-mail address "@email" does not appear valid.', array('@email' => $email)));
        }
      }
    }
  }
}

/**
 * Validate handler for webform_email_edit_form().
 */
function webform_email_edit_form_validate($form, &$form_state) {
  if ($form_state['values']['from_address_option'] == 'custom' && !valid_email_address($form_state['values']['from_address_custom'])) {
    form_set_error('from_address_custom', t('The entered e-mail address "@email" does not appear valid.', array('@email' => $form_state['values']['from_address_custom'])));
  }
}

/**
 * Submit handler for webform_email_edit_form().
 */
function webform_email_edit_form_submit($form, &$form_state) {
  // Merge the e-mail, name, address, and subject options into single values.
  $email = array(
    'eid' => $form_state['values']['eid'],
    'nid' => $form_state['values']['node']->nid,
  );

  foreach (array('email', 'from_name', 'from_address', 'subject') as $field) {
    $option = $form_state['values'][$field . '_option'];
    if ($option == 'default') {
      $email[$field] = 'default';
    }
    else {
      $email[$field] = $form_state['values'][$field . '_' . $option];
    }
  }

  // Ensure templates are unaffected by differences in line breaks.
  $form_state['values']['template'] = str_replace(array("\r", "\n"), array('', "\n"), $form_state['values']['template']);
  $form_state['values']['templates']['default'] = str_replace(array("\r", "\n"), array('', "\n"), $form_state['values']['templates']['default']);

  // Set the template value.
  // TODO: Support reuse of templates.
  if (strcmp(trim($form_state['values']['templates']['default']), trim($form_state['values']['template'])) == 0) {
    $email['template'] = 'default';
  }
  else {
    $email['template'] = $form_state['values']['template'];
  }

  // Save the list of included components.
  // We actually maintain an *exclusion* list, so any new components will
  // default to being included in the %email_values token until unchecked.
  $included = array_keys(array_filter((array) $form_state['values']['components']));
  $excluded = array_diff(array_keys($form_state['values']['node']->webform['components']), $included);
  $email['excluded_components'] = $excluded;

  if (empty($form_state['values']['eid'])) {
    drupal_set_message(t('Email settings added.'));
    $cid = webform_email_insert($email);
  }
  else {
    drupal_set_message(t('Email settings updated.'));
    webform_email_update($email);
  }

  $form_state['redirect'] = array('node/' . $form_state['values']['node']->nid . '/webform/emails');
}

/**
 * Form for deleting an e-mail setting.
 */
function webform_email_delete_form($form_state, $node, $email) {
  $eid = $email['eid'];

  $form = array();
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['email'] = array(
    '#type' => 'value',
    '#value' => $email,
  );

  $question = t('Delete e-mail settings?');
  if (is_numeric($email['email'])) {
    $description = t('This will immediately delete the e-mail settings based on the @component component.', array('@component' => $email['email']));
  }
  else {
    $description = t('This will immediately delete the e-mail settings sending to the @address address.', array('@address' => $email['email']));
  }

  return confirm_form($form, $question, 'node/' . $node->nid . '/webform/emails', $description, t('Delete'));
}

/**
 * Submit handler for webform_component_delete_form().
 */
function webform_email_delete_form_submit($form, &$form_state) {
  drupal_set_message(t('E-mail settings deleted.'));
  webform_email_delete($form_state['values']['node'], $form_state['values']['email']);
  $form_state['redirect'] = 'node/' . $form_state['values']['node']->nid . '/webform/emails';
}

/**
 * Insert a new e-mail setting into the database.
 *
 * @param $email
 *   An array of settings for sending an e-mail.
 */
function webform_email_insert($email) {
  db_lock_table('webform_emails');
  $email['eid'] = isset($email['eid']) ? $email['eid'] : db_result(db_query('SELECT MAX(eid) FROM {webform_emails} WHERE nid = %d', $email['nid'])) + 1;
  db_query("INSERT INTO {webform_emails} (nid, eid, email, subject, from_name, from_address, template, excluded_components) VALUES (%d, %d, '%s', '%s', '%s', '%s', '%s', '%s')", $email['nid'], $email['eid'], $email['email'], $email['subject'], $email['from_name'], $email['from_address'], $email['template'], implode(',', $email['excluded_components']));
  db_unlock_tables();
  return $email['eid'];
}

/**
 * Update an existing e-mail setting with new values.
 *
 * @param $email
 *   An array of settings for sending an e-mail containing a nid, eid, and all
 *   other fields from the e-mail form.
 */
function webform_email_update($email) {
  return db_query("UPDATE {webform_emails} SET email = '%s', subject = '%s', from_name = '%s', from_address = '%s', template = '%s', excluded_components = '%s' WHERE nid = %d AND eid = %d", $email['email'], $email['subject'], $email['from_name'], $email['from_address'], $email['template'], implode(',', $email['excluded_components']), $email['nid'], $email['eid']);
}

/**
 * Delete an e-mail setting.
 */
function webform_email_delete($node, $email) {
  db_query('DELETE FROM {webform_emails} WHERE nid = %d AND eid = %d', $node->nid, $email['eid']);
}
