<?php
// $Id: webform.components.inc,v 1.25.2.17 2010/08/14 21:02:25 quicksketch Exp $

/**
 * @file
 * Webform module component handling.
 */

/**
 * Provides interface and database handling for editing components of a webform.
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

/**
 * Overview page of all components for this webform.
 */
function webform_components_page($node) {
  $form = drupal_get_form('webform_components_form', $node);
  return theme('webform_components_page', $node, $form);
}

/**
 * Theme the output of the main components page.
 *
 * This theming provides a way to toggle between the editing modes if Form
 * Builder module is available.
 */
function theme_webform_components_page($node, $form) {
  // Add CSS and JS. Don't preprocess because these files are used rarely.
  drupal_add_css(drupal_get_path('module', 'webform') . '/css/webform-admin.css', 'theme', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'webform') . '/js/webform-admin.js', 'module', 'header', FALSE, TRUE, FALSE);

  return $form;
}

/**
 * The table-based listing of all components for this webform.
 */
function webform_components_form($form_state, $node) {
  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
    'components' => array(),
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $options = array();
  foreach ($node->webform['components'] as $cid => $component) {
    $options[$cid] = check_plain($component['name']);
    $form['components'][$cid]['cid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['cid'],
    );
    $form['components'][$cid]['pid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['pid'],
    );
    $form['components'][$cid]['weight'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#title' => t('Weight'),
      '#default_value' => $component['weight'],
    );
    $form['components'][$cid]['mandatory'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mandatory'),
      '#default_value' => $component['mandatory'],
      '#access' => webform_component_feature($component['type'], 'required'),
    );
  }

  $form['add']['name'] = array(
    '#type' => 'textfield',
    '#size' => 24,
  );

  $form['add']['type'] = array(
    '#type' => 'select',
    '#options' => webform_component_options(),
    '#weight' => 3,
    '#default_value' => (isset($_GET['cid']) && isset($node->webform['components'][$_GET['cid']])) ? $node->webform['components'][$_GET['cid']]['type'] : 'textfield',
  );
  $form['add']['mandatory'] = array(
    '#type' => 'checkbox',
  );
  $form['add']['cid'] = array(
    '#type' => 'hidden',
    '#default_value' => '',
  );
  $form['add']['pid'] = array(
    '#type' => 'hidden',
    '#default_value' => (isset($_GET['cid']) && isset($node->webform['components'][$_GET['cid']])) ? $node->webform['components'][$_GET['cid']]['pid'] : 0,
  );
  $form['add']['weight'] = array(
    '#type' => 'weight',
    '#delta' => count($node->webform['components']) > 10 ? count($node->webform['components']) : 10,
    '#default_value' => (isset($_GET['cid']) && isset($node->webform['components'][$_GET['cid']])) ? $node->webform['components'][$_GET['cid']]['weight'] + 1: count($node->webform['components']),
  );

  $form['add']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#weight' => 45,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 45,
  );

  return $form;
}

/**
 * Theme the node components form. Use a table to organize the components.
 *
 * @param $form
 *   The form array.
 * @return
 *   Formatted HTML form, ready for display.
 */
