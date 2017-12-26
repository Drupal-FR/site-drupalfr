<?php

/**
 * @file
 * Credit-card helper functions for Drupal commerce.
 */

/**
 * Returns a set of credit card form elements that payment method modules can
 *   incorporate into their submission form callbacks.
 *
 * @param $fields
 *   An associative array specifying the fields that should be included on the
 *   credit card form. The card number and expiration fields are always present,
 *   and fields whose array keys listed below aren't set will be left out of the
 *   credit card form:
 *   - type: an array identifying supported card types using the keys of the
 *     return array from commerce_payment_credit_card_types().
 *   - owner: TRUE to include an owner name textfield.
 *   - start_date: TRUE to include start date select lists.
 *   - issue: boolean that when present enables an issue number field; if TRUE,
 *     makes the field required; if FALSE, makes the field optional.
 *   - code: text label to use for a security code / CVV textfield.
 *   - bank: TRUE to include a bank name textfield.
 * @param $default
 *   An array of default values for the available CC fields.
 *
 * @return
 *   A credit card form array for use in another form.
 */
function commerce_payment_credit_card_form($fields = array(), $default = array()) {
  // Merge default values into the default array.
  $default += array(
    'type' => '',
    'owner' => '',
    'number' => '',
    'start_month' => '',
    'start_year' => date('Y') - 5,
    'exp_month' => date('m'),
    'exp_year' => date('Y'),
    'issue' => '',
    'code' => '',
    'bank' => '',
  );

  $current_year_2 = date('y');
  $current_year_4 = date('Y');

  $form['credit_card'] = array(
    '#tree' => TRUE,
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'commerce_payment') . '/theme/commerce_payment.theme.css'),
    ),
  );

  // Add a card type selector if specified.
  if (isset($fields['type'])) {
    $form['credit_card']['type'] = array(
      '#type' => 'select',
      '#title' => t('Card type'),
      '#options' => array_intersect_key(commerce_payment_credit_card_types(), drupal_map_assoc((array) $fields['type'])),
      '#default_value' => $default['type'],
    );
    $form['credit_card']['valid_types'] = array(
      '#type' => 'value',
      '#value' => $fields['type'],
    );
  }
  else {
    $form['credit_card']['valid_types'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }

  // Add a field for the credit card owner if specified.
  if (isset($fields['owner'])) {
    $form['credit_card']['owner'] = array(
      '#type' => 'textfield',
      '#title' => t('Card owner'),
      '#default_value' => $default['owner'],
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#size' => 32,
    );
  }

  // Always add a field for the credit card number.
  $form['credit_card']['number'] = array(
    '#type' => 'textfield',
    '#title' => t('Card number'),
    '#default_value' => $default['number'],
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 19,
    '#size' => 20,
  );

  // Add fields for the credit card start date if specified.
  if (isset($fields['start_date'])) {
    $form['credit_card']['start_month'] = array(
      '#type' => 'select',
      '#title' => t('Start date'),
      '#options' => drupal_map_assoc(array_keys(commerce_months())),
      '#default_value' => strlen($default['start_month']) == 1 ? '0' . $default['start_month'] : $default['start_month'],
      '#required' => TRUE,
      '#prefix' => '<div class="commerce-credit-card-start">',
      '#suffix' => '<span class="commerce-month-year-divider">/</span>',
    );

    // Build a year select list that uses a 4 digit key with a 2 digit value.
    $options = array();

    for ($i = -10; $i < 1; $i++) {
      $options[$current_year_4 + $i] = str_pad($current_year_2 + $i, 2, '0', STR_PAD_LEFT);
    }

    $form['credit_card']['start_year'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default['start_year'],
      '#suffix' => '</div>',
    );
  }

  // Always add fields for the credit card expiration date.
  $form['credit_card']['exp_month'] = array(
    '#type' => 'select',
    '#title' => t('Expiration'),
    '#options' => drupal_map_assoc(array_keys(commerce_months())),
    '#default_value' => strlen($default['exp_month']) == 1 ? '0' . $default['exp_month'] : $default['exp_month'],
    '#required' => TRUE,
    '#prefix' => '<div class="commerce-credit-card-expiration">',
    '#suffix' => '<span class="commerce-month-year-divider">/</span>',
  );

  // Build a year select list that uses a 4 digit key with a 2 digit value.
  $options = array();

  for ($i = 0; $i < 20; $i++) {
    $options[$current_year_4 + $i] = str_pad($current_year_2 + $i, 2, '0', STR_PAD_LEFT);
  }

  $form['credit_card']['exp_year'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $default['exp_year'],
    '#suffix' => '</div>',
  );

  // Add a field for the card issue number if specified.
  if (isset($fields['issue'])) {
    $form['credit_card']['issue'] = array(
      '#type' => 'textfield',
      '#title' => t('Issue number', array(), array('context' => 'credit card issue number for card types that require it')),
      '#default_value' => $default['issue'],
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => empty($fields['issue']) ? FALSE : TRUE,
      '#maxlength' => 2,
      '#size' => 2,
    );
  }

  // Add a field for the security code if specified.
  if (isset($fields['code'])) {
    $form['credit_card']['code'] = array(
      '#type' => 'textfield',
      '#title' => !empty($fields['code']) ? $fields['code'] : t('Security code'),
      '#default_value' => $default['code'],
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 4,
      '#size' => 4,
    );
  }

  // Add a field for the issuing bank if specified.
  if (isset($fields['bank'])) {
    $form['credit_card']['bank'] = array(
      '#type' => 'textfield',
      '#title' => t('Issuing bank'),
      '#default_value' => $default['bank'],
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#size' => 32,
    );
  }

  return $form;
}

