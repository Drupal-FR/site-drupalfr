<?php
/**
 * @file
 * Drupal Messaging Framework - Mail admin page
 */

/**
 * Specific mail settings
 */
function messaging_mail_admin_settings($form_state) {
  $default_mail = variable_get('site_mail', ini_get('sendmail_from'));

  $form['mail'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default mail addresses'),
  );
  $form['mail']['messaging_mail_default_from'] = array(
    '#title' => t('Sender mail for outgoing messages coming from the site.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('messaging_mail_default_from', $default_mail),
  );
  $form['mail']['messaging_mail_returnpath'] = array(
    '#title' => t('Mail for return-path and errors-to email address.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('messaging_mail_returnpath', $default_mail),
  );

  $form['format'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sender format'),
    '#description' => t('You can use <i>[name]</i> and <i>[mail]</i> tokens depending on the sender (that may be a user or the system) or use a fixed name and e-mail address for all outgoing mails.'),
  );
  $form['format']['messaging_mail_sender_format'] = array(
    '#title' => t('Format for sender name and address'),
    '#type' => 'textfield',
    '#default_value' => variable_get('messaging_mail_sender_format', '[name] <[mail]>'),
  );
  return system_settings_form($form);
}