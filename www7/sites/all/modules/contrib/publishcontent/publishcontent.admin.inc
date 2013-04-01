<?php

/**
 * @file
 * Contains page callbacks for publishcontent
 */

function publishcontent_config_form($form, &$form_state) {

  $form['publishcontent_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Publish/Unpublish tabs'),
    '#default_value' => variable_get('publishcontent_tabs', TRUE),
    '#description' => t("Uncheck this box to hide the publish/unpublish tabs on nodes."),
  );

  return system_settings_form($form);
}

