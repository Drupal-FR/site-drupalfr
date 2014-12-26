<?php

/**
 * @file
 * Administrative page and form callbacks for Customizable Products.
 */


/**
 * Page callback: displays an edit page for the specified line item type.
 *
 * @param $type
 *   The machine-name of the line item type whose edit form should be displayed.
 */
function commerce_custom_product_line_item_type_edit($type) {
  $line_item_type = commerce_line_item_type_load($type);
  return drupal_get_form('commerce_custom_product_line_item_type_form', $line_item_type);
}

/**
 * Form callback: builds the customizable product line item type form.
 */
function commerce_custom_product_line_item_type_form($form, &$form_state, $line_item_type) {
  // Store the initial shipping service in the form state.
  $form_state['line_item_type'] = $line_item_type;

  $form['line_item_type'] = array(
    '#tree' => TRUE,
  );

  $form['line_item_type']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $line_item_type['name'],
    '#description' => t('The human-readable name of the customizable product line item type. It is recommended that this title begin with a capital letter and contain only letters, numbers, and spaces.'),
    '#required' => TRUE,
    '#size' => 32,
    '#maxlength' => 255,
    '#field_suffix' => ' <small id="edit-line-item-type-name-suffix">' . t('Machine name: @type', array('@type' => $line_item_type['type'])) . '</small>',
  );

  if (empty($line_item_type['type'])) {
    $form['line_item_type']['type'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#default_value' => $line_item_type['type'],
      '#maxlength' => 32,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'commerce_line_item_type_load',
        'source' => array('line_item_type', 'name'),
      ),
      '#description' => t('The machine-name of this customizable product line item type. This name must contain only lowercase letters, numbers, and underscores. It must be unique.'),
    );
  }
  else {
    $form['line_item_type']['type'] = array(
      '#type' => 'value',
      '#value' => $line_item_type['type'],
    );
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save line item type'),
  );

  if (!empty($form_state['line_item_type']['type']) &&
    commerce_custom_product_line_item_type_delete_access($form_state['line_item_type']['type'])) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete line item type'),
      '#suffix' => l(t('Cancel'), 'admin/commerce/config/line-items'),
      '#submit' => array('commerce_custom_product_line_item_type_form_delete_submit'),
      '#weight' => 45,
    );
  }
  else {
    $form['actions']['submit']['#suffix'] = l(t('Cancel'), 'admin/commerce/config/line-items');
  }

  return $form;
}

/**
 * Submit handler: saves the customizable product line item type.
 */
function commerce_custom_product_line_item_type_form_submit($form, &$form_state) {
  $line_item_type = $form_state['values']['line_item_type'];

  // Save the line item type.
  $op = commerce_custom_product_line_item_type_save($line_item_type);

  // Display a message and redirect to the line item type overview table.
  if ($op) {
    drupal_set_message(t('%name line item type saved.', array('%name' => $line_item_type['name'])));
  }
  else {
    drupal_set_message(t('Saving the line item type was prevented by an error.', 'error'));
  }

  $form_state['redirect'] = 'admin/commerce/config/line-items';
}

/**
 * Submit handler: redirects to the delete confirmation form.
 */
function commerce_custom_product_line_item_type_form_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/config/line-items/' . strtr($form_state['line_item_type']['type'], '_', '-') . '/delete';
}

/**
 * Form callback: builds the customizable product line item type delete form.
 */
function commerce_custom_product_line_item_type_delete_form($form, &$form_state, $type) {
  // Load the line item type to create a meaningful message.
  $line_item_type = commerce_line_item_type_load($type);

  $form['line_item_type'] = array(
    '#type' => 'value',
    '#value' => $line_item_type,
  );

  $form = confirm_form($form,
    t('Are you sure you want to delete the %name line item type?', array('%name' => $line_item_type['name'])),
    'admin/commerce/config/line-items',
    '<p>' . t('Deleting this line item type cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Form callback: deletes the customizable product line item type.
 */
function commerce_custom_product_line_item_type_delete_form_submit($form, &$form_state) {
  // Reload the line item type information.
  $line_item_type = commerce_line_item_type_load($form_state['values']['line_item_type']['type']);

  // Delete the line item type.
  commerce_custom_product_line_item_type_delete($line_item_type['type']);

  // Display a message and redirect to the line item type overview table.
  drupal_set_message(t('The %name line item type has been deleted.', array('%name' => $line_item_type['name'])));
  $form_state['redirect'] = 'admin/commerce/config/line-items';
}
