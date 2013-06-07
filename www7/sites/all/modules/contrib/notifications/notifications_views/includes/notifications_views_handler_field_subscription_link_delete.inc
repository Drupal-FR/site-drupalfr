<?php
/**
 * Field handler to present a link subscription delete.
 */
class notifications_views_handler_field_subscription_link_delete extends notifications_views_handler_field_subscription_link {
  function render_link($data, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "notifications/subscription/" . $this->get_value($values, 'sid') . "/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
