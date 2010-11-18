<?php
// $Id: grid.inc,v 1.14.2.8 2010/06/20 18:10:33 quicksketch Exp $

/**
 * @file
 * Webform module grid component.
 */

// Grid depends on functions provided by select.
webform_component_include('select');

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_grid() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'email' => 1,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'options' => '',
      'questions' => '',
      'optrand' => 0,
      'qrand' => 0,
      'description' => '',
      'custom_option_keys' => 0,
      'custom_question_keys' => 0,
    ),
  );
}


/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_grid() {
  return array(
    'webform_grid' => array(
      'arguments' => array('grid_element' => NULL),
    ),
    'webform_display_grid' => array(
      'arguments' => array('element' => NULL),
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_grid($component) {
  $form = array();

  if (module_exists('options_element')) {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options'),
      '#collapsible' => TRUE,
      '#description' => t('Options to select across the top. Usually these are ratings such as "poor" through "excellent" or "strongly disagree" through "strongly agree".'),
      '#attributes' => array('class' => 'webform-options-element'),
      '#element_validate' => array('_webform_edit_validate_options'),
    );
    $form['options']['options'] = array(
      '#type' => 'options',
      '#options' => _webform_select_options_from_text($component['extra']['options'], TRUE),
      '#optgroups' => FALSE,
      '#default_value' => FALSE,
      '#optgroups' => FALSE,
      '#key_type' => 'mixed',
      '#key_type_toggle' => t('Customize option keys (Advanced)'),
      '#key_type_toggled' => $component['extra']['custom_option_keys'],
    );

    $form['questions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Questions'),
      '#collapsible' => TRUE,
      '#description' => t('Questions list down the side of the grid.'),
      '#attributes' => array('class' => 'webform-options-element'),
      '#element_validate' => array('_webform_edit_validate_options'),
    );
    $form['questions']['options'] = array(
      '#type' => 'options',
      '#options' => _webform_select_options_from_text($component['extra']['questions'], TRUE),
      '#optgroups' => FALSE,
      '#default_value' => FALSE,
      '#optgroups' => FALSE,
      '#key_type' => 'mixed',
      '#key_type_toggle' => t('Customize question keys (Advanced)'),
      '#key_type_toggled' => $component['extra']['custom_question_keys'],
    );
  }
  else {
    $form['extra']['options'] = array(
      '#type' => 'textarea',
      '#title' => t('Options'),
      '#default_value' => $component['extra']['options'],
      '#description' => t('Options to select across the top. One option per line. Key-value pairs must be entered separated by pipes. i.e. safe_key|Some readable option') . theme('webform_token_help'),
      '#cols' => 60,
      '#rows' => 5,
      '#weight' => -3,
      '#required' => TRUE,
      '#element_validate' => array('_webform_edit_validate_select'),
    );
    $form['extra']['questions'] = array(
      '#type' => 'textarea',
      '#title' => t('Questions'),
      '#default_value' => $component['extra']['questions'],
      '#description' => t('Questions list down the side of the grid. One question per line. Key-value pairs must be entered separated by pipes. i.e safe_key|Some readable option') . theme('webform_token_help'),
      '#cols' => 60,
      '#rows' => 5,
      '#weight' => -2,
      '#required' => TRUE,
      '#element_validate' => array('_webform_edit_validate_select'),
    );
  }

  $form['display']['optrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Options'),
    '#default_value' => $component['extra']['optrand'],
    '#description' => t('Randomizes the order of options on the top when they are displayed in the form.'),
    '#parents' => array('extra', 'optrand')
  );
  $form['display']['qrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Questions'),
    '#default_value' => $component['extra']['qrand'],
    '#description' => t('Randomize the order of the questions on the side when they are displayed in the form.'),
    '#parents' => array('extra', 'qrand')
  );
  return $form;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_grid($component, $value = NULL, $filter = TRUE) {
  $element = array(
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_grid',
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#prefix' => '<div class="webform-component webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);

  if ($component['extra']['optrand']) {
    _webform_shuffle_options($options);
  }

  if ($component['extra']['qrand']) {
    _webform_shuffle_options($questions);
  }

  foreach ($questions as $key => $question) {
    if ($question != '') {
      $element[$key] = array(
        '#title' => $question,
        '#required' => $component['mandatory'],
        '#options' => $options,
        '#type' => 'radios',
        '#process' => array('expand_radios', 'webform_expand_select_ids'),

        // Webform handles validation manually.
        '#validated' => TRUE,
        '#webform_validated' => FALSE,
      );
    }
  }

  if (isset($value)) {
    foreach (element_children($element) as $key) {
      $element[$key]['#default_value'] = isset($value[$key]) ? $value[$key] : NULL;
    }
  }

  return $element;
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_grid($component, $value, $format = 'html') {
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);

  $element = array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#component' => $component,
    '#format' => $format,
    '#questions' => $questions,
    '#options' => $options,
    '#theme' => 'webform_display_grid',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#sorted' => TRUE,
  );

  foreach ($questions as $key => $question) {
    if ($question != '') {
      $element[$question] = array(
        '#title' => $question,
        '#value' => isset($value[$key]) ? $value[$key] : NULL,
      );
    }
  }

  return $element;
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_grid($element) {
  $component = $element['#component'];
  $format = $element['#format'];

  if ($format == 'html') {
    $rows = array();
    $header = array(array('data' => '', 'class' => 'webform-grid-question'));
    foreach ($element['#options'] as $option) {
      $header[] = array('data' => _webform_filter_xss($option), 'class' => 'checkbox webform-grid-option');
    }
    foreach (element_children($element) as $key) {
      $row = array();
      $row[] = array('data' => _webform_filter_xss($element[$key]['#title']), 'class' => 'webform-grid-question');
      foreach ($element['#options'] as $option_value => $option_label) {
        if ($option_value == $element[$key]['#value']) {
          $row[] = array('data' => '<strong>X</strong>', 'class' => 'checkbox webform-grid-option');
        }
        else {
          $row[] = array('data' => '&nbsp', 'class' => 'checkbox webform-grid-option');
        }
      }
      $rows[] = $row;
    }

    $option_count = count($header) - 1;
    $output = theme('table', $header, $rows, array('class' => 'webform-grid webform-grid-' . $option_count));
  }
  else {
    $items = array();
    foreach (element_children($element) as $key) {
      $items[] = ' - ' . $element[$key]['#title'] . ': ' . (isset($element['#options'][$element[$key]['#value']]) ? $element['#options'][$element[$key]['#value']] : '');
    }
    $output = implode("\n", $items);
  }

  return $output;
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_grid($component, $sids = array()) {
  // Generate the list of options and questions.
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);

  // Generate a lookup table of results.
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (" . implode(",", $placeholders) . ")" : "";
  $query = 'SELECT no, data, count(data) as datacount ' .
    ' FROM {webform_submitted_data} ' .
    ' WHERE nid = %d ' .
    ' AND cid = %d ' .
    " AND data != '' " . $sidfilter .
    ' GROUP BY no, data';
  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  $counts = array();
  while ($data = db_fetch_object($result)) {
    $counts[$data->no][$data->data] = $data->datacount;
  }

  // Create an entire table to be put into the returned row.
  $rows = array();
  $header = array('');

  // Add options as a header row.
  foreach ($options as $option) {
    $header[] = _webform_filter_xss($option);
  }

  // Add questions as each row.
  foreach ($questions as $qkey => $question) {
    $row = array(_webform_filter_xss($question));
    foreach ($options as $okey => $option) {
      $row[] = !empty($counts[$qkey][$okey]) ? $counts[$qkey][$okey] : 0;
    }
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows, array('class' => 'webform-grid'));


  return array(array(array('data' => $output, 'colspan' => 2)));
}

/**
 * Implementation of _webform_table_component().
 */
function _webform_table_grid($component, $value) {
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);

  $output = '';
  // Set the value as a single string.
  foreach ($questions as $key => $label) {
    if (isset($value[$key]) && isset($options[$value[$key]])) {
      $output .= _webform_filter_xss($label) . ': ' . _webform_filter_xss($options[$value[$key]]) . '<br />';
    }
  }

  return $output;
}

