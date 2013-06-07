<?php
/**
 * Argument handler to accept a subscription type.
 */
class notifications_views_handler_argument_subscription_type extends views_handler_argument {
  function construct() {
    parent::construct('type');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly version
   * of the node type.
   */
  function summary_name($data) {
    return $this->notifications_subscription_type($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly version of the
   * node type.
   */
  function title() {
    return $this->notifications_subscription_type($this->argument);
  }

  function notifications_subscription_type($type) {
    $output = notifications_subscription_type($type);
    $title = isset($output['title']) ? $output['title'] : t('Unknown subscription type');
    return check_plain($title);
  }
}
