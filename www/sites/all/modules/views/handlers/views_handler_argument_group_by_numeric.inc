<?php

/**
 * Simple handler for arguments using group by.
 */
class views_handler_argument_group_by_numeric extends views_handler_argument  {
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    $placeholder = empty($this->definition['numeric']) ? "'%s'" : '%d';
    $field = $this->get_field();

    $this->query->add_having(0, "$field = $placeholder", $this->argument);
  }

  function ui_name($short = FALSE) {
    return $this->get_field(parent::ui_name($short));
  }
}