/**
 * Implementation of _webform_csv_headers_component().
 */
function _webform_csv_headers_grid($component, $export_options) {
  $header = array();
  $header[0] = array('');
  $header[1] = array($component['name']);
  $items = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $count = 0;
  foreach ($items as $key => $item) {
    // Empty column per sub-field in main header.
    if ($count != 0) {
      $header[0][] = '';
      $header[1][] = '';
    }
    // The value for this option.
    $header[2][] = $item;
    $count++;
  }

  return $header;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_grid($component, $export_options, $value) {
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);
  $return = array();
  foreach ($questions as $key => $question) {
    $return[] = isset($value[$key]) && isset($options[$value[$key]]) ? $options[$value[$key]] : '';
  }
  return $return;
}

function theme_webform_grid($element) {
  $rows = array();
  $header = array(array('data' => '', 'class' => 'webform-grid-question'));
  $first = TRUE;
  foreach (element_children($element) as $key) {
    $question_element = $element[$key];

    // Set the header for the table.
    if ($first) {
      foreach ($question_element['#options'] as $option) {
        $header[] = array('data' => _webform_filter_xss($option), 'class' => 'checkbox webform-grid-option');
      }
      $first = FALSE;
    }

    // Create a row with the question title.
    $row = array(array('data' => _webform_filter_xss($question_element['#title']), 'class' => 'webform-grid-question'));

    // Render each radio button in the row.
    $radios = expand_radios($question_element);
    foreach (element_children($radios) as $key) {
      unset($radios[$key]['#title']);
      $row[] = array('data' => drupal_render($radios[$key]), 'class' => 'checkbox webform-grid-option');
    }
    $rows[] = $row;
  }

  $option_count = count($header) - 1;
  return theme('form_element', $element, theme('table', $header, $rows, array('class' => 'webform-grid webform-grid-' . $option_count)));
}
