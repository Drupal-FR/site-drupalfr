<?php

/**
 * Menu callback.
 */
function drupalfr_user_membership_number_form($form, $form_state) {
  $form = array();

  $form['drupalfr_user_next_membership_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Next membership number'),
    '#description' => t('The next membership number to be attributed. This number will auto-increment.'),
    '#default_value' => variable_get('drupalfr_user_next_membership_number', 1),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  return system_settings_form($form);
}
