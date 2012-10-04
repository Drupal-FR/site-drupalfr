<?php

/**
 * A handler to run a field through check_markup, using a companion
 * format field.
 *
 * - format: (REQUIRED) The field in this table used to control the format
 *           such as the 'format' field in the node, which goes with the
 *           'body' field.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_markup extends views_handler_field {
  /**
   * Constructor; calls to base object constructor.
   */
  function construct() {
    parent::construct();

    $this->format = $this->definition['format'];

    $this->additional_fields = array();
    if (!is_numeric($this->format)) {
      $this->additional_fields['format'] = array('field' => $this->format);
    }
  }

  function render($values) {
    $value = $this->get_value($values);
    if (!is_numeric($this->format)) {
      $format = $this->get_value($values, 'format');
    }
    else {
      $format = $this->format;
    }
    if ($value) {
      $value = str_replace('<!--break-->', '', $value);
      return check_markup($value, $format, FALSE);
    }
  }

  function element_type($none_supported = FALSE, $default_empty = FALSE, $inline = FALSE) {
    if ($inline) {
      return 'span';
    }

    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }

    return 'div';
  }
}
