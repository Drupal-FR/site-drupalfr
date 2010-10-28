<?php
// $Id: simplenews_handler_field_newsletter_s_status.inc,v 1.1.2.4 2009/01/02 11:59:34 sutharsan Exp $

/**
 * @file
 * Views handler for simplenews sent status.
 */

/**
 * Display newsletter sent status.
 */
class simplenews_handler_field_newsletter_s_status extends views_handler_field {
  function render($values) {
    switch ($values->{$this->field_alias}) {
      case SIMPLENEWS_STATUS_SEND_NOT:
      default:
        return t('Not sent');
      case SIMPLENEWS_STATUS_SEND_PENDING:
        return t('Pending');
      case SIMPLENEWS_STATUS_SEND_READY:
        return t('Sent');
    }
  }
}
