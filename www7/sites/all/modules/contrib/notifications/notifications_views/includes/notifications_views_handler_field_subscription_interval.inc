<?php

/**
 * Field handler to translate a node type into its readable form.
 */
class notifications_views_handler_field_subscription_interval extends notifications_views_handler_field_machine_name {
  /**
    * Render field as human readable name, unless using machine_name option.
    */
  function render_name($data, $values) {;
    if ($this->options['machine_name'] != 1 && $data !== NULL && $data !== '') {
      $intervals = notifications_send_intervals();
      $name = isset($intervals[$data]) ? $intervals[$data] : t('Unknown');
      return t($this->sanitize_value($name));
    }
    return $this->sanitize_value($data);
  }
}

