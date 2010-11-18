<?php
// $Id: hidden.inc,v 1.21.2.3 2010/06/21 03:35:13 quicksketch Exp $

/**
 * @file
 * Webform module hidden component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_hidden() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
    ),
  );
}

/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_hidden() {
  return array(
    'webform_display_hidden' => array(
      'arguments' => array('element' => NULL),
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_hidden($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => 0,
  );
  $form['extra']['description'] = array(); // Hide the description box.
  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_hidden($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#type'          => 'hidden',
    '#title'         => $component['name'],
    '#default_value' => $filter ? _webform_filter_values($component['value']) : $component['value'],
    '#weight'        => $component['weight'],
  );

  if (isset($value[0])) {
    $element['#default_value'] = $value[0];
  }

  return $element;
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_hidden($component, $value, $format = 'html') {
  $element = array(
    '#title' => t('!name (hidden)', array('!name' => $component['name'])),
    '#value' => isset($value[0]) ? $value[0] : NULL,
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_hidden',
    '#component' => $component,
    '#format' => $format,
    '#theme' => 'webform_display_hidden',
    '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
    '#post_render' => array('webform_element_wrapper'),
  );

  // TODO: This check is unusual. It shows hidden fields in e-mails but not
  // when viewing in the browser unless you're an administrator. This should be
  // a more logical check. See these related issues:
  // http://drupal.org/node/313639
  // http://drupal.org/node/781786
  if ($format == 'html') {
    $element['#access'] = user_access('edit all webform submissions') || user_access('access all webform results');
  }

  return $element;
}

function theme_webform_display_hidden($element) {
  return $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_hidden($component, $sids = array()) {
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (" . implode(",", $placeholders) . ")" : "";
  $query = 'SELECT data ' .
    ' FROM {webform_submitted_data} ' .
    ' WHERE nid = %d ' .
    ' AND cid = %d ' . $sidfilter;
  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  while ($data = db_fetch_array($result)) {
    if ( strlen(trim($data['data'])) > 0 ) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }

  $rows[0] = array( t('Empty'), ($submissions - $nonblanks));
  $rows[1] = array( t('Non-empty'), $nonblanks);
  $rows[2] = array( t('Average submission length in words (ex blanks)'),
                    ($nonblanks !=0 ? number_format($wordcount/$nonblanks, 2) : '0'));
  return $rows;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_table_hidden($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_headers_hidden($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_hidden($component, $export_options, $value) {
  return empty($value[0]) ? '' : $value[0];
}