function theme_webform_components_form($form) {
  // Add CSS to display submission info. Don't preprocess because this CSS file is used rarely.
  drupal_add_css(drupal_get_path('module', 'webform') . '/css/webform-admin.css', 'theme', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'webform') . '/js/webform-admin.js', 'module', 'header', FALSE, TRUE, FALSE);

  drupal_add_tabledrag('webform-components', 'order', 'sibling', 'webform-weight');
  drupal_add_tabledrag('webform-components', 'match', 'parent', 'webform-pid', 'webform-pid', 'webform-cid');

  $node = $form['#node'];

  $header = array(t('Name'), t('Type'), t('Value'), t('Mandatory'), t('Weight'), array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();

  // Add a row containing form elements for a new item.
  unset($form['add']['name']['#title'], $form['add_type']['#description']);
  $form['add']['name']['#attributes']['rel'] = t('New component name');
  $form['add']['name']['#attributes']['class'] = 'webform-default-value';
  $form['add']['cid']['#attributes']['class'] = 'webform-cid';
  $form['add']['pid']['#attributes']['class'] = 'webform-pid';
  $form['add']['weight']['#attributes']['class'] = 'webform-weight';
  $row_data = array(
    drupal_render($form['add']['name']),
    drupal_render($form['add']['type']),
    '',
    drupal_render($form['add']['mandatory']),
    drupal_render($form['add']['cid']) . drupal_render($form['add']['pid']) . drupal_render($form['add']['weight']),
    array('colspan' => 3, 'data' => drupal_render($form['add']['add'])),
  );
  $add_form = array('data' => $row_data, 'class' => 'draggable webform-add-form');
  $form_rendered = FALSE;

  if (!empty($node->webform['components'])) {
    $component_tree = array();
    $page_count = 1;
    _webform_components_tree_build($node->webform['components'], $component_tree, 0, $page_count);
    $component_tree = _webform_components_tree_sort($component_tree);
    // Build the table rows.
    function _webform_components_form_rows($node, $cid, $component, $level, &$form, &$rows, &$add_form) {
      // Create presentable values.
      if (drupal_strlen($component['value']) > 30) {
        $component['value'] = drupal_substr($component['value'], 0, 30);
        $component['value'] .= '...';
      }
      $component['value'] = check_plain($component['value']);

      // Remove individual titles from the mandatory and weight fields.
      unset($form['components'][$cid]['mandatory']['#title']);
      unset($form['components'][$cid]['pid']['#title']);
      unset($form['components'][$cid]['weight']['#title']);

      // Add special classes for weight and parent fields.
      $form['components'][$cid]['cid']['#attributes']['class'] = 'webform-cid';
      $form['components'][$cid]['pid']['#attributes']['class'] = 'webform-pid';
      $form['components'][$cid]['weight']['#attributes']['class'] = 'webform-weight';

      // Build indentation for this row.
      $indents = '';
      for ($n = 1; $n <= $level; $n++) {
        $indents .= '<div class="indentation">&nbsp;</div>';
      }

      // Add each component to a table row.
      $row_data = array(
        $indents . filter_xss($component['name']),
        t($component['type']),
        ($component['value'] == '') ? '-' : $component['value'],
        drupal_render($form['components'][$cid]['mandatory']),
        drupal_render($form['components'][$cid]['cid']) . drupal_render($form['components'][$cid]['pid']) . drupal_render($form['components'][$cid]['weight']),
        l(t('Edit'), 'node/' . $node->nid . '/webform/components/' . $cid, array('query' => drupal_get_destination())),
        l(t('Clone'), 'node/' . $node->nid . '/webform/components/' . $cid . '/clone', array('query' => drupal_get_destination())),
        l(t('Delete'), 'node/' . $node->nid . '/webform/components/' . $cid . '/delete', array('query' => drupal_get_destination())),
      );
      $row_class = 'draggable';
      if (!webform_component_feature($component['type'], 'group')) {
        $row_class .= ' tabledrag-leaf';
      }
      if ($component['type'] == 'pagebreak') {
        $row_class .= ' tabledrag-root webform-pagebreak';
        $row_data[0] = array('class' => 'webform-pagebreak', 'data' => $row_data[0]);
      }
      $rows[] = array('data' => $row_data, 'class' => $row_class);
      if (isset($component['children']) && is_array($component['children'])) {
        foreach ($component['children'] as $cid => $component) {
          _webform_components_form_rows($node, $cid, $component, $level + 1, $form, $rows, $add_form);
        }
      }

      // Add the add form if this was the last edited component.
      if (isset($_GET['cid']) && $component['cid'] == $_GET['cid'] && $add_form) {
        $add_form['data'][0] = $indents . $add_form['data'][0];
        $rows[] = $add_form;
        $add_form = FALSE;
      }
    }
    foreach ($component_tree['children'] as $cid => $component) {
      _webform_components_form_rows($node, $cid, $component, 0, $form, $rows, $add_form);
    }
  }
  else {
    $rows[] = array(array('data' => t('No Components, add a component below.'), 'colspan' => 9));
  }

  // Append the add form if not already printed.
  if ($add_form) {
    $rows[] = $add_form;
  }

  $output = '';
  $output .= theme('table', $header, $rows, array('id' => 'webform-components'));
  $output .= drupal_render($form);
  return $output;
}

