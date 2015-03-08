<?php
/**
 * @file
 * The page number plugin for PDF page display.
 *
 * This plugin is used to add the page number to a PDF display.
 *
 */


/**
 * Plugin class that holds the functionality for the
 * page number in a PDF display.
 *
 */
class views_pdf_handler_page_number extends views_handler_field {

  /**
   * This method  is used to query data. In our case
   * we want that no data is queried.
   *
   */
  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'pdf_page_number_' . $this->position;
  }

  /**
   * This method adds a page number to the display, if it is a PDF display.
   * Therefore the PDF class is used.
   */
  function render($values) {

    if (isset($this->view->pdf) && is_object($this->view->pdf)) {
      return $this->view->pdf->getPage();
    }
    else {
      return '';
    }

  }

  /**
   * We dont want to use advanced rendering.
   */
  function allow_advanced_render() {
    return FALSE;
  }
}
