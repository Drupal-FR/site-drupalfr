<?php
/**
 * @file
 * Plugin for the views include field
 */

/**
 * This class contains the functionality to add a view as a new field in
 * another view.
 *
 */
class views_view_field_handler_include_view extends views_handler_field {

  /**
   * Query the view. Deactivated because we do not want to query anything.
   */
  function query() {
    // Override parent::query() and don't alter query.
    $this->field_alias = 'view_include_' . $this->position;
  }

  /**
   * Definitions of the views field options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['view'] = array('default' => '');
    $options['number_of_args'] = array('default' => 1);
    $options['args'] = array('default' => array());

    return $options;
  }

  /**
   * Settings form
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $views = views_get_all_views();

    foreach ($views as $key => $view) {
      if ($this->view->name != $view->name) {
        $view_options[$key] = $view->name;
      }
    }

    $form['view'] = array(
      '#type' => 'select',
      '#title' => t('View to include'),
      '#options' => $view_options,
      '#default_value' => $this->options['view'],
      '#description' => t('Select the view to include. The display will be automatically determined.'),
    );

    $form['number_of_args'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter the number of arguments'),
      '#default_value' => $this->options['number_of_args'],
      '#description' => t('Enter the number of arguments you want to pass to the view.'),
    );

    for ($i = 0; $this->options['number_of_args'] > $i; $i++) {
      $form['args'][$i] = array(
        '#type' => 'textfield',
        '#title' => t('Argument #%number', array('%number' => ($i+1))),
        '#default_value' => $this->options['args'][$i],
        '#description' => t('Enter here the argument to pass to the view. If you want to use a value from a field, then use the replacement patterns in <strong>Rewrite results</strong> below.'),
      );
    }

    // Get a list of the available fields and arguments for token replacement.
    $options = array();
    foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {

      // We only use fields up to (and excluding) this one.
      if ($field == $this->options['id']) {
        break;
      }
      $options[t('Fields')]["[$field]"] = $handler->ui_name();
    }
    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }

    $this->document_self_tokens($options[t('Fields')]);

    // Default text.
    $output = t('<p>You must add some additional fields to this display before using this field. These fields may be marked as <em>Exclude from display</em> if you prefer. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
    // We have some options, so make a list.
    if (!empty($options)) {
      $output = t('<p>The following substitution patterns are available for this display. Use the pattern shown on the left to display the value indicated on the right. Note that due to rendering order, you cannot use fields that come after this field; if you need a field not listed here, rearrange your fields.</p>');
      foreach (array_keys($options) as $type) {
        if (!empty($options[$type])) {
          $items = array();
          foreach ($options[$type] as $key => $value) {
            $items[] = $key . ' == ' . $value;
          }
          $output .= theme('item_list', array('items' => $items, 'title' => $type));
        }
      }
    }

    // This construct uses 'hidden' and not markup because process doesn't
    // run. It also has an extra div because the dependency wants to hide
    // the parent in situations like this, so we need a second div to
    // make this work.
    $form['alter']['help'] = array(
      '#type' => 'hidden',
      '#id' => 'views-tokens-help',
      '#prefix' => '<div><fieldset id="views-tokens-help"><legend>' . t('Replacement patterns') . '</legend>' . $output . '</fieldset></div>',
      /* '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'edit-options-alter-make-link' => array(1),
        'edit-options-alter-alter-text' => array(1),
      ), */
    );

  }

  /**
   * Renders the field. For rendering the new views is created an added. For
   * PDF displays the two PDF classes where merged.
   */
  function render($values) {

    if (!empty($this->options['exclude'])) {
      return '';
    }

    $displayType = $this->view->display_handler->get_style_type();
    $currentDisplay = $this->view->current_display;

    $tokens = $this->get_render_tokens('');
    $args = array();
    foreach ($this->options['args'] as $arg) {
      $args[] = str_replace(array_keys($tokens), $tokens, $arg);
    }

    $view_name = $this->options['view'];

    $view = views_get_view($view_name);

    // look for a display named pdf_X.
    $pdf_display = NULL;
    foreach($view->display as $display) {
      if (preg_match('/^pdf_/', $display->id)) {
        // found a PDF display so break out of loop.
        $pdf_display = $display->id;
        break;
      }
    }

    // Set the found PDF display or automatically revert to default if it's
    // not found.
    $view->set_display($pdf_display);

    $view->pre_execute($args);

    $view->init_style();

    // Important only for pdf views. With this action we assign the
    // PDF document to the new view
    if (isset($this->view->pdf) && is_object($this->view->pdf)) {
      $view->pdf =& $this->view->pdf;

      $output = $view->render($view->display_handler->display->id);
    }
    else {
      $output = $view->display_handler->execute();
    }

    return $output;
  }

  function allow_advanced_render() {
    return FALSE;
  }
}
