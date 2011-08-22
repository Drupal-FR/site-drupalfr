<?php
// $Id: feeds_views_handler_filter_severity.inc,v 1.1.2.1 2010/10/26 03:05:58 alexb Exp $

/**
 * @file
 * Filter by severity.
 */

class feeds_views_handler_filter_severity extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_title = t('Severity');
    $this->value_options = watchdog_severity_levels();
  }
}
