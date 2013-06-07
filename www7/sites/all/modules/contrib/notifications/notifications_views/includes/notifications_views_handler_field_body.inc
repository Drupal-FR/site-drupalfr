<?php
// @TODO: Review this handler.

/**
 * Field handler
 */
class notifications_views_handler_field_body extends views_handler_field {
  function render($values) {
    $method_info = messaging_method_info('simple');
    $value = $values->{$this->field_alias};
    
    // The filter is applied now only once
    if ($method_info['filter']) {
      return check_markup($value, $filter, FALSE);
    }

    return check_plain($value);
  }
}

