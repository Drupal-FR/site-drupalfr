<?php
// $Id: fieldset.inc,v 1.11.2.3 2010/03/21 03:43:16 quicksketch Exp $

/**
 * @file
 * Webform module fieldset component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_fieldset() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'collapsible' => 0,
      'collapsed' => 0,
      'description' => '',
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_fieldset($component) {
  $form = array();
  $form['display']['collapsible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsible'),
    '#default_value' => $component['extra']['collapsible'],
    '#description' => t('If this fieldset is collapsible, the user may open or close the fieldset.'),
    '#weight' => 0,
    '#parents' => array('extra', 'collapsible'),
  );
  $form['display']['collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsed by Default'),
    '#default_value' => $component['extra']['collapsed'],
    '#description' => t('Collapsible fieldsets are "open" by default. Select this option to default the fieldset to "closed."'),
    '#weight' => 3,
    '#parents' => array('extra', 'collapsed'),
  );
  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_fieldset($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type'          => $component['type'],
    '#title'         => filter_xss_admin($component['name']),
    '#weight'        => $component['weight'],
    '#description'   => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#collapsible'   => $component['extra']['collapsible'],
    '#collapsed'     => $component['extra']['collapsed'],
    '#attributes'    => array('class' => 'webform-component-' . $component['type'], 'id' => 'webform-component-' . $component['form_key']),
  );

  return $element;
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_fieldset($component, $value, $format = 'html') {
  if ($format == 'text') {
    $element = array(
      '#title' => $component['name'],
      '#weight' => $component['weight'],
      '#post_render' => array('webform_element_wrapper'),
      '#theme_wrappers' => array('webform_element_text'),
    );
  }
  else {
    $element = _webform_render_fieldset($component, $value);
  }

  $element['#component'] = $component;
  $element['#format'] = $format;

  return $element;
}
