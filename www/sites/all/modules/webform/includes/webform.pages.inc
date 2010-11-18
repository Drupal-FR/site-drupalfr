<?php
// $Id: webform.pages.inc,v 1.10.2.2 2010/08/19 03:00:45 quicksketch Exp $

/**
 * @file
 *
 * Menu callbacks and functions for configuring and editing webforms.
 */

/**
 * Main configuration form for editing a webform node.
 */
function webform_configure_form(&$form_state, $node) {
  $form = array();

  drupal_add_css(drupal_get_path('module', 'webform') . '/css/webform-admin.css');

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  /* Start Edit Form */
  $form['submission'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -4,
  );

  $form['submission']['confirmation_wrapper']['confirmation'] = array(
    '#type' => 'textarea',
    '#title' => t('Confirmation message'),
    '#description' => t('Message to be shown upon successful submission. If <em>Redirect URL</em> is set this displays as a message, otherwise it will be shown on its own page.'),
    '#default_value' => $node->webform['confirmation'],
    '#cols' => 40,
    '#rows' => 10,
  );

  $form['submission']['confirmation_wrapper']['format'] = filter_form($node->webform['confirmation_format'], NULL, array('confirmation_format'));

  // Redirect URL.
  $form['submission']['redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL'),
    '#description' => t('URL to redirect the user to upon successful submission.'),
    '#default_value' => $node->webform['redirect_url'],
    '#maxlength' => 255,
  );

  // Submission limit settings.
  $form['submission']['submit_limit'] = array(
    '#type' => 'item',
    '#title' => t('Submission limit'),
    '#theme' => 'webform_advanced_submit_limit_form',
    '#description' => t('Limit the number of submissions <em>per user</em>. A user is identified by their user login if logged-in, or by their IP Address and Cookie if anonymous. Use of cookies may be modified in the global <a href="!url">Webform settings</a>.', array('!url' => url('admin/settings/webform'))),
  );
  $form['submission']['submit_limit']['enforce_limit'] = array(
    '#type' => 'radios',
    '#options' => array('no' => t('Unlimited'), 'yes' => 'Limit to !count submission(s) !timespan'),
    '#default_value' => $node->webform['submit_limit'] == -1 ? 'no' : 'yes',
    '#parents' => array('enforce_limit'),
  );
  $form['submission']['submit_limit']['submit_limit'] = array(
    '#type' => 'textfield',
    '#maxlength' => 2,
    '#size' => 2,
    '#default_value' => $node->webform['submit_limit'] != -1 ? $node->webform['submit_limit'] : '',
    '#parents' => array('submit_limit'),
  );
  $form['submission']['submit_limit']['submit_interval'] = array(
    '#type' => 'select',
    '#options' => array(
      '-1' => t('ever'),
      '3600' => t('every hour'),
      '86400' => t('every day'),
      '604800' => t('every week'),
    ),
    '#default_value' => $node->webform['submit_interval'],
    '#parents' => array('submit_interval'),
  );

  /* End Edit Form */

  /* Start per-role submission control */
  $form['role_control'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission access'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -3,
    '#description' => t('These permissions affect which roles can submit this webform. It does not prevent access to the webform page. If needing to prevent access to the webform page entirely, use a content access module such as <a href="http://drupal.org/project/taxonomy_access">Taxonomy Access</a> or <a href="http://drupal.org/project/node_privacy_byrole">Node Privacy by Role</a>.'),
    '#access' => variable_get('webform_submission_access_control', 1),
  );
  $user_roles = user_roles();
  foreach ($user_roles as $rid => $rname) {
    if ($rid == DRUPAL_ANONYMOUS_RID || $rid == DRUPAL_AUTHENTICATED_RID) {
      continue;
    }
    $user_roles[$rid] = webform_tt("user:rid:$rid:name", $rname);
  }
  $form['role_control']['roles'] = array(
    '#default_value' => $node->webform['roles'],
    '#options' => $user_roles,
    '#type' => 'checkboxes',
    '#title' => t('Roles that can submit this webform'),
    '#description' => t('Uncheck all roles to prevent new submissions. The %authenticated role applies to any user signed into the site, regardless of other assigned roles.', array('%authenticated' => $user_roles[2])),
  );
  /* End per-role submission control */

  /* Start advanced settings form */
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -1,
  );
  $form['advanced']['teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show complete form in teaser'),
    '#default_value' => $node->webform['teaser'],
    '#description' => t('Display the entire form in the teaser display of this node.'),
  );
  $form['advanced']['allow_draft'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to save a draft'),
    '#default_value' => $node->webform['allow_draft'],
    '#description' => t('Allow your users to save and finish the form later.'),
  );
  $form['advanced']['submit_notice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the notification about previous submissions.'),
    '#default_value' => $node->webform['submit_notice'],
    '#description' => t('Show the previous submissions notification that appears when users have previously submitted this form.'),
  );
  $form['advanced']['submit_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Submit button text'),
    '#default_value' => $node->webform['submit_text'],
    '#description' => t('By default the submit button on this form will have the label <em>Submit</em>. Enter a new title here to override the default.'),
  );
  /* End Advanced Settings Form */

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 300,
  );

  return $form;
}

