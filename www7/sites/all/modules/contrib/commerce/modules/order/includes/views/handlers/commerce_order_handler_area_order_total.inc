<?php

/**
 * Defines an area handler that renders an order's total field.
 */
class commerce_order_handler_area_order_total extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();

    $options['label_display'] = array(
      'default' => FALSE,
      'bool' => TRUE,
    );

    $options['label_custom'] = array(
      'default' => t('Order total'),
      'translatable' => TRUE,
    );

    $options['label_position'] = array(
      'default' => 'inline',
    );

    $options['formatter'] = array(
      'default' => 'commerce_price_formatted_components',
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['empty']['#description'] = t("Even if selected, this area handler will never render if a valid order cannot be found in the View's arguments.");

    $form['label_display'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a custom label for this field'),
      '#description' => t('Displaying a custom label may require updates to your theme to maintain the default alignment of the field without a label.'),
      '#default_value' => $this->options['label_display'],
    );

    $form['label_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom label'),
      '#default_value' => $this->options['label_custom'],
      '#dependency' => array(
        'edit-options-label-display' => array(1),
      ),
    );

    $form['label_position'] = array(
      '#type' => 'select',
      '#title' => t('Label position'),
      '#default_value' => $this->options['label_position'],
      '#dependency' => array(
        'edit-options-label-display' => array(1),
      ),
      '#options' => array(
        'inline' => t('Inline'),
        'above' => t('Above'),
      ),
    );

    // Load the formatter options for the commerce_order_total field.
    module_load_include('inc', 'field_ui', 'field_ui.admin');
    $field = field_info_field('commerce_order_total');

    $form['formatter'] = array(
      '#type' => 'select',
      '#title' => t('Formatter'),
      '#options' => field_ui_formatter_options($field['type']),
      '#default_value' => $this->options['formatter'],
    );
  }

  function render($empty = FALSE) {
    if (!$empty || !empty($this->options['empty'])) {
      // First look for an order_id argument.
      foreach ($this->view->argument as $name => $argument) {
        if ($argument instanceof commerce_order_handler_argument_order_order_id) {
          // If it is single value...
          if (count($argument->value) == 1) {
            // Load the order.
            if ($order = commerce_order_load(reset($argument->value))) {

              // Prepare a display settings array.
              $display = array(
                'label' => empty($this->options['label_display']) ? 'hidden' : $this->options['label_position'],
                'type' => $this->options['formatter'],
                'settings' => array(
                  'calculation' => FALSE,
                ),
              );

              // Render the order's order total field with the current display.
              $field = field_view_field('commerce_order', $order, 'commerce_order_total', $display);
              if (!empty($this->options['label_display'])) {
                $field['#title'] = $this->options['label_custom'];
              }

              return '<div class="commerce-order-handler-area-order-total">' . drupal_render($field) . '</div>';
            }
          }
        }
      }
    }

    return '';
  }
}
