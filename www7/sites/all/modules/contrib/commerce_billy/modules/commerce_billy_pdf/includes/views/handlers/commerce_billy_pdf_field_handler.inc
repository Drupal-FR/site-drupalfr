<?php

/**
 * @file
 * Views handler for pdf link.
 */

/**
 * Simple entity generic views handler for a invoice PDF link.
 */
class commerce_billy_pdf_field_handler extends views_handler_field_entity {

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#description' => t('If left empty, the invoice number will be used.'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->render_link($entity, $values);
    }
  }

  function render_link($order, $values) {
    if (commerce_billy_pdf_view_access($order)) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "invoice-pdf/$order->order_id";
      $text = !empty($this->options['text']) ? $this->options['text'] : $order->order_number;
      return $text;
    }
  }
}
