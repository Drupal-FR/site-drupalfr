<?php


/**
 * @file
 * Unformatted PDF style
 */


/**
 * This class holds all the funtionality used for the unformatted style plugin.
 */
class views_pdf_plugin_style_unformatted extends views_plugin_style {
  /**
   * Render the grouping sets.
   *
   * Plugins may override this method if they wish some other way of handling
   * grouping.
   *
   * @param $sets
   *   Array containing the grouping sets to render.
   * @param $level
   *   Integer indicating the hierarchical level of the grouping.
   *
   * @return string
   *   Rendered output of given grouping sets.
   */
  function render_grouping_sets($sets, $level = 0) {

    $output = '';

    $next_level = $level + 1;
    foreach ($sets as $set) {
      $row = reset($set['rows']);
      // Render as a grouping set.
      if (is_array($row) && isset($row['group'])) {
        $field_id = $this->options['grouping'][$level]['field'];
        $options = array();
        if(isset($this->row_plugin->options['formats'][$field_id])) {
          $options = $this->row_plugin->options['formats'][$field_id];
        }
        $this->view->pdf->drawContent($set['group'], $options, $this->view);
        $this->render_grouping_sets($set['rows'], $next_level);
      }
      // Render as a record set.
      else {
        if (!empty($set['group'])) {
          $field_id = $this->options['grouping'][$level]['field'];
          $options = array();
          if(isset($this->row_plugin->options['formats'][$field_id])) {
            $options = $this->row_plugin->options['formats'][$field_id];
          }
          $this->view->pdf->drawContent($set['group'], $options, $this->view);
        }

        if ($this->uses_row_plugin()) {
          foreach ($set['rows'] as $index => $row) {
            $this->view->row_index = $index;
            $set['rows'][$index] = $this->row_plugin->render($row);
          }
        }
      }
    }

    unset($this->view->row_index);
    return $output;
  }

  /**
   * Attach this view to another display as a feed.
   *
   * Provide basic functionality for all export style views like attaching a
   * feed image link.
   */
  function attach_to($display_id, $path, $title) {
    $display = $this->view->display[$display_id]->handler;
    $url_options = array();
    $input = $this->view->get_exposed_input();
    if ($input) {
      $url_options['query'] = $input;
    }

    if (empty($this->view->feed_icon)) {
      $this->view->feed_icon = '';
    }
    $this->view->feed_icon .= theme(
      'views_pdf_icon',
      array(
        'path' => $this->view->get_url(NULL, $path),
        'title' => $title,
        'options' => $url_options
      )
    );
  }

}