/**
 * Validates a set of credit card details entered via the credit card form.
 *
 * @param $details
 *   An array of credit card details as retrieved from the credit card array in
 *   the form values of a form containing the credit card form.
 * @param $settings
 *   Settings used for calling validation functions and setting form errors:
 *   - form_parents: an array of parent elements identifying where the credit
 *     card form was situated in the form array
 *
 * @return
 *   TRUE or FALSE indicating the validity of all the data.
 *
 * @see commerce_payment_credit_card_form()
 */
function commerce_payment_credit_card_validate($details, $settings) {
  $prefix = implode('][', $settings['form_parents']) . '][';
  $valid = TRUE;

  // Validate the credit card type.
  if (!empty($details['valid_types'])) {
    $type = commerce_payment_validate_credit_card_type($details['number'], $details['valid_types']);

    if ($type === FALSE) {
      form_set_error($prefix . 'type', t('You have entered a credit card number of an unsupported card type.'));
      $valid = FALSE;
    }
    elseif ($type != $details['type']) {
      form_set_error($prefix . 'number', t('You have entered a credit card number that does not match the type selected.'));
      $valid = FALSE;
    }
  }

  // Validate the credit card number.
  if (!commerce_payment_validate_credit_card_number($details['number'])) {
    form_set_error($prefix . 'number', t('You have entered an invalid credit card number.'));
    $valid = FALSE;
  }

  // Validate the expiration date.
  if (($invalid = commerce_payment_validate_credit_card_exp_date($details['exp_month'], $details['exp_year'])) !== TRUE) {
    form_set_error($prefix . 'exp_' . $invalid, t('You have entered an expired credit card.'));
    $valid = FALSE;
  }

  // Validate the security code if present.
  if (!empty($details['code']) && !commerce_payment_validate_credit_card_security_code($details['number'], $details['code'])) {
    form_set_error($prefix . 'code', t('You have entered an invalid card security code.'));
    $valid = FALSE;
  }

  // Validate the start date if present.
  if (isset($details['start_month']) && ($invalid = commerce_payment_validate_credit_card_start_date($details['start_month'], $details['start_year'])) !== TRUE) {
    form_set_error($prefix . 'start_' . $invalid, t('Your have entered an invalid start date.'));
    $valid = FALSE;
  }

  // Validate the issue number if present.
  if (isset($details['issue']) && !commerce_payment_validate_credit_card_issue($details['issue'])) {
    form_set_error($prefix . 'issue', t('You have entered an invalid issue number.'));
    $valid = FALSE;
  }

  return $valid;
}

/**
 * Validates a credit card number using an array of approved card types.
 *
 * @param int $number
 *   The credit card number to validate.
 * @param array $card_types
 *   An array of credit card types containing any of the keys from the array
 *   returned by commerce_payment_credit_card_types(). Only numbers determined
 *   to be of the types specified will pass validation. This determination is
 *   based on the length of the number and the valid number ranges for the
 *   various types of known credit card types.
 *
 * @return
 *   FALSE if a number is not valid based on approved credit card types or the
 *   credit card type if it is valid and could be determined.
 *
 * @see http://en.wikipedia.org/wiki/Bank_card_number#Issuer_Identification_Number_.28IIN.29
 * @see commerce_payment_credit_card_types()
 */
function commerce_payment_validate_credit_card_type($number, array $card_types = array()) {
  $type = CommercePaymentCreditCard::detectType($number);

  if (!$type || !in_array($type['id'], $card_types)) {
    return FALSE;
  }

  return $type['id'];
}

