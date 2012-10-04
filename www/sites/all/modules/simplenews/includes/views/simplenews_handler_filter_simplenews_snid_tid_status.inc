<?php 

/**
 * @file
 * Views filter handler for simplenews subscription status
 */

/**
 * Display subscription status (subscribed/unsubscribed).
 */
class simplenews_handler_filter_simplenews_snid_tid_status extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = array(
      0 => t('unsubscribed'),
      1 => t('subscribed')
    );
  }
}
