<?php
// $Id: userpoints_views_handler_filter_category.inc,v 1.1.2.1 2010/12/10 09:58:03 berdir Exp $
/**
 * Filter by category
 */
class userpoints_views_handler_filter_category extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Category');
      $this->value_options = userpoints_get_categories();
    }
  }
}