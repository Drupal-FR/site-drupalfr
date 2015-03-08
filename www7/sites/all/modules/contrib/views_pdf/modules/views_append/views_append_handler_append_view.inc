<?php
/**
 * @file
 * Views Append Handler
 *
 * To append a view to another a field handler is used.
 */

/**
 * This class contains all the functionality to append a view to another one.
 *
 */
class views_append_handler_append_view extends views_handler_field {

  /**
   * This method  is used to query data. In our case
   * we want that no data is queried.
   *
   */
  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'view_append_' . $this->position;
  }

  /**
   * This method contains the defintion of the options for appending a view.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['url'] = array('default' => '');

    return $options;
  }

  /**
   * Option form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter the URL to the file'),
      '#default_value' => $this->options['url'],
      '#description' => t('Enter the URL to the file. You can use tokens to replace some parts of the URL.'),
    );
  }

  /**
   * This method renders the other view.
   */
  function render($values) {

    if ($this->options['exclude'] == '1') {
      return '';
    }

    $tokens = $this->get_render_tokens('');

    $url = str_replace(array_keys($tokens), $tokens, $this->options['url']);
    $data = file_get_contents($url);

    $tmp_file = md5($url . time());

    $files_path = file_directory_temp();
    $dir = $files_path . '/views_append_tmp_files';

    if (!is_dir($dir)) {
      @mkdir($dir);
    }

    if (is_writable($dir)) {
      $path = $dir . '/' . $tmp_file;
      views_append_request_with_cookie($url, $path);

      if (isset($this->view->pdf) && is_object($this->view->pdf)) {
        $this->view->pdf->addPdfDocument($path);
      }
      else {
        return file_get_contents($path);
      }
    }
  }

  /**
   * We dont want to use advanced rendering.
   */
  function allow_advanced_render() {
    return FALSE;
  }
}
