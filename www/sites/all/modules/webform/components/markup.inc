<?php
// $Id: markup.inc,v 1.13.2.3 2010/03/21 03:38:12 quicksketch Exp $

/**
 * @file
 * Webform module markup component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_markup() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'format' => FILTER_FORMAT_DEFAULT,
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_markup($component) {
  $form = array();
  $form['markup']['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Value'),
    '#default_value' => $component['value'],
    '#description' => t('Markup allows you to enter custom HTML or PHP logic into your form.') . theme('webform_token_help'),
    '#weight' => -1,
    '#parents' => array('value'),
  );
  // Add the filter form.
  $form['markup']['format'] = filter_form($component['extra']['format'], 0, array('extra', 'format'));

  // No description for markup.
  $form['extra']['description'] = array();

  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_markup($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type'   => 'markup',
    '#title'  => $component['name'],
    '#weight' => $component['weight'],
    '#markup' => $filter ? _webform_filter_values(check_markup($component['value'], $component['extra']['format'], FALSE), NULL, NULL, NULL, FALSE) : $component['value'],
    '#input_format' => $component['extra']['format'],
    '#prefix' => '<div class="webform-component webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  // TODO: Remove when #markup becomes available in D7.
  $element['#value'] = $element['#markup'];

  return $element;
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_markup($component, $value, $format = 'html') {
  return array();
}