function webform_components_form_validate($form, &$form_state) {
  // Check that the entered component name is valid.
  if ($form_state['values']['op'] == t('Add') && drupal_strlen(trim($form_state['values']['add']['name'])) <= 0) {
    form_error($form['add']['name'], t('When adding a new component, the name field is required.'));
  }

  // Check that no two components end up with the same form key.
  $duplicates = array();
  $parents = array();
  if (isset($form_state['values']['components'])) {
    foreach ($form_state['values']['components'] as $cid => $component) {
      $form_key = $form['#node']->webform['components'][$cid]['form_key'];
      if (isset($parents[$component['pid']]) && ($existing = array_search($form_key, $parents[$component['pid']])) && $existing !== FALSE) {
        if (!isset($duplicates[$component['form_key']])) {
          $duplicates[$component['form_key']] = array($existing);
        }
        $duplicates[$component['form_key']][] = $cid;
      }
      $parents[$component['pid']][$cid] = $form_key;
    }
  }

  if (!empty($duplicates)) {
    $error = t('The form order failed to save because the following elements have same form keys and are under the same parent. Edit each component and give them a unique form key, then try moving them again.');
    $items = array();
    foreach ($duplicates as $form_key => $cids) {
      foreach ($cids as $cid) {
        $items[] = $form['#node']->webform['components'][$cid]['name'];
      }
    }

    form_error($form['components'], $error . theme('item_list', $items));
  }
}

function webform_components_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  // Update all mandatory and weight values.
  foreach ($node->webform['components'] as $cid => $component) {
    if ($component['pid'] != $form_state['values']['components'][$cid]['pid'] || $component['weight'] != $form_state['values']['components'][$cid]['weight'] || $component['mandatory'] != $form_state['values']['components'][$cid]['mandatory']) {
      $component['weight'] = $form_state['values']['components'][$cid]['weight'];
      $component['mandatory'] = $form_state['values']['components'][$cid]['mandatory'];
      $component['pid'] = $form_state['values']['components'][$cid]['pid'];
      $component['nid'] = $node->nid;
      webform_component_update($component);
    }
  }

  if (isset($_POST['op']) && $_POST['op'] == t('Publish')) {
    $node->status = 1;
    node_save($node);
    drupal_set_message(t('Your webform has been published.'));
    return 'node/' . $node->nid;
  }
  elseif (isset($_POST['op']) && $_POST['op'] == t('Add')) {
    $component = $form_state['values']['add'];
    $form_state['redirect'] = array('node/' . $node->nid . '/webform/components/new/' . $component['type'], 'name=' . urlencode($component['name']) . '&mandatory=' . $component['mandatory'] . '&pid=' . $component['pid'] . '&weight=' . $component['weight']);
  }
  else {
    drupal_set_message(t('The component positions and mandatory values have been updated.'));

    // Since Webform components have been updated but the node itself has not
    // been saved, it is necessary to explicitly clear the cache to make sure
    // the updated webform is visible to anonymous users.
    cache_clear_all();
  }
}