/**
 * Validates a credit card number using the Luhn algorithm.
 *
 * @param $number
 *   The credit card number to validate.
 *
 * @return
 *   TRUE or FALSE indicating the number's validity.
 *
 * @see http://www.merriampark.com/anatomycc.htm
 */
function commerce_payment_validate_credit_card_number($number) {
  $type = CommercePaymentCreditCard::detectType($number);
  return CommercePaymentCreditCard::validateNumber($number, $type);
}

/**
 * Validates a credit card start date.
 *
 * @param int $month
 *   The 1 or 2-digit numeric representation of the month, i.e. 1, 6, 12.
 * @param int $year
 *   The 4-digit numeric representation of the year, i.e. 2010.
 *
 * @return
 *   TRUE for cards whose start date is blank (both month and year) or in the
 *   past, 'year' or 'month' for expired cards indicating which value should
 *   receive the error.
 */
function commerce_payment_validate_credit_card_start_date($month, $year) {
  if (empty($month) && empty($year)) {
    return TRUE;
  }

  if (empty($month) || empty($year)) {
    return empty($month) ? 'month' : 'year';
  }

  if ($month < 1 || $month > 12) {
    return 'month';
  }

  if ($year > date('Y')) {
    return 'year';
  }
  elseif ($year == date('Y')) {
    if ($month > date('n')) {
      return 'month';
    }
  }

  return TRUE;
}

/**
 * Validates a credit card expiration date.
 *
 * @param int $month
 *   The 1 or 2-digit numeric representation of the month, i.e. 1, 6, 12.
 * @param int $year
 *   The 4-digit numeric representation of the year, i.e. 2010.
 *
 * @return
 *   TRUE for non-expired cards, 'year' or 'month' for expired cards indicating
 *   which value should receive the error.
 */
function commerce_payment_validate_credit_card_exp_date($month, $year) {
  return CommercePaymentCreditCard::validateExpirationDate($month, $year);
}

/**
 * Validates that an issue number is numeric if present.
 */
