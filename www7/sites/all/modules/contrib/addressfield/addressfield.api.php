<?php

/**
 * @file
 * API documentation for Addressfield.
 */

/**
 * Format generation callback.
 *
 * @param $format
 *   The address format being generated.
 * @param $address
 *   The address this format is generated for.
 * @param $context
 *   An associative array of context information pertaining to how the address
 *   format should be generated. If no mode is given, it will initialize to the
 *   default value. The remaining context keys should only be present when the
 *   address format is being generated for a field:
 *   - mode: either 'form' or 'render'; defaults to 'render'.
 *   - field: the field info array.
 *   - instance: the field instance array.
 *   - langcode: the langcode of the language the field is being rendered in.
 *   - delta: the delta value of the given address.
 *
 * @ingroup addressfield_format
 */
function CALLBACK_addressfield_format_callback(&$format, $address, $context = array()) {
  // No example.
}

/**
 * Allows modules to alter the predefined address formats.
 *
 * @param $address_formats
 *   The array of all predefined address formats.
 *
 * @see addressfield_get_address_format()
 */
function hook_addressfield_address_formats_alter(&$address_formats) {
  // Remove the postal_code from the list of required fields for China.
  $address_formats['CN']['required_fields'] = array('locality', 'administrative_area');
}

/**
 * Allows modules to alter the predefined administrative areas.
 *
 * @param $administrative_areas
 *   The array of all predefined administrative areas.
 *
 * @see addressfield_get_administrative_areas()
 */
function hook_addressfield_administrative_areas_alter(&$administrative_areas) {
  // Alter the label of the Spanish administrative area with the iso code PM.
  $administrative_areas['ES']['PM'] = t('Balears / Baleares');

  // Add administrative areas for imaginary country XT, keyed by their
  // imaginary ISO codes.
  $administrative_areas['XT'] = array(
      'A' => t('Aland'),
      'B' => t('Bland'),
  );
}

/**
 * Allows modules to add arbitrary AJAX commands to the array returned from the
 * standard address field widget refresh.
 *
 * @param &$commands
 *   The array of AJAX commands used to refresh the address field widget.
 * @param $form
 *   The rebuilt form array.
 * @param $form_state
 *   The form state array from the form.
 *
 * @see addressfield_standard_widget_refresh()
 */
function hook_addressfield_standard_widget_refresh_alter(&$commands, $form, $form_state) {
  // Display an alert message.
  $commands[] = ajax_command_alert(t('The address field widget has been updated.'));
}
