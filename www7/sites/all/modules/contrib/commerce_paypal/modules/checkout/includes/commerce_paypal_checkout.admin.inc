<?php

/**
 * @file
 * Administrative forms for the Paypal Checkout module.
 */

/**
 * Form callback: allows the user to capture a prior authorization.
 */
function commerce_paypal_checkout_capture_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;
  $balance = commerce_payment_order_balance($order);


  if ($balance['amount'] > 0 && $balance['amount'] < $transaction->amount) {
    $default_amount = $balance['amount'];
  }
  else {
    $default_amount = $transaction->amount;
  }

  // Convert the price amount to a user friendly decimal value.
  $default_amount = number_format(commerce_currency_amount_to_decimal($default_amount, $transaction->currency_code), 2, '.', '');

  $description = implode('<br />', array(
    t('Authorization: @amount', array('@amount' => commerce_currency_format($transaction->amount, $transaction->currency_code))),
    t('Order balance: @balance', array('@balance' => commerce_currency_format($balance['amount'], $balance['currency_code']))),
  ));

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Capture amount'),
    '#description' => $description,
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to capture?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Capture'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: ensure a valid amount is given.
 */
function commerce_paypal_checkout_capture_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for capture.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to capture.'));
  }

  // Ensure the amount is within the allowed limit for PayPal authorizations.
  $authorization_amount = commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code);
  $authorization_amount_upper = commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code) + 75;

  if ($amount > $authorization_amount * 1.15 || $amount > $authorization_amount_upper) {
    form_set_error('amount', t('You cannot capture an amount $75 or 115% greater than the authorization amount in PayPal Checkout.'));
  }

  // If the authorization has expired, display an error message and redirect.
  if (REQUEST_TIME - $transaction->created > 86400 * 29) {
    drupal_set_message(t('This authorization has passed its 29 day limit and cannot be captured.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a prior authorization capture via PayPal Checkout.
 */
function commerce_paypal_checkout_capture_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $order = $form_state['order'];
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_total = $order_wrapper->commerce_order_total->value();
  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $transaction->currency_code);
  $api_client = commerce_paypal_checkout_api_client($form_state['payment_method']['settings']);

  $params = array(
    'final_capture' => $order_total['amount'] == $amount,
    'amount' => array(
      'value' => commerce_paypal_checkout_price_amount($amount, $transaction->currency_code),
      'currency_code' => $transaction->currency_code,
    ),
  );

  try {
    // If the transaction was authorized more than 3 days ago, we need to
    // re-authorize the payment.
    if (time() >= ($transaction->created + (86400 * 3))) {
      $api_client->reAuthorizePayment($transaction->remote_id, array('amount' => $params['amount']));
    }

    $response = $api_client->capturePayment($transaction->remote_id, $params);
    if (strtolower($response['status']) == 'completed') {
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    }
    else {
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    }
    $transaction->remote_id = $response['id'];
    $transaction->amount = $amount;
    $transaction->remote_status = $response['status'];
    // Note the capture in the transaction message.
    $transaction->message .= '<br />' . t('Captured: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
    $transaction->payload[REQUEST_TIME . '-capture'] = $response;
    // Save the updated original transaction.
    commerce_payment_transaction_save($transaction);
  }
  catch (PayPalCheckoutHttpException $exception) {
    watchdog_exception('commerce_paypal_checkout', $exception);
    // Display an error message but leave the transaction pending.
    drupal_set_message(t('Prior authorization capture failed, so the transaction will remain in a pending status.'), 'error');
  }

  // Redirect back to the current order payment page.
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to void a transaction.
 */
function commerce_paypal_checkout_void_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $form['markup'] = array(
    '#markup' => t('Are you sure that you want to void this transaction?'),
  );

  $form = confirm_form($form,
    t('Are you sure that you want to void this transaction?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Void'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit handler: process the void request.
 */
function commerce_paypal_checkout_void_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $api_client = commerce_paypal_checkout_api_client($form_state['payment_method']['settings']);

  try {
    // This API call doesn't return content, so nothing to check, if no
    // exception is thrown, this means the void operation succeeded.
    $api_client->voidPayment($transaction->remote_id);
    drupal_set_message(t('Transaction successfully voided.'));

    // Set the remote and local status accordingly.
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->remote_status = COMMERCE_CREDIT_VOID;

    // Update the transaction message to show that it has been voided.
    $transaction->message .= '<br />' . t('Voided: @date', array('@date' => format_date(REQUEST_TIME, 'short')));
    commerce_payment_transaction_save($transaction);
  }
  catch (PayPalCheckoutHttpException $exception) {
    drupal_set_message(t('The void operation failed, so the transaction will remain in a pending status.'), 'error');
  }
  $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['order']->order_id . '/payment';
}

/**
 * Form callback: allows the user to issue a credit on a prior transaction.
 */
function commerce_paypal_checkout_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  // Load and store the payment method instance for this transaction.
  $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
  $form_state['payment_method'] = $payment_method;

  $default_amount = number_format(commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code), 2, '.', '');

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Refund amount'),
    '#description' => t('Enter the amount to be refunded back to the PayPal account.'),
    '#default_value' => $default_amount,
    '#field_suffix' => check_plain($transaction->currency_code),
    '#size' => 16,
  );

  $form = confirm_form($form,
    t('What amount do you want to refund?'),
    'admin/commerce/orders/' . $order->order_id . '/payment',
    '',
    t('Refund'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Validate handler: check the credit amount before attempting a refund request.
 */
function commerce_paypal_checkout_refund_form_validate($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $amount = $form_state['values']['amount'];

  // Ensure a positive numeric amount has been entered for refund.
  if (!is_numeric($amount) || $amount <= 0) {
    form_set_error('amount', t('You must specify a positive numeric amount to refund.'));
  }

  // Ensure the amount is less than or equal to the captured amount.
  if ($amount > commerce_currency_amount_to_decimal($transaction->amount, $transaction->currency_code)) {
    form_set_error('amount', t('You cannot refund more than you captured.'));
  }

  // If the transaction is older than 60 days, display an error message and redirect.
  if (time() - $transaction->created > 86400 * 60) {
    drupal_set_message(t('This transaction has passed its 60 day limit for issuing refunds.'), 'error');
    drupal_goto('admin/commerce/orders/' . $form_state['order']->order_id . '/payment');
  }
}

/**
 * Submit handler: process a refund request.
 */
function commerce_paypal_checkout_refund_form_submit($form, &$form_state) {
  $transaction = $form_state['transaction'];
  $payment_method = $form_state['payment_method'];

  $order = $form_state['order'];
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_total = $order_wrapper->commerce_order_total->value();
  $api_client = commerce_paypal_checkout_api_client($form_state['payment_method']['settings']);
  $amount = commerce_currency_decimal_to_amount($form_state['values']['amount'], $order_total['currency_code']);

  $params = array(
    'amount' => array(
      'value' => commerce_paypal_checkout_price_amount($amount, $transaction->currency_code),
      'currency_code' => $transaction->currency_code,
    ),
  );
  try {
    $response = $api_client->refundPayment($transaction->remote_id, $params);
    // Not sure this can happen...
    if (strtolower($response['status']) != 'completed') {
      drupal_set_message(t('Refund failed'));
    }
    else {
      drupal_set_message(t('Refund for @amount issued successfully.', array('@amount' => commerce_currency_format($amount, $transaction->currency_code))));

      // Create a new transaction to record the credit.
      $credit_transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
      $credit_transaction->instance_id = $payment_method['instance_id'];
      $credit_transaction->remote_id = $response['id'];
      $credit_transaction->amount = $amount * -1;
      $credit_transaction->currency_code = $transaction->currency_code;
      $credit_transaction->payload[REQUEST_TIME] = $response;
      $credit_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $credit_transaction->remote_status = COMMERCE_CREDIT_CREDIT;
      $credit_transaction->message = t('Refunded to @remote_id.', array('@remote_id' => $transaction->remote_id));

      // Save the credit transaction.
      commerce_payment_transaction_save($credit_transaction);
    }
  }
  catch (PayPalCheckoutHttpException $exception) {
    watchdog_exception('commerce_paypal_checkout', $exception);
    drupal_set_message(t('Refund failed'));
  }

  $form_state['redirect'] = 'admin/commerce/orders/' . $order->order_id . '/payment';
}
