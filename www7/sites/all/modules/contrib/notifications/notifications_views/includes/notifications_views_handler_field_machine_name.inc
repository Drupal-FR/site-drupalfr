<?php

/**
 * Field handler to translate a machine name into its readable form.
 */
class notifications_views_handler_field_machine_name extends views_handler_field_node {
  function option_definition() {
    $options = parent::option_definition();
    $options['machine_name'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide machine_name option for to node type display.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['machine_name'] = array(
      '#title' => t('Output machine name'),
      '#description' => t('Display field as the content type machine name.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['machine_name']),
      '#fieldset' => 'more',
    );
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->render_name($value, $values), $values);
  }
}
