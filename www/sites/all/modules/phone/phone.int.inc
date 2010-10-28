<?php
// $Id: phone.int.inc,v 1.6 2010/01/02 14:10:50 thierrygd Exp $

/**
 * Verifies that $phonenumber is a valid international phone number as per ITU or,
 * if a default country code is specified, a valid subscriber number.
 * 
 * @see http://www.itu.int/rec/T-REC-E.123/en
 * 
 * @param $phonenumber
 *   International phone number to validate
 * @return
 *   TRUE if valid, FALSE if otherwise.
 */
function valid_int_phone_number($phonenumber) {
  $phonenumber = trim($phonenumber);
  if ($phonenumber === '') {
    return FALSE;
  }
  $phonenumber = _normalize_country_code($phonenumber, $field);
  $base_phonenumber = str_replace(array('.', '(', ')', '[', ']', '-', '+', ' '), '', $phonenumber);
  if (!isset($field['phone_int_max_length'])) {
    $field['phone_int_max_length'] = 15;
  }
  if (strlen($base_phonenumber) > $field['phone_int_max_length']) {
    $error = t('Invalid international phone number: Phone number is too long; international phone numbers are limited to 15 digits.');
    return FALSE;
  }
  // Check if digits are used
  // Remove plus sign:
  $digits = substr($base_phonenumber, 1);
  if (!ctype_digit($digits)) {
    $error = t('Invalid international phone number: Phone number contains invalid characters; only allowed characters are numbers and punctuation.');
    return FALSE;
  }
  // Extract country code and see if it's correct:
  preg_match('/^\+(\d+)/', $phonenumber, $matches);
  $cc = $matches[1];
  if (strlen($cc) > 3) {
    $error = array(
      t('Invalid international phone number: Country code "+%cc" is too long; valid country codes are three digits or less.'),
      array('%cc' => $cc)
    );
    return FALSE;
  }
  // TODO: Check if parentheses/brackets add up.
  return TRUE;
}

/**
 * Formats $phonenumber into the standard representation of international
 * numbers as per E.123.
 * 
 * @param $phonenumber
 *   International phone number to format
 * @return
 *   Formatted international phone number
 */
function format_int_phone_number($phonenumber, $field = array()) {
  $phonenumber = trim($phonenumber);
  if ($phonenumber === '') {
    return '';
  }
  $phonenumber = _normalize_country_code($phonenumber, $field);
  $bits = preg_split('/[.()\[\]\- ]/', $phonenumber, -1, PREG_SPLIT_NO_EMPTY);
  // $bits[0] is the country code WITH a plus sign.
  if (isset($bits[1])) {
    // This is the first non-CC segment, so it could have the NDN.
    switch ($bits[1][0]) {
      case 0:
        $bits[1] = substr($bits[1], 1);
        break;
    }
    switch ($bits[1]) {
      case 0:
      case 7:
      case 8:
        array_splice($bits, 1, 1);
        break;
    }
  }
  return implode(' ', $bits);
}

/**
 * Adds a country code to a phone number if necessary.
 * 
 * @param $phonenumber
 *   International or local phone number to format
 * @return
 *   International phone number with country code
 */
function _normalize_country_code($phonenumber, $field = array()) {
  if ($phonenumber[0] !== '+') {
    $cc = isset($field['phone_default_country_code']) ? $field['phone_default_country_code'] : '1';
    return "+$cc $phonenumber";
  }
  return $phonenumber;
}
