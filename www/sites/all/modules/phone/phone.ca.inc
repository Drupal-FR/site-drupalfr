<?php
// $Id: phone.ca.inc,v 1.31 2010/01/02 14:10:50 thierrygd Exp $

/**
 * @file
 * CCK Field for Canadian phone numbers.
 */
 
function phone_ca_metadata() {
  return array(
    'label' => 'US & Canadian Phone Numbers',
    'error' => '"%value" is not a valid North American phone number<br>North American Phone numbers should only contain numbers and + and - and ( and ) and spaces and be like 999-999-9999. Please enter a valid ten-digit phone number with optional extension.',
  );
}
 
/**  
 * Verifies that $phonenumber is a valid ten-digit North American phone number
 *
 * @param string $phonenumber
 * @return boolean Returns boolean FALSE if the phone number is not valid. 
 */

function valid_ca_phone_number($phonenumber) {

  //$phonenumber = trim($phonenumber);

  // define regular expression
  $regex = '/
    \D*           # ignore non-digits
    1?            # an optional 1
    \D*           # optional separator 
    [02-9]\d{2}   # area code (cannot start with 1)
    \D*           # optional separator
    [02-9]\d{2}   # 3-digit prefix (cannot start with 1)    
    \D*           # optional separator
    \d{4}         # 4-digit line number
    \D*           # optional separator
    \d*           # optional extension
    \D*           # ignore trailing non-digits
    /x';
  // return true if valid, false otherwise
  return (bool) preg_match($regex, $phonenumber); 
} 

/**
 * Convert a valid North American phone number into standard (444) 867-5309 x1234 format
 * 
 * @param $phonenumber must be a valid ten-digit number (with optional extension)
 * 
 */
function format_ca_phone_number($phonenumber, $field) {

  // define regular expression   
  $regex = '/
    ^\D*          # ignore non-digits
    1?            # an optional 1
    \D*           # optional separator 
    ([02-9]\d{2}) # capture area code       
    \D*           # optional separator
    (\d{3})       # capture 3-digit prefix
    \D*           # optional separator
    (\d{4})       # capture 4-digit line number                       
    \D*           # optional separator
    (\d*)         # capture optional extension                       
    \D*$          # ignore trailing non-digits
  /x'; 
    
  // get digits of phone number
  preg_match($regex, $phonenumber, $matches);
  
  $separator = isset($field['ca_phone_separator']) ? $field['ca_phone_separator'] : '-';
  
  // construct ten-digit phone number
  $phonenumber =
    ( $field['ca_phone_parentheses'] ?
      '(' . $matches[1] . ') ' :
      $matches[1] . $separator ) .
      $matches[2] . $separator . $matches[3];  

 // Optional extension
  if ($matches[4] != '') {
      $phonenumber .= ' x' . $matches[4];
  }
  
  if ($field['phone_country_code']) {
    if ($matches[1] != '1') {
  	$phonenumber = '1' . ' ' . $phonenumber; 
    }
  }

  return $phonenumber;
}
