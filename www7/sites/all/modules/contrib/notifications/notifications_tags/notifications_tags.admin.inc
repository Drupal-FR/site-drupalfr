<?php
/**
 * @file
 * Subscriptions to taxonomy terms
 */

/**
 * Admin settings form. Omitted taxonomy vocabularies.
 */
function notifications_tags_admin_settings($form, &$form_state) {
  $select = array();
  foreach (taxonomy_get_vocabularies() as $vocabulary) {
    $select[$vocabulary->vid] = check_plain($vocabulary->name);
  }
  $form['tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tag subscriptions'),
    '#collapsible' => TRUE,
    '#weight' => 0,
  );
  $form['tags']['notifications_tags_vocabularies'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Allowed vocabularies'),
    '#default_value' => notifications_tags_vocabulary_list('vid'),
    '#options'       => $select,
    '#description'   => t('Select vocabularies to which subscriptions should be <em>allowed</em>.'),
    '#multiple'      => TRUE,
  );
  $form['tags']['notifications_tags_showsubscribed'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Limit the displayed taxonomy terms'),
    '#default_value' => variable_get('notifications_tags_showsubscribed', 0),
    '#description'   => t('If checked, the displayed taxonomy terms in User Interface form will be limited to subscribed terms only. When you have too many taxonomy terms causing the Tags tab to run out of memory under Notifications UI, enabling this setting can fix the problem by only displaying the subscribed taxonomy terms. Note that if you enable this setting, you will have to provide your users an alternative way to subscribe to taxonomy items, such as providing links like: notifications/subscribe/%user/taxonomy/tid/%tid . The link will bring up the standard subscription confirmation dialog.'),
  );
  return system_settings_form($form);
}
