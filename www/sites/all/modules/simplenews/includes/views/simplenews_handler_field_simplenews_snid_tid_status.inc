<?php

/**
 * @file
 * Views field handler for simplenews subscription status.
 */

/**
 * Display subscription status (subscribed/unsubscribed).
 */
class simplenews_handler_field_simplenews_snid_tid_status extends views_handler_field {
  function render($values) {

    switch ($values->{$this->field_alias}) {
      case 0:
        return t('unsubscribed');
      case 1:
        return t('subscribed');     
    }
  }
}