function webform_component_edit_form(&$form_state, $node, $component, $clone = FALSE) {
  drupal_set_title(t('Edit component: @name', array('@name' => $component['name'])));
  drupal_add_css(drupal_get_path('module', 'webform') . '/css/webform-admin.css', 'theme', 'all', FALSE);

  // Print the correct field type specification.
  // We always need: name and description.
  $form = array(
    '#tree' => TRUE,
  );
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $component['type'],
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => isset($component['cid']) ? $component['cid'] : NULL,
  );
  $form['clone'] = array(
    '#type' => 'value',
    '#value' => $clone,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#default_value' => $component['name'],
    '#title' => t('Label'),
    '#description' => t('This is used as a descriptive label when displaying this form element.'),
    '#required' => TRUE,
    '#weight' => -10,
    '#maxlength' => 255,
  );
  $form['form_key'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($component['form_key']) ? _webform_safe_name($component['name']) : $component['form_key'],
    '#title' => t('Field Key'),
    '#description' => t('Enter a machine readable key for this form element. May contain only lowercase alphanumeric characters and underscores. This key will be used as the name attribute of the form element. This value has no effect on the way data is saved, but may be helpful if using Additional Processing or Validation code.'),
    '#required' => TRUE,
    '#weight' => -9,
  );
  $form['extra']['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
    '#title' => t('Description'),
    '#description' => t('A short description of the field used as help for the user when he/she uses the form.') . theme('webform_token_help'),
    '#weight' => -1,
  );

  // Display settings.
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 8,
  );

  // Validation settings.
  $form['validation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Validation'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 5,
  );
  if (webform_component_feature($component['type'], 'required')) {
    $form['validation']['mandatory'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mandatory'),
      '#default_value' => ($component['mandatory'] == '1' ? TRUE : FALSE),
      '#description' => t('Check this option if the user must enter a value.'),
      '#weight' => -1,
      '#parents' => array('mandatory'),
    );
  }

  // Position settings, only shown if JavaScript is disabled.
  $form['position'] = array(
    '#type' => 'fieldset',
    '#title' => t('Position'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
    '#weight' => 20,
    '#attributes' => array('class' => 'webform-position'),
  );

  if (variable_get('webform_enable_fieldset', TRUE) && is_array($node->webform['components'])) {
    $options = array('0' => t('Root'));
    foreach ($node->webform['components'] as $existing_cid => $value) {
      if (webform_component_feature($value['type'], 'group') && (!isset($component['cid']) || $existing_cid != $component['cid'])) {
        $options[$existing_cid] = $value['name'];
      }
    }
    $form['position']['pid'] = array(
      '#type' => 'select',
      '#title' => t('Parent Fieldset'),
      '#default_value' => $component['pid'],
      '#description' => t('Optional. You may organize your form by placing this component inside another fieldset.'),
      '#options' => $options,
      '#weight' => 3,
    );
  }
  $form['position']['weight'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Weight'),
    '#default_value' => $component['weight'],
    '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
    '#weight' => 4,
  );

  // Add conditional fields.
  $conditional_components = array();
  $counter = 0;
  $last_pagebreak_slice = 0;
  foreach ($node->webform['components'] as $cid => $test_component) {
    // Only components before the pagebreak can be considered.
    if ($test_component['type'] == 'pagebreak') {
      $last_pagebreak_slice = $counter;
    }
    if (isset($component['cid']) && $cid == $component['cid']) {
      break;
    }
    if (webform_component_feature($test_component['type'], 'conditional')) {
      $conditional_components[$cid] = $test_component;
    }
    $counter++;
  }
  if ($component['type'] != 'pagebreak') {
    $fieldset_description = t('Create a rule to control whether or not to skip this page.');
  }
  else {
    $fieldset_description = t('Create a rule to control whether or not to show this form element.');
  }
  $conditional_components = array_slice($conditional_components, 0, $last_pagebreak_slice, TRUE);
  $form['conditional'] = array(
    '#weight' => 10,
    '#type' => 'fieldset',
    '#title' => t('Conditional rules'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Create a rule to control whether or not to show this form element.'),
    '#tree' => FALSE,
  );
  $form['conditional']['extra'] = array(
    '#tree' => TRUE,
  );
  $form['conditional']['extra']['conditional_component'] = array(
    '#type' => 'select',
    '#title' => t('Component'),
    '#options' => webform_component_list($node, $conditional_components, FALSE, TRUE),
    '#description' => t('Select another component to decide whether to show or hide this component. You can only select components occurring before the most recent pagebreak.'),
    '#default_value' => $component['extra']['conditional_component'],
  );
  $form['conditional']['extra']['conditional_operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#options' => array(
      '=' => t('Is one of'),
      '!=' => t('Is not one of')
    ),
    '#description' => t('Determines whether the list below is inclusive or exclusive.'),
    '#default_value' => $component['extra']['conditional_operator'],
  );
  $form['conditional']['extra']['conditional_values'] = array(
    '#type' => 'textarea',
    '#title' => t('Values'),
    '#description' => t('List values, one per line, that will trigger this action. If you leave this blank, this component will always display.'),
    '#default_value' => $component['extra']['conditional_values'],
  );
  if (empty($conditional_components)) {
    $form['conditional']['#access'] = FALSE;
  }

  // Add the fields specific to this component type:
  $additional_form_elements = (array) webform_component_invoke($component['type'], 'edit', $component);
  if (empty($additional_form_elements)) {
    drupal_set_message(t('The webform component of type @type does not have an edit function defined.', array('@type' => $component['type'])));
  }

  // Merge the additional fields with the current fields:
  if (isset($additional_form_elements['extra'])) {
    $form['extra'] = array_merge($form['extra'], $additional_form_elements['extra']);
    unset($additional_form_elements['extra']);
  }
  if (isset($additional_form_elements['position'])) {
    $form['position'] = array_merge($form['position'], $additional_form_elements['position']);
    unset($additional_form_elements['position']);
  }
  if (isset($additional_form_elements['display'])) {
    $form['display'] = array_merge($form['display'], $additional_form_elements['display']);
    unset($additional_form_elements['display']);
  }
  else {
    unset($form['display']);
  }
  if (isset($additional_form_elements['validation'])) {
    $form['validation'] = array_merge($form['validation'], $additional_form_elements['validation']);
    unset($additional_form_elements['validation']);
  }
  elseif (count(element_children($form['validation'])) == 0) {
    unset($form['validation']);
  }
  $form = array_merge($form, $additional_form_elements);

  // Add the submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Field name validation for the webform unique key. Must be alphanumeric.
 */
function webform_component_edit_form_validate($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['form_key'])) {
    form_set_error('form_key', t('The field key %field_key is invalid. Please include only lowercase alphanumeric characters and underscores.', array('%field_key' => $form_state['values']['form_key'])));
  }

  foreach ($node->webform['components'] as $cid => $component) {
    if (($component['cid'] != $form_state['values']['cid'] || $form_state['values']['clone']) && ($component['pid'] == $form_state['values']['pid']) && (strcasecmp($component['form_key'], $form_state['values']['form_key']) == 0)) {
      form_set_error('form_key', t('The field key %field_key is already in use by the field labeled %existing_field. Please use a unique key.', array('%field_key' => $form_state['values']['form_key'], '%existing_field' => $component['name'])));
    }
  }
}

