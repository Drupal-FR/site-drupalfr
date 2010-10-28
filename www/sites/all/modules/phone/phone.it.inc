<?php
// $Id: phone.it.inc,v 1.24 2010/01/02 14:10:50 thierrygd Exp $

/**
 * @file
 * CCK Field for Italian phone numbers.
 */
 
function phone_it_metadata() {
  // These strings are translated using t() on output.
  return array(
    'label' => 'Italian Phone Numbers',
    'error' => '"%value" is not a valid Italian phone number<br>Italian phone numbers should only ...',
  );
}

/**  
 * Verifies that $phonenumber is a valid ten-digit North American phone number
 *
 * @param string $phonenumber
 * @return boolean Returns boolean FALSE if the phone number is not valid. 
 */
function valid_it_phone_number($phonenumber) {
  // define regular expression
  $regex = "/^(0([1345789][0159]|(37|54|73|96)[1234567]|(12|17|34|38|46|52|57|78|8[678]|9[2378])[12345]|(14|32|47)[1234]|(18|36|53|57|77|82)[345]|(33|72|83|94)[12]|1(31|6[1356]|8[27])|2|3(46|62|86)|4([23][123456789]|4[245]|81)|5([368]6|32|6[45]|[78][78]|8[345])|6|7(4[2346]|6[1356]|7[16]|89)|8(2[78]|3[356])|968|976)\/[1-9]|3([234][03789]|3[456]|46|6[0368]|8[0389]|9[0123])\/)[0-9 -]+[0-9]$/i";

  // return true if valid, false otherwise
  return (bool) preg_match($regex, $phonenumber);   
}  

/**
 * Formatting for Italian Phone Numbers.  
 *
 * @param string $phonenumber
 * @return string Returns a string containting the phone number with some formatting.
 */
function format_it_phone_number($phonenumber, $field) {

  //$phonenumber = trim($phonenumber);

   // do some formatting on the phone number 
   $phonenumber = str_replace("[ -]", "", $phonenumber);
   
   if ($field['phone_country_code']) {
      if ($matches[1] != "+39") {
  	$phonenumber = "+39" . " " . $phonenumber; 
      }
   }   
  
   return $phonenumber;
}
