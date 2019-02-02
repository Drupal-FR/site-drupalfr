<?php

/**
 * @file
 * Hide the locality field.
 */

$plugin = array(
  'title' => t('Hide the locality'),
  'format callback' => 'addressfield_format_address_hide_locality',
  'type' => 'address',
  'weight' => -84,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_address_hide_locality(&$format, $address, $context = array()) {
  if (isset($format['locality_block']['locality'])) {
    $format['locality_block']['locality']['#access'] = FALSE;
  }
}