function webform_component_edit_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  // Remove empty extra values.
  if (isset($form_state['values']['extra'])) {
    foreach ($form_state['values']['extra'] as $key => $value) {
      if ($value  === '') {
        unset($form_state['values']['extra'][$key]);
      }
    }
  }

  // Remove empty attribute values.
  if (isset($form_state['values']['extra']['attributes'])) {
    foreach ($form_state['values']['extra']['attributes'] as $key => $value) {
      if ($value === '') {
        unset($form_state['values']['extra']['attributes'][$key]);
      }
    }
  }

  if ($form_state['values']['clone']) {
    drupal_set_message(t('Component %name cloned.', array('%name' => $form_state['values']['name'])));
    webform_component_clone($node, $form_state['values']);
  }
  elseif (!empty($form_state['values']['cid'])) {
    drupal_set_message(t('Component %name updated.', array('%name' => $form_state['values']['name'])));
    webform_component_update($form_state['values']);
  }
  else {
    drupal_set_message(t('New component %name added.', array('%name' => $form_state['values']['name'])));
    $cid = webform_component_insert($form_state['values']);
  }

  // Since Webform components have been updated but the node itself has not
  // been saved, it is necessary to explicitly clear the cache to make sure
  // the updated webform is visible to anonymous users.
  cache_clear_all();

  $form_state['redirect'] = array('node/' . $form_state['values']['nid'] . '/webform/components', isset($cid) ? 'cid=' . $cid : '');
}

