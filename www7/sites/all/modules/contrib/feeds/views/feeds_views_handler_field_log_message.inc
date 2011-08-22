<?php
// $Id: feeds_views_handler_field_log_message.inc,v 1.1.2.1 2010/10/26 03:05:58 alexb Exp $

/**
 * @file
 * Views handler for displaying a log message.
 */

class feeds_views_handler_field_log_message extends views_handler_field {

  /**
   * Override parent::render().
   */
  function render($values) {
    $message = $values->{$this->field_alias};
    $variables = unserialize($values->{$this->aliases['variables']});
    return t($message, $variables);
  }

  /**
   * Disallow advanced rendering.
   */
  function allow_advanced_render() {
    return FALSE;
  }
}
