<?php
/**
 * Argument handler to accept a subscription send method.
 */
class notifications_views_handler_argument_subscription_send_method extends views_handler_argument {
  function construct() {
    parent::construct('type');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly version
   * of the node type.
   */
  function summary_name($data) {
    return $this->notifications_subscription_send_method($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly version of the
   * node type.
   */
  function title() {
    return $this->notifications_subscription_send_method($this->argument);
  }

  function notifications_subscription_send_method($method) {
    $methods = messaging_method_list();
    $title = isset($methods[$method]) ? $methods[$method] : t('Unknown send method');
    return check_plain($title);
  }
}