function webform_component_delete_form($form_state, $node, $component) {
  $cid = $component['cid'];

  $form = array();
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['component'] = array(
    '#type' => 'value',
    '#value' => $component,
  );

  if (webform_component_feature($node->webform['components'][$cid]['type'], 'group')) {
    $question = t('Delete the %name fieldset?', array('%name' => $node->webform['components'][$cid]['name']));
    $description = t('This will immediately delete the %name fieldset and all children elements within %name from the %webform webform. This cannot be undone.', array('%name' => $node->webform['components'][$cid]['name'], '%webform' => $node->title));
  }
  else {
    $question = t('Delete the %name component?', array('%name' => $node->webform['components'][$cid]['name']));
    $description = t('This will immediately delete the %name component from the %webform webform. This cannot be undone.', array('%name' => $node->webform['components'][$cid]['name'], '%webform' => $node->title));
  }

  return confirm_form($form, $question, 'node/' . $node->nid . '/webform/components', $description, t('Delete'));
}

function webform_component_delete_form_submit($form, &$form_state) {
  drupal_set_message(t('Component %name deleted.', array('%name' => $form_state['values']['component']['name'])));
  webform_component_delete($form_state['values']['node'], $form_state['values']['component']);
  $form_state['redirect'] = 'node/' . $form_state['values']['node']->nid . '/webform/components';
}

/**
 * Insert a new component into the database.
 *
 * @param $component
 *   A full component containing fields from the component form.
 */
function webform_component_insert(&$component) {
  // Allow modules to modify the component before saving.
  foreach (module_implements('webform_component_presave') as $module) {
    $function = $module . '_webform_component_presave';
    $function($component);
  }

  db_lock_table('webform_component');
  $component['cid'] = isset($component['cid']) ? $component['cid'] : db_result(db_query('SELECT MAX(cid) FROM {webform_component} WHERE nid = %d', $component['nid'])) + 1;
  $component['value'] = isset($component['value']) ? $component['value'] : NULL;
  $component['mandatory'] = isset($component['mandatory']) ? $component['mandatory'] : 0;
  db_query("INSERT INTO {webform_component} (nid, cid, pid, form_key, name, type, value, extra, mandatory, weight) VALUES (%d, %d, %d, '%s', '%s', '%s', '%s', '%s', %d, %d)", $component['nid'], $component['cid'], $component['pid'], $component['form_key'], $component['name'], $component['type'], $component['value'], serialize($component['extra']), $component['mandatory'], $component['weight']);
  db_unlock_tables();

  // Post-insert actions.
  module_invoke_all('webform_component_insert', $component);

  return $component['cid'];
}

/**
 * Update an existing component with new values.
 *
 * @param $component
 *   A full component containing a nid, cid, and all other fields from the
 *   component form. Additional properties are stored in the extra array.
 */
function webform_component_update($component) {
  // Allow modules to modify the component before saving.
  foreach (module_implements('webform_component_presave') as $module) {
    $function = $module . '_webform_component_presave';
    $function($component);
  }

  $component['value'] = isset($component['value']) ? $component['value'] : NULL;
  $component['mandatory'] = isset($component['mandatory']) ? $component['mandatory'] : 0;
  $success = db_query("UPDATE {webform_component} SET pid = %d, form_key = '%s', name = '%s', type = '%s', value = '%s', extra = '%s', mandatory = %d, weight = %d WHERE nid = %d AND cid = %d", $component['pid'], $component['form_key'], $component['name'], $component['type'], $component['value'], serialize($component['extra']), $component['mandatory'], $component['weight'], $component['nid'], $component['cid']);

  // Post-update actions.
  module_invoke_all('webform_component_update', $component);

  return $success;
}

