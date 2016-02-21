<?php

/**
 * @file
 * PDF display plugin.
 */

/**
 * This class contains all the functionality of the PDF display.
 */
class views_pdf_plugin_display extends views_plugin_display_page {

  /**
   * Define the display type
   */
  function get_style_type() { return 'pdf'; }

  /**
   * Disable the breadcrumb
   */
  function uses_breadcrumb() { return FALSE; }

  function has_path() { return TRUE; }


  /**
   * Render the display
   */
  function render() {
    // Generall document layout

    // Set default code
    $this->view->pdf->SetFont('');

    // Add leading pages
    $path = $this->view->pdf->getTemplatePath($this->get_option('leading_template'));
    $this->view->pdf->addPdfDocument($path);

    // Set the default background template
    $path = $this->view->pdf->getTemplatePath($this->get_option('template'));
    $this->view->pdf->setDefaultPageTemplate($path, 'main');

    // Render the items
    $this->view->style_plugin->render();

    // Add succeed pages
    $path = $this->view->pdf->getTemplatePath($this->get_option('succeed_template'));
    $this->view->pdf->addPdfDocument($path);

  }

  /**
   * Provide the preview. Because PDF cannot be embedded good in HTML, we do not
   * provide a preview.
   */
  function preview() {
    return t('The PDF display does not provide a preview.');
  }

  /**
   * This function executes the PDF display.
   */
  function execute($path_to_store_pdf = '', $destination = 'I') {

    // Defines external configuration for TCPDF library
    if (!defined('K_TCPDF_EXTERNAL_CONFIG')) {
      $tcpdf_path = drupal_realpath(views_pdf_get_library('tcpdf'));
      $cache_path = 'public://views_pdf_cache/';
      file_prepare_directory($cache_path, FILE_CREATE_DIRECTORY);
      global $base_url;
      define('K_TCPDF_EXTERNAL_CONFIG', TRUE);
      define('K_PATH_MAIN', dirname($_SERVER['SCRIPT_FILENAME']));
      define('K_PATH_URL', $base_url);
      define('K_PATH_FONTS', $tcpdf_path . '/fonts/');
      define('K_PATH_CACHE', drupal_realpath($cache_path));
      define('K_PATH_IMAGES', '');
      define('K_BLANK_IMAGE', $tcpdf_path . '/images/_blank.png');
      define('K_CELL_HEIGHT_RATIO', 1.25);
      define('K_SMALL_RATIO', 2/3);
    }

    if ($this->get_option('default_page_format') == 'custom') {
      if (preg_match('~([0-9\.]+)x([0-9\.]+)~', $this->get_option('default_page_format_custom'), $result)) {
        $format[0] = $result[1]; // width
        $format[1] = $result[2]; // height
      }
      else {
        $format = 'A4';
      }

    }
    else {
      $format = $this->get_option('default_page_format');
    }

    $orientation = $this->get_option('default_page_orientation'); // P or L
    $unit = $this->get_option('unit');
    $this->view->pdf = views_pdf_get_new_pdf_instance($orientation, $unit, $format);

    // Set margins: top, left, right
    $this->view->pdf->SetMargins($this->get_option('margin_left'), $this->get_option('margin_top'), $this->get_option('margin_right'), TRUE);

    // Set auto page break: margin bottom:
    $this->view->pdf->SetAutoPageBreak(TRUE, $this->get_option('margin_bottom'));

    $this->view->pdf->setDefaultFontSize($this->get_option('default_font_size'));
    $this->view->pdf->setDefaultFontFamily($this->get_option('default_font_family'));
    $this->view->pdf->setDefaultFontStyle($this->get_option('default_font_style'));
    $this->view->pdf->setDefaultTextAlign($this->get_option('default_text_align'));
    $this->view->pdf->setDefaultFontColor($this->get_option('default_font_color'));

    $this->view->pdf->setViewsHeader($this->view->display_handler->render_header());
    $this->view->pdf->setViewsFooter($this->view->display_handler->render_footer());

    $html = $this->view->render($this->display->id);

    if (!empty($html)) {
      echo $html;
    }

    if (empty($path_to_store_pdf)) {
      $path_to_store_pdf = $this->view->get_title();
      $this->view->pdf->SetTitle($this->view->get_title());
    }

    if (!preg_match('/\.pdf$/', $path_to_store_pdf)) {
      $path_to_store_pdf .= '.pdf';
    }

    ob_clean();
    if ($destination == 'I') {
      echo $this->view->pdf->Output($path_to_store_pdf, $destination);
      exit();
    }
    else {
      return $this->view->pdf->Output($path_to_store_pdf, $destination);
    }
  }

