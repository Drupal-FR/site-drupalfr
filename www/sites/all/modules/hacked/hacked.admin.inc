<?php

/**
 * @file
 *   The settings forms for Hacked!
 */

/**
 * The settings form for Hacked!
 */
function hacked_settings_form() {
  $form = array();

  $hashers = hacked_get_file_hashers();

  $form['hacked_selected_file_hasher'] = array(
    '#type' => 'fieldset',
    '#title' => t('File hasher'),
  );

  $parents = array('hacked_selected_file_hasher');
  $default = variable_get('hacked_selected_file_hasher', HACKED_DEFAULT_FILE_HASHER);

  foreach ($hashers as $name => $hasher_info) {
    // Generate the parents as the autogenerator does, so we will have a
    // unique id for each radio button.
    $parents_for_id = array_merge($parents, array($name));
    $form['hacked_selected_file_hasher'][$name] = array(
      '#type' => 'radio',
      '#title' => $hasher_info['name'],
      '#default_value' => $default,
      '#return_value' => $name,
      '#parents' => $parents,
      '#description' => !empty($hasher_info['description']) ? $hasher_info['description'] : '',
      '#id' => form_clean_id('edit-' . implode('-', $parents_for_id)),
    );
  }

  $form['#submit'][] = 'hacked_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for hacked_settings_form().
 */
function hacked_settings_form_submit($form, &$form_state) {
  // Clear the Hacked! report cache.
  cache_clear_all('hacked:full-report', HACKED_CACHE_TABLE);
}