function webform_component_delete($node, $component) {
  // Check if a delete function is available for this component. If so,
  // load all submissions and allow the component to delete each one.

  webform_component_include($component['type']);
  $delete_function = '_webform_delete_' . $component['type'];
  if (function_exists($delete_function)) {
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    $submissions = webform_get_submissions($node->nid);
    foreach ($submissions as $submission) {
      if (isset($submission->data[$component['cid']])) {
        webform_component_invoke($component['type'], 'delete', $component, $submission->data[$component['cid']]['value']);
      }
    }
  }

  // Remove database entries.
  db_query('DELETE FROM {webform_component} WHERE nid = %d AND cid = %d', $node->nid, $component['cid']);
  db_query('DELETE FROM {webform_submitted_data} WHERE nid = %d AND cid = %d', $node->nid, $component['cid']);

  // Delete all elements under this element.
  $result = db_query('SELECT cid FROM {webform_component} WHERE nid = %d AND pid = %d', $node->nid, $component['cid']);
  while ($row = db_fetch_object($result)) {
    $child_component = $node->webform['components'][$row->cid];
    webform_component_delete($node, $child_component);
  }

  // Post-delete actions.
  module_invoke_all('webform_component_delete', $component);
}

/**
 * Recursively insert components into the database.
 * @param $node
 *   The node object containing the current webform.
 * @param $component
 *   A full component containing fields from the component form.
 */
function webform_component_clone(&$node, &$component) {
  $original_cid = $component['cid'];
  unset($component['cid']);
  $new_cid = webform_component_insert($component);
  if (webform_component_feature($component['type'], 'group')) {
    foreach ($node->webform['components'] as $cid => $child_component) {
      if ($child_component['pid'] == $original_cid) {
        $child_component['pid'] = $new_cid;
        webform_component_clone($node, $child_component);
      }
    }
  }
  return $new_cid;
}

/**
 * Check if a component has a particular feature.
 */
function webform_component_feature($type, $feature) {
  $components = webform_components();
  $defaults = array(
    'csv' => TRUE,
    'email' => TRUE,
    'email_address' => FALSE,
    'email_name' => FALSE,
    'required' => TRUE,
    'conditional' => TRUE,
    'spam_analysis' => FALSE,
    'group' => FALSE,
  );
  return isset($components[$type]['features'][$feature]) ? $components[$type]['features'][$feature] : $defaults[$feature];
}

/**
 * Create a list of components suitable for a select list.
 *
 * @param $node
 *   The webform node.
 * @param $component_filter
 *   Either an array of components, or a string containing a feature name (csv,
 *   email, required, conditional) on which this list of components will be
 *   restricted.
 * @param $indent
 *   Indent components placed under fieldsets with hyphens.
 * @param $optgroups
 *   Determine if pagebreaks should be converted to option groups in the
 *   returned list of options.
 */
function webform_component_list($node, $component_filter = NULL, $indent = TRUE, $optgroups = FALSE) {
  $options = array();
  $page_names = array();

  $components = is_array($component_filter) ? $component_filter : $node->webform['components'];
  $feature = is_string($component_filter) ? $component_filter : NULL;

  foreach ($components as $cid => $component) {
    if (!isset($feature) || webform_component_feature($component['type'], $feature) || ($indent && webform_component_feature($component['type'], 'group'))) {
      $prefix = '';
      $page_num = $component['page_num'];
      $page_index = 'p' . $page_num;
      if ($indent && ($parent_count = count(webform_component_parent_keys($node, $component)) - 1)) {
        $prefix = str_repeat('-', $parent_count);
      }
      if ($optgroups && $component['type'] == 'pagebreak') {
        $page_names[$page_index] = $component['name'];
      }
      elseif ($optgroups && $page_num > 1) {
        $options[$page_index][$cid] = $prefix . $component['name'];
      }
      else {
        $options[$cid] = $prefix . $component['name'];
      }
    }
  }

  if ($optgroups) {
    $grouped_options = $options;
    $options = array();
    foreach ($grouped_options as $key => $values) {
      if (is_array($values) && isset($page_names[$key])) {
        $options[$page_names[$key]] = $values;
      }
      else {
        $options[$key] = $values;
      }
    }
  }

  return $options;
}