  /**
   * This method defines the default sections and the appropriated values.
   *
   */
  function defaultable_sections($section = NULL) {
    if (in_array($section, array('style_options', 'style_plugin', 'row_options', 'row_plugin'))) {
      return FALSE;
    }

    $sections = parent::defaultable_sections($section);

    // Tell views our sitename_title option belongs in the title section.
    if ($section == 'title') {
      $sections[] = 'sitename_title';
    }
    elseif (!$section) {
      $sections['title'][] = 'sitename_title';
    }
    return $sections;
  }

  /**
   * The option definition.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'pdf_unformatted';
    $options['style_options']['default']  = array('mission_description' => FALSE, 'description' => '');
    $options['sitename_title']['default'] = FALSE;
    $options['row_plugin']['default'] = 'pdf_fields';
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;

    // New Options
    $options['default_page_format'] = array('default' => 'A4');
    $options['default_page_format_custom'] = array('default' => '');
    $options['default_page_orientation'] = array('default' => 'P');
    $options['unit'] = array('default' => 'mm');
    $options['margin_left'] = array('default' => '15');
    $options['margin_right'] = array('default' => '15');
    $options['margin_top'] = array('default' => '15');
    $options['margin_bottom'] = array('default' => '15');

    $options['leading_template'] = array('default' => '');
    $options['template'] = array('default' => '');
    $options['succeed_template'] = array('default' => '');

    $options['default_font_family'] = array('default' => 'helvetica');
    $options['default_font_style'] = array('default' => array());
    $options['default_font_size'] = array('default' => '11');
    $options['default_text_align'] = array('default' => 'L');
    $options['default_font_color'] = array('default' => '000000');
    $options['default_text_hyphenate'] = array('default' => 'none');

    $options['css_file'] = array('default' => '');

    return $options;
  }

  /**
   * Option form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'pdf_page':
        $form['#title'] .= t('PDF Page Options');
        $form['default_page_format'] = array(
          '#type' => 'select',
          '#title' => t('Default Page Format'),
          '#required' => TRUE,
          '#options' => views_pdf_get_page_formats(),
          '#description' => t('This is the default page format. If you specifiy a different format in the template section, this settings will be override.'),
          '#default_value' => $this->get_option('default_page_format'),
        );
        $form['default_page_format_custom'] = array(
          '#type' => 'textfield',
          '#title' => t('Custom Page Format'),
          '#description' => t('Here you can specifiy a custom page format. The schema is "[width]x[height]".'),
          '#default_value' => $this->get_option('default_page_format_custom'),
        );
        $form['default_page_orientation'] = array(
          '#type' => 'radios',
          '#title' => t('Default Page Orientation'),
          '#required' => TRUE,
          '#options' => array('P' => t('Portrait'), 'L' => t('Landscape')),
          '#description' => t('This is the default page orientation.'),
          '#default_value' => $this->get_option('default_page_orientation'),
        );
        $form['unit'] = array(
          '#type' => 'select',
          '#title' => t('Unit'),
          '#required' => TRUE,
          '#options' => array('mm' => t('mm: Millimeter'), 'pt' => t('pt: Point'), 'cm' => t('cm: Centimeter'), 'in' => t('in: Inch')),
          '#description' => t('This is the unit for the entered unit data. If you change this option all defined units were changed, but not converted.'),
          '#default_value' => $this->get_option('unit'),
        );
        $form['margin_left'] = array(
          '#type' => 'textfield',
          '#title' => t('Margin: Left'),
          '#required' => TRUE,
          '#default_value' => $this->get_option('margin_left'),
        );
        $form['margin_right'] = array(
          '#type' => 'textfield',
          '#title' => t('Margin: Right'),
          '#required' => TRUE,
          '#default_value' => $this->get_option('margin_right'),
        );
        $form['margin_top'] = array(
          '#type' => 'textfield',
          '#title' => t('Margin: Top'),
          '#required' => TRUE,
          '#default_value' => $this->get_option('margin_top'),
        );
        $form['margin_bottom'] = array(
          '#type' => 'textfield',
          '#title' => t('Margin: Bottom'),
          '#required' => TRUE,
          '#default_value' => $this->get_option('margin_bottom'),
        );
        break;
      case 'pdf_fonts':
        $fonts = views_pdf_get_font_list();
        $font_styles = array(
          'b' => t('Bold'),
          'i' => t('Italic'),
          'u' => t('Underline'),
          'd' => t('Line through'),
          'o' => t('Overline')
        );

        $align = array(
          'L' => t('Left'),
          'C' => t('Center'),
          'R' => t('Right'),
          'J' => t('Justify'),
        );

        $hyphenate = array(
          'none' => t('None'),
          'auto' => t('Detect automatically'),
        );
        $hyphenate = array_merge($hyphenate, views_pdf_get_hyphenations());

        $form['#title'] .= t('PDF Default Font Options');
        $form['description'] = array(
          '#prefix' => '<div class="description form-item">',
          '#suffix' => '</div>',
          '#value' => t('Here you specify a the default font settings for the document.'),
        );
        $form['default_font_size'] = array(
          '#type' => 'textfield',
          '#title' => t('Font Size'),
          '#size' => 10,
          '#default_value' => $this->get_option('default_font_size'),
        );
        $form['default_font_family'] = array(
          '#type' => 'select',
          '#title' => t('Font Family'),
          '#options' => $fonts,
          '#size' => 5,
          '#default_value' => $this->get_option('default_font_family'),
        );
        $form['default_font_style'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Font Style'),
          '#options' => $font_styles,
          '#default_value' => $this->get_option('default_font_style'),
        );
        $form['default_text_align'] = array(
          '#type' => 'radios',
          '#title' => t('Text Alignment'),
          '#options' => $align,
          '#default_value' => $this->get_option('default_text_align'),
        );
        $form['default_text_hyphenate'] = array(
          '#type' => 'select',
          '#title' => t('Text Hyphenation'),
          '#options' => $hyphenate,
          '#description' => t('If you want to use hyphenation, then you need to download from <a href="@url">ctan.org</a> your needed pattern set. Then upload it to the dir "hyphenate_patterns" in the TCPDF lib directory. Perhaps you need to create the dir first. If you select the automated detection, then we try to get the language of the current node and select an appropriate hyphenation pattern.', array('@url' => 'http://www.ctan.org/tex-archive/language/hyph-utf8/tex/generic/hyph-utf8/patterns/tex')),
          '#default_value' => $this->get_option('default_text_hyphenate'),
        );

        $form['default_font_color'] = array(
          '#type' => 'textfield',
          '#title' => t('Text Color'),
          '#description' => t('If a value is entered without a comma, it will be interpreted as a hexadecimal RGB color. Normal RGB can be used by separating the components by a comma. e.g 255,255,255 for white. A CMYK color can be entered in the same way as RGB. e.g. 0,100,0,0 for magenta.'),
          '#size' => 20,
          '#default_value' => $this->get_option('default_font_color'),
        );

        break;

      case 'pdf_template':
        $form['#title'] .= t('PDF Templates');

        $templates = array_merge(array(t('-- None --')), views_pdf_get_pdf_templates());
        $form['leading_template'] = array(
          '#type' => 'select',
          '#options' => $templates,
          '#title' => t('Leading PDF Template'),
          '#required' => FALSE,
          '#description' => t('Here you specify a PDF file to be printed in front of every row.'),
          '#default_value' => $this->get_option('leading_template'),
        );

        $form['template'] = array(
          '#type' => 'select',
          '#options' => $templates,
          '#title' => t('Template PDF'),
          '#description' => t('Here you specify a PDF file on which the content is printed. The first page of this document is used for the first page, in the target document. The second page is used for the second page in the target document and so on. If the target document has more that this template file, the last page of the template will be repeated. The leading document has no effect on the order of the pages.'),
          '#default_value' => $this->get_option('template'),
        );

        $form['succeed_template'] = array(
          '#type' => 'select',
          '#options' => $templates,
          '#title' => t('Succeed PDF Template'),
          '#required' => FALSE,
          '#description' => t('Here you specify a PDF file to be printed after the main content.'),
          '#default_value' => $this->get_option('succeed_template'),
        );

        $form['template_file'] = array(
          '#type' => 'file',
          '#title' => t('Upload New Template File'),
        );

        $form['#attached']['js'][] = array(
          'type' => 'setting',
          'data' => array(
            'urlIsAjaxTrusted' => array(
              $GLOBALS['base_url'] . '/' . current_path() => TRUE,
            ),
          ),
        );

        break;

      case 'displays':
        $form['#title'] .= t('Attach to');
        $displays = array();
        foreach ($this->view->display as $display_id => $display) {
          if (!empty($display->handler) && $display->handler->accept_attachments()) {
            $displays[$display_id] = $display->display_title;
          }
        }
        $form['displays'] = array(
          '#type' => 'checkboxes',
          '#description' => t('The feed icon will be available only to the selected displays.'),
          '#options' => $displays,
          '#default_value' => $this->get_option('displays'),
        );
        break;

      case 'css':
        $form['#title'] .= t('CSS File');
        $form['css_file'] = array(
          '#type' => 'textfield',
          '#description' => t('URL to a CSS file. This file is attached to all fields, rendered as HTML.'),
          '#default_value' => $this->get_option('css_file'),
        );
        break;
    }
  }

  /**
   * Provide a summary of the options.
   */
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);

    $fonts = views_pdf_get_font_list();

    // Change Page title:
    $categories['page'] = array(
      'title' => t('PDF settings'),
      'column' => 'second',
      'build' => array(
        '#weight' => -10,
      ),
    );

    // Add for attach the display to others:
    $displays = array_filter($this->get_option('displays'));
    if (count($displays) > 1) {
      $attach_to = t('Multiple displays');
    }
    elseif (count($displays) == 1) {
      $display = array_shift($displays);
      if (!empty($this->view->display[$display])) {
        $attach_to = check_plain($this->view->display[$display]->display_title);
      }
    }

    if (!isset($attach_to)) {
      $attach_to = t('None');
    }

    $options['displays'] = array(
      'category' => 'page',
      'title' => t('Attach to'),
      'value' => $attach_to,
    );

    // Add for pdf page settings
    $options['pdf_page'] = array(
      'category' => 'page',
      'title' => t('PDF Page Settings'),
      'value' => $this->get_option('default_page_format'),
      'desc' => t('Define some PDF specific settings.'),
    );

    // Add for pdf font settings
    $options['pdf_fonts'] = array(
      'category' => 'page',
      'title' => t('PDF Fonts Settings'),
      'value' => t('!family at !size pt', array('!family' => $fonts[$this->get_option('default_font_family')], '!size' => $this->get_option('default_font_size'))),
      'desc' => t('Define some PDF specific settings.'),
    );

    // add for pdf template settings
    if ($this->get_option('leading_template') != '' || $this->get_option('template') != '' || $this->get_option('succeed_template') != '') {
      $isAnyTemplate = t('Yes');
    }
    else {
      $isAnyTemplate = t('No');
    }

    $options['pdf_template'] = array(
      'category' => 'page',
      'title' => t('PDF Template Settings'),
      'value' => $isAnyTemplate,
      'desc' => t('Define some PDF specific settings.'),
    );

    if ($this->get_option('css_file') == '') {
      $css_file = t('None');
    }
    else {
      $css_file = $this->get_option('css_file');
    }

    $options['css'] = array(
      'category' => 'page',
      'title' => t('CSS File'),
      'value' => $css_file,
      'desc' => t('Define a CSS file attached to all HTML output.'),
    );
  }

  /**
   * Handles the storage of the options.
   *
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'pdf_page':
        $this->set_option('default_page_format', $form_state['values']['default_page_format']);
        $this->set_option('default_page_format_custom', $form_state['values']['default_page_format_custom']);
        $this->set_option('default_page_orientation', $form_state['values']['default_page_orientation']);
        $this->set_option('unit', $form_state['values']['unit']);
        $this->set_option('margin_left', $form_state['values']['margin_left']);
        $this->set_option('margin_right', $form_state['values']['margin_right']);
        $this->set_option('margin_top', $form_state['values']['margin_top']);
        $this->set_option('margin_bottom', $form_state['values']['margin_bottom']);

        break;

      case 'pdf_fonts':
        $this->set_option('default_font_size', $form_state['values']['default_font_size']);
        $this->set_option('default_font_style', $form_state['values']['default_font_style']);
        $this->set_option('default_font_family', $form_state['values']['default_font_family']);
        $this->set_option('default_text_align', $form_state['values']['default_text_align']);
        $this->set_option('default_font_color', $form_state['values']['default_font_color']);

        break;

      case 'pdf_template':
        // Save new file:
        // Note: The jQuery update is required to use Ajax for file upload. With
        // default Drupal jQuery it will not work.
        // For upload with Ajax a iFrame is open and upload in it, because
        // normal forms are not allowed to handle directly.
        $destination = variable_get('views_pdf_template_stream', 'public://views_pdf_templates');

        if (file_prepare_directory($destination, FILE_CREATE_DIRECTORY)) {
          $file = file_save_upload('template_file', array(), $destination);
          if (is_object($file)) {
            $file_name =  basename($file->destination, '.pdf');
            $form_state['values']['template'] = $file_name;
            $file->status |= FILE_STATUS_PERMANENT;
            $file = file_save($file);
          }
        }

        $this->set_option('leading_template', $form_state['values']['leading_template']);
        $this->set_option('template', $form_state['values']['template']);
        $this->set_option('succeed_template', $form_state['values']['succeed_template']);
        break;

      case 'displays':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]);
        break;

      case 'css':
        $this->set_option('css_file', $form_state['values']['css_file']);
        break;
    }
  }

  /**
   * Attach to another view.
   */
  function attach_to($display_id) {
    $displays = $this->get_option('displays');
    if (empty($displays[$display_id])) {
      return;
    }

    // Defer to the feed style; it may put in meta information, and/or
    // attach a feed icon.
    $plugin = $this->get_plugin();
    if ($plugin) {
      $clone = $this->view->clone_view();
      $clone->set_display($this->display->id);
      $clone->build_title();
      $plugin->attach_to($display_id, $this->get_path(), $clone->get_title());
    }
  }


}
