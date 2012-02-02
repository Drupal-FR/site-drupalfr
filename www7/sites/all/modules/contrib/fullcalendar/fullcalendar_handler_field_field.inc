<?php
// $Id: fullcalendar_handler_field_field.inc,v 1.1.2.1 2011/02/09 16:10:06 timplunkett Exp $

/**
 * @file
 * A wrapper around date fields provided by Field API to retrieve raw values.
 */

class fullcalendar_handler_field_field extends views_handler_field_field {
  function fullcalendar_render($values) {
    $output = array(
      'value' => $this->get_items($values),
      'field_name' => $this->definition['field_name'],
    );
    return $output;
  }
}
