<?php

/**
 * Filter by system type.
 */
class views_handler_filter_system_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Type');
      // Enable filtering by type.
      $types = array();
      $result = db_query('SELECT DISTINCT(type) FROM {system} ORDER BY type');
      while ($row = db_fetch_array($result)) {
        if (!empty($row['type'])) {
          $types[$row['type']] = $row['type'];
        }
      }
      $this->value_options = $types;
    }
  }
}
