<?php
// $Id: pagebreak.inc,v 1.9 2010/02/09 05:39:08 quicksketch Exp $

/**
 * @file
 * Webform module page break component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_pagebreak() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(),
  );
}

/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_pagebreak() {
  return array(
    'webform_display_pagebreak' => array(
      'arguments' => array('element' => NULL),
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_pagebreak($component) {
  $form = array();
  // No description.
  $form['extra']['description'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );
  // Force the parent to always be root.
  $form['position']['pid'] = array(
    '#type' => 'hidden',
    '#value' => '0',
  );

  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_pagebreak($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type' => 'hidden',
    '#value' => $component['name'],
    '#weight' => $component['weight'],
  );
  return $element;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_display_pagebreak($component, $value = NULL, $format = 'html') {
  $element = array(
    '#theme' => 'webform_display_pagebreak',
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#component' => $component,
    '#format' => $format,
  );
  return $element;
}

/**
 * Format the text output data for this component.
 */
function theme_webform_display_pagebreak($element) {
  return $element['#format'] == 'html' ? '<h2 class="webform-page">' . check_plain($element['#title']) . '</h2>' : "--" . $element['#title'] . "--\n";
}
