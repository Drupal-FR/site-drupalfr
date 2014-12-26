<?php

/**
 * @file
 * Settings for Commerce Billy.
 */

/**
 * Admin settings form for invoice generation.
 */
function commerce_billy_admin_form($form, &$form_state) {
  // Confirm form for changing the last number.
  if (isset($form_state['confirm_nr_change']) && !empty($form_state['confirm_nr_change'])) {
    $override = $form_state['values']['commerce_billy_invoice_nr_override'];
    $form['nr_override'] = array('#type' => 'value', '#value' => $override);
    return confirm_form($form,
      t('Are you sure you want to change the current invoice id?'),
      'admin/commerce/config/billy-invoice',
      t('Changing the id might lead to gaps in your invoice numbers, causing troubles in the accounting system.'));
  }

  $form['#theme'] = 'system_settings_form';

  $last_number = commerce_billy_query_variable('commerce_billy_invoice_nr_last');
  if (!isset($last_number['id'])) {
    $last_number = array('id' => 0);
  }

  $invoice_nr_options = array(
    COMMERCE_BILLY_INVOICE_METHOD_INFINITE => t('Infinite (one single number, that is never reset, and incremented at each invoice generation)'),
    COMMERCE_BILLY_INVOICE_METHOD_YEARLY => t('Reset every year, with an id incremented at each invoice generation (e.g. [date:custom:Y]-{id}, resulting in @invoice_number)', array('@invoice_number' => date('Y') . '-' . $last_number['id'])),
    COMMERCE_BILLY_INVOICE_METHOD_MONTHLY => t('Reset every month, with an id incremented at each invoice generation (e.g. [date:custom:Y-m]-{id}, resulting in @invoice_number)', array('@invoice_number' => date('Y-m') . '-' . $last_number['id'])),
  );

  $form['commerce_billy_invoice_nr_method'] = array(
    '#type' => 'radios',
    '#title' => t('Invoice number generation method'),
    '#default_value' => variable_get('commerce_billy_invoice_nr_method', COMMERCE_BILLY_INVOICE_METHOD_YEARLY),
    '#options' => $invoice_nr_options,
  );

  $pattern_help_text = t('In addition to the generation method, a pattern for the
    invoice number can be set, e.g. to pre- or suffix the calculated number. The
    placeholder "{id}" is replaced with the generated number and *must* be
    included in the pattern. Tokens can be used, e.g. [date:custom:Y-m-d].
    If you are using the yearly pattern, a token for the current year must be
    included. For the montly pattern, tokens for the current month and year must
    be included.');
  $form['commerce_billy_invoice_nr_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_billy_invoice_nr_pattern', '[date:custom:Y]-{id}'),
    '#description' => $pattern_help_text,
  );

  $form['commerce_billy_invoice_nr_override'] = array(
    '#type' => 'textfield',
    '#title' => t('Current invoice id'),
    '#description' => t('Use with caution! Only values bigger than the original value allowed. Next id being used: @next.', array('@next' => $last_number['id'] + 1)),
    '#size' => 10,
    '#default_value' => $last_number['id'],
  );

  $form['commerce_billy_auto_invoice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically invoice orders on checkout completion.'),
    '#default_value' => variable_get('commerce_billy_auto_invoice', FALSE),
    '#description' => t('If set, the order state is set to \'invoiced\' on order completion and an invoice number is generated. Otherwise an order has to be manually moved to the state "invoiced" to generated the invoice number. Changing this value requires a cache-clear.'),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Validation handler for Commerce Billy admin form.
 */
function commerce_billy_admin_form_validate($form, &$form_state) {
  $pattern = $form_state['values']['commerce_billy_invoice_nr_pattern'];
  if (strpos($pattern, '{id}') === FALSE) {
    form_set_error('commerce_billy_invoice_nr_pattern', t('Invalid pattern. {id} is missing.'));
  }
  elseif ($pattern === '{id}') {
    form_set_error('commerce_billy_invoice_nr_pattern', t('You must include additional characters to {id}, as otherwise the invoice number might overlap with the order ID, leading the database exceptions.'));
  }

  // Check for valid characters.
  $test_nr = str_replace('{id}', 1, $pattern);
  $test_nr = token_replace($test_nr, array(), array('clear' => TRUE));
  if (!commerce_order_validate_number_characters($test_nr)) {
    form_set_error('commerce_billy_invoice_nr_pattern', t('Invalid pattern. Only alphanumeric values, underscores and dashes are allowed.'));
  }
}

/**
 * Submit handler for Commerce Billy admin form.
 */
function commerce_billy_admin_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);

  // Prepare confirmation form for invoice number override.
  if (isset($form_state['values']['commerce_billy_invoice_nr_method'])) {
    $override = $form_state['values']['commerce_billy_invoice_nr_override'];
    $last_number = commerce_billy_query_variable('commerce_billy_invoice_nr_last');
    $last_number['id'] = isset($last_number['id']) ? $last_number['id'] : 0;
    if ($override && $override != $last_number['id'] && $override > $last_number['id']) {
      $form_state['rebuild'] = TRUE;
      $form_state['confirm_nr_change'] = TRUE;
    }
  }
  // Invoice number override has been confirmed.
  elseif (isset($form_state['values']['nr_override']) && isset($form_state['values']['confirm']) && !empty($form_state['values']['confirm'])) {
    $last_number = commerce_billy_query_variable('commerce_billy_invoice_nr_last');
    $last_number['id'] = $form_state['values']['nr_override'];
    variable_set('commerce_billy_invoice_nr_last', $last_number);
    drupal_set_message('Your invoice number has successfully been overridden.');
    return;
  }

  // Save all relevant settings.
  foreach ($form_state['values'] as $key => $value) {
    if ($key != 'commerce_billy_invoice_nr_override') {
      variable_set($key, $value);
    }
  }

  if (!isset($form_state['confirm_nr_change'])) {
    // No number override, default success message.
    drupal_set_message(t('The configuration options for Commerce Billy have been saved.'));
  }
  else {
    // Number override -> confirm form.
    drupal_set_message(t('Basic configuration options for Commerce Billy have been saved. Change of invoice number requires confirmation.'));
  }
}
