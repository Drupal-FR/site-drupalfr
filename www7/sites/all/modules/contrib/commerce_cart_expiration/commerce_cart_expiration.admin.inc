<?php

/**
 * @file
 * Provides administration pages for commerce_cart_expiration module.
 */

/**
 * Form constructor for the cart expiration form.
 */
function commerce_cart_expiration_admin_settings() {
  $form = array();

  $explanation_content = variable_get('commerce_cart_expiration_explanation_page', array('value' => '<p>'. t('Sorry, you took too much time in the checkout process.') .'</p><p><a href="[site:url]">'. t('Return to the front page') .'</a></p>', 'format' => NULL));
  $form['explanation_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Explanation page'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['explanation_page']['commerce_cart_expiration_explanation_page'] = array(
    '#type' => 'text_format',
    '#title' => t('Content of the explanation page'),
    '#default_value' => $explanation_content['value'],
    '#format' => $explanation_content['format'],
  );
  $form['explanation_page']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['explanation_page']['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('commerce-order', 'user', 'site'),
  );

  return system_settings_form($form);
}