/**
 * A Form API process function to expand a component list into checkboxes.
 */
function webform_component_select($element) {
  // Split the select list into checkboxes.
  foreach ($element['#options'] as $key => $label) {
    $indents = 0;
    $label = preg_replace('/^([\-])+/', '', $label, -1, $indents);
    $element[$key] = array(
      '#title' => $label,
      '#type' => 'checkbox',
      '#default_value' => array_search($key, $element['#value']) !== FALSE,
      '#parents' => array_merge($element['#parents'], array($key)),
      '#indent' => $indents,
    );
  }

  // Set value to NULL so that Drupal doesn't output "Array" as the markup.
  $element['#value'] = NULL;
  $element['#type'] = 'markup';
  $element['#theme'] = 'webform_component_select';

  return $element;
}

/**
 * Theme the contents of a Webform component select element.
 */
function theme_webform_component_select($element) {
  drupal_add_js('misc/tableselect.js');
  drupal_add_js(drupal_get_path('module', 'webform') . '/js/webform-admin.js', 'module', 'header', FALSE, TRUE, FALSE);

  $rows = array();
  $header = array();
  $header = array(array('class' => 'select-all', 'data' => ' ' . t('Include all components')));
  foreach (element_children($element) as $key) {
    $rows[] = array(
      theme('indentation', $element[$key]['#indent']) . drupal_render($element[$key]),
    );
  }
  $element['#collapsible'] = TRUE;
  $element['#collapsed'] = TRUE;

  $element['#attributes']['class'] = 'webform-component-select-table';
  $element['#children'] = theme('table', $header, $rows);;
  return theme('fieldset', $element);
}

/**
 * Find a components parents within a node.
 */
function webform_component_parent_keys($node, $component) {
  $parents = array($component['form_key']);
  $pid = $component['pid'];
  while ($pid) {
    $parents[] = $node->webform['components'][$pid]['form_key'];
    $pid = $node->webform['components'][$pid]['pid'];
  }
  return array_reverse($parents);
}

/**
 * Populate a component with the defaults for that type.
 */
function webform_component_defaults(&$component) {
  if ($defaults = webform_component_invoke($component['type'], 'defaults')) {
    foreach ($defaults as $key => $default) {
      if (!isset($component[$key])) {
        $component[$key] = $default;
      }
    }
    foreach ($defaults['extra'] as $extra => $default) {
      if (!isset($component['extra'][$extra])) {
        $component['extra'][$extra] = $default;
      }
    }
    $component['extra'] += array(
      'conditional_component' => '',
      'conditional_operator' => '=',
      'conditional_values' => '',
    );
  }
}

/**
 * Validate an element value is unique with no duplicates in the database.
 */
function webform_validate_unique($element, $form_state) {
  $nid = $form_state['values']['details']['nid'];
  $sid = $form_state['values']['details']['sid'];
  $count = db_result(db_query("SELECT count(*) FROM {webform_submitted_data} WHERE nid = %d AND cid = %d AND sid <> %d AND LOWER(data) = '%s'", $nid, $element['#webform_component']['cid'], $sid, $element['#value']));
  if ($count) {
    form_error($element, t('The value %value has already been submitted once for the %title field. You may have already submitted this form, or you need to use a different value.', array('%value' => $element['#value'], '%title' => $element['#title'])));
  }
}
