<?php
// $Id: fullcalendar_handler_empty.inc,v 1.1.2.3 2011/02/10 14:00:21 timplunkett Exp $

/**
 * @file
 * Provide an empty region to display a blank FullCalendar.
 */

class fullcalendar_handler_empty extends views_handler {
  function render() {
    fullcalendar_get_settings($this->view->style_options);
    return '<div class="fullcalendar"></div>';
  }
}
