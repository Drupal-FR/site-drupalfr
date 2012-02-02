<?php
// $Id: fullcalendar_handler_field_date.inc,v 1.1.2.2 2011/02/13 04:12:36 timplunkett Exp $

/**
 * @file
 * A wrapper around timestamp provided by Drupal to retrieve raw values.
 */

class fullcalendar_handler_field_date extends views_handler_field_date {
  function fullcalendar_render($values) {
    return array(
      'value' => array(array(
        'rendered' => array('#markup' => $this->render($values)),
        'raw' => array(
          'value' => $values->{$this->field_alias},
        ),
      )),
      'field_name' => $this->field_alias,
    );
  }

  function query() {
    parent::query();
    $this->query->add_field($this->view->base_table, $this->view->base_field);
  }
}