function commerce_payment_validate_credit_card_issue($issue) {
  if (empty($issue) || (is_numeric($issue) && $issue > 0)) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Validates a card security code based on the type of the credit card.
 *
 * @param $number
 *   The number of the credit card to validate the security code against.
 * @param $code
 *   The card security code to validate with the given number.
 *
 * @return
 *   TRUE or FALSE indicating the security code's validity.
 */
function commerce_payment_validate_credit_card_security_code($number, $code) {
  $type = CommercePaymentCreditCard::detectType($number);
  return CommercePaymentCreditCard::validateSecurityCode($code, $type);
}

/**
 * Returns an associative array of credit card types.
 *
 * Provides BC layer for CommercePaymentCreditCard::getTypeLabels().
 *
 * @see CommercePaymentCreditCard::getTypeLabels()
 *
 * @return array
 *   An array keyed by card type with card type label.
 */
function commerce_payment_credit_card_types() {
  return CommercePaymentCreditCard::getTypeLabels();
}

/**
 * Provides logic for listing card types and validating card details.
 */
final class CommercePaymentCreditCard {

  /**
   * Gets all available credit card types.
   *
   * @return array
   *   The credit card types.
   */
  public static function getTypes() {
    $definitions = array(
      'visa' => array(
        'id' => 'visa',
        'label' => t('Visa'),
        'number_prefixes' => array('4'),
      ),
      'mastercard' => array(
        'id' => 'mastercard',
        'label' => t('MasterCard'),
        'number_prefixes' => array('51-55', '222100-272099'),
      ),
      'maestro' => array(
        'id' => 'maestro',
        'label' => t('Maestro'),
        'number_prefixes' => array(
          '5018', '5020', '5038', '5612', '5893', '6304',
          '6759', '6761', '6762', '6763', '0604', '6390',
        ),
        'number_lengths' => array(12, 13, 14, 15, 16, 17, 18, 19),
      ),
      'amex' => array(
        'id' => 'amex',
        'label' => t('American Express'),
        'number_prefixes' => array('34', '37'),
        'number_lengths' => array(15),
        'security_code_length' => 4,
      ),
      'dc' => array(
        'id' => 'dc',
        'label' => t('Diners Club'),
        'number_prefixes' => array('300-305', '309', '36', '38', '39'),
        'number_lengths' => array(14),
      ),
      'discover' => array(
        'id' => 'discover',
        'label' => t('Discover Card'),
        'number_prefixes' => array('6011', '622126-622925', '644-649', '65'),
        'number_lengths' => array(16, 19),
      ),
      'jcb' => array(
        'id' => 'jcb',
        'label' => t('JCB'),
        'number_prefixes' => array('3528-3589'),
      ),
      'unionpay' => array(
        'id' => 'unionpay',
        'label' => t('UnionPay'),
        'number_prefixes' => array('62', '88'),
        'number_lengths' => array(16, 17, 18, 19),
        'uses_luhn' => FALSE,
      ),
    );

    foreach ($definitions as &$definition) {
      $definition += array(
        'number_lengths' => array(16),
        'security_code_length' => 3,
        'uses_luhn' => TRUE,
      );
    }

    return $definitions;
  }

  /**
   * Gets the labels of all available credit card types.
   *
   * @return array
   *   The labels, keyed by ID.
   */
  public static function getTypeLabels() {
    $type_labels = array();

    foreach (self::getTypes() as $type) {
      $type_labels[$type['id']] = $type['label'];
    }

    return $type_labels;
  }

  /**
   * Detects the credit card type based on the number.
   *
   * @param string $number
   *   The credit card number.
   *
   * @return array|false
   *   The credit card type, or NULL if unknown.
   */
  public static function detectType($number) {
    if (!is_numeric($number)) {
      return FALSE;
    }
    $types = self::getTypes();
    foreach ($types as $type) {
      foreach ($type['number_prefixes'] as $prefix) {
        if (self::matchPrefix($number, $prefix)) {
          return $type;
        }
      }
    }

    return FALSE;
  }

  /**
   * Checks whether the given credit card number matches the given prefix.
   *
   * @param string $number
   *   The credit card number.
   * @param string $prefix
   *   The prefix to match against. Can be a single number such as '43' or a
   *   range such as '30-35'.
   *
   * @return bool
   *   TRUE if the credit card number matches the prefix, FALSE otherwise.
   */
  public static function matchPrefix($number, $prefix) {
    if (is_numeric($prefix)) {
      return substr($number, 0, strlen($prefix)) == $prefix;
    }
    else {
      list($start, $end) = explode('-', $prefix);
      $number = substr($number, 0, strlen($start));
      return $number >= $start && $number <= $end;
    }
  }

  /**
   * Validates the given credit card number.
   *
   * @param string $number
   *   The credit card number.
   * @param array $type
   *   The credit card type.
   *
   * @return bool
   *   TRUE if the credit card number is valid, FALSE otherwise.
   */
  public static function validateNumber($number, array $type) {
    if (!is_numeric($number)) {
      return FALSE;
    }
    if (!in_array(strlen($number), $type['number_lengths'])) {
      return FALSE;
    }
    if ($type['uses_luhn'] && !self::validateLuhn($number)) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Validates the given credit card number using the Luhn algorithm.
   *
   * @param string $number
   *   The credit card number.
   *
   * @return bool
   *   TRUE if the credit card number is valid, FALSE otherwise.
   */
  public static function validateLuhn($number) {
    $total = 0;
    foreach (array_reverse(str_split($number)) as $i => $digit) {
      $digit = $i % 2 ? $digit * 2 : $digit;
      $digit = $digit > 9 ? $digit - 9 : $digit;
      $total += $digit;
    }
    return ($total % 10 === 0);
  }

  /**
   * Validates the given credit card expiration date.
   *
   * @param string $month
   *   The 1 or 2-digit numeric representation of the month, i.e. 1, 6, 12.
   * @param string $year
   *   The 4-digit numeric representation of the year, i.e. 2010.
   *
   * @return bool
   *   TRUE if the credit card expiration date is valid, FALSE otherwise.
   */
  public static function validateExpirationDate($month, $year) {
    if ($month < 1 || $month > 12) {
      return FALSE;
    }
    if ($year < date('Y')) {
      return FALSE;
    }
    elseif ($year == date('Y') && $month < date('n')) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Calculates the unix timestamp for a credit card expiration date.
   *
   * @param string $month
   *   The 1 or 2-digit numeric representation of the month, i.e. 1, 6, 12.
   * @param string $year
   *   The 4-digit numeric representation of the year, i.e. 2010.
   *
   * @return int
   *   The expiration date as a unix timestamp.
   */
  public static function calculateExpirationTimestamp($month, $year) {
    // Credit cards expire on the last day of the month.
    $month_start = strtotime($year . '-' . $month . '-01');
    $last_day = date('t', $month_start);
    return strtotime($year . '-' . $month . '-' . $last_day);
  }

  /**
   * Validates the given credit card security code.
   *
   * @param string $security_code
   *   The credit card security code.
   * @param array $type
   *   The credit card type.
   *
   * @return bool
   *   TRUE if the credit card security code is valid, FALSE otherwise.
   */
  public static function validateSecurityCode($security_code, array $type) {
    if (!is_numeric($security_code)) {
      return FALSE;
    }
    if (strlen($security_code) != $type['security_code_length']) {
      return FALSE;
    }

    return TRUE;
  }

}