/**
 * Validate handler for webform_configure_form().
 */
function webform_configure_form_validate($form, &$form_state) {
  // Ensure the entered e-mail addresses are valid.
  if (!empty($form_state['values']['email'])) {
    $emails = explode(',', $form_state['values']['email']);
    foreach ($emails as $email) {
      if (!valid_email_address(trim($email))) {
        form_error($form['submission']['redirect_url'], t('The entered email address %address is not a valid address.', array('%address' => $email)));
        break;
      }
    }
  }

  // Ensure the entered redirect URL is valid.
  $redirect_url = trim($form_state['values']['redirect_url']);
  if (!empty($redirect_url) && strpos($redirect_url, 'http') === 0 && !valid_url($redirect_url, TRUE)) {
    form_error($form['submission']['redirect_url'], t('The entered URL is not a valid address.'));
  }
  else {
    form_set_value($form['submission']['redirect_url'], $redirect_url, $form_state);
  }
}

/**
 * Submit handler for webform_configure_form().
 */
function webform_configure_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  // Save the confirmation.
  $node->webform['confirmation'] = $form_state['values']['confirmation'];
  $node->webform['confirmation_format'] = $form_state['values']['confirmation_format'];

  // Save the redirect URL
  $node->webform['redirect_url'] = $form_state['values']['redirect_url'];

  // Save roles.
  $node->webform['roles'] = array_keys(array_filter($form_state['values']['roles']));

  // Set the Show complete form in teaser setting.
  $node->webform['teaser'] = $form_state['values']['teaser'];

  // Set the draft option.
  $node->webform['allow_draft'] = $form_state['values']['allow_draft'];

  // Set the submit limit to -1 if set to unlimited.
  if ($form_state['values']['enforce_limit'] == 'no') {
    $node->webform['submit_limit'] = -1;
    $node->webform['submit_interval'] = -1;
  }
  else {
    $node->webform['submit_limit'] = $form_state['values']['submit_limit'];
    $node->webform['submit_interval'] = $form_state['values']['submit_interval'];
  }

  // Set submit notice.
  $node->webform['submit_notice'] = $form_state['values']['submit_notice'];

  // Set submit button text.
  $node->webform['submit_text'] = $form_state['values']['submit_text'];

  node_save($node);

  drupal_set_message(t('The form settings have been updated.'));
}

/**
 * Theme the submit limit fieldset on the webform node form.
 */
function theme_webform_advanced_submit_limit_form($form) {
  $form['submit_limit']['#attributes']['class'] = 'webform-set-active';
  $form['submit_interval']['#attributes']['class'] = 'webform-set-active';
  // Remove div wrappers around limit options.
  $replacements = array(
    '!count' => preg_replace('/(<div[^>]*>)(.*?)(<\/div>)/s', '$2', drupal_render($form['submit_limit'])),
    '!timespan' =>  preg_replace('/(<div[^>]*>)(.*?)(<\/div>)/s', '$2', drupal_render($form['submit_interval'])),
  );
  $form['enforce_limit']['yes']['#title'] = t('Limit to !count submission(s) !timespan', $replacements);
  return drupal_render($form);
}
