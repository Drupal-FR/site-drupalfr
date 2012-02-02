<?php
// $Id: views_plugin_node_fullcalendar.inc,v 1.5.2.7 2011/02/11 05:35:16 timplunkett Exp $

/**
 * @file
 * Contains the node view row style plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 */
class views_plugin_node_fullcalendar extends views_plugin_row {

  function option_definition() {
    $options = parent::option_definition();
    $options['custom'] = array(
      'contains' => array(
        'fc_title_field' => array('default' => ''),
        'fc_url_field' => array('default' => ''),
        'fc_date_field' => array('default' => ''),
        'fc_title' => array('default' => FALSE),
        'fc_url' => array('default' => FALSE),
        'fc_date' => array('default' => FALSE),
      ),
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $field_options = $this->display->handler->get_field_labels();
    $date_fields = _fullcalendar_parse_fields($this->display->handler->get_handlers('field'), $field_options);

    $form['custom'] = array(
      '#type' => 'fieldset',
      '#title' => t('Customize fields'),
      '#description' => t('Add fields to the view in order to customize fields below.'),
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['custom']['fc_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use a custom title'),
      '#default_value' => $this->options['custom']['fc_title'],
    );
    $form['custom']['fc_title_field'] = array(
      '#type' => 'select',
      '#title' => t('Title Field'),
      '#options' => $field_options,
      '#default_value' => $this->options['custom']['fc_title_field'],
      '#description' => t('Choose the field with the custom title.'),
      '#process' => array('form_process_select', 'ctools_dependent_process'),
      '#dependency' => array('edit-row-options-custom-fc-title' => array(1)),
    );
    $form['custom']['fc_url'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use a custom redirect URL'),
      '#default_value' => $this->options['custom']['fc_url'],
    );
    $form['custom']['fc_url_field'] = array(
      '#type' => 'select',
      '#title' => t('URL Field'),
      '#options' => $field_options,
      '#default_value' => $this->options['custom']['fc_url_field'],
      '#description' => t('Choose the field with the custom link.'),
      '#process' => array('form_process_select', 'ctools_dependent_process'),
      '#dependency' => array('edit-row-options-custom-fc-url' => array(1)),
    );
    $form['custom']['fc_date'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use a custom date field'),
      '#default_value' => $this->options['custom']['fc_date'],
    );
    $form['custom']['fc_date_field'] = array(
      '#type' => 'select',
      '#title' => t('Date Fields'),
      '#options' => $date_fields,
      '#default_value' => $this->options['custom']['fc_date_field'],
      '#description' => t('Select one or more date fields.'),
      '#multiple' => TRUE,
      '#size' => count($date_fields),
      '#process' => array('form_process_select', 'ctools_dependent_process'),
      '#dependency' => array('edit-row-options-custom-fc-date' => array(1)),
    );

    // Disable form elements when not needed.
    if (empty($field_options)) {
      $form['custom']['#description'] = t('All the options are hidden, you need to add fields first.');
      $form['custom']['fc_title']['#type'] = 'hidden';
      $form['custom']['fc_title_field']['#disabled'] = TRUE;
      $form['custom']['fc_url']['#type'] = 'hidden';
      $form['custom']['fc_url_field']['#disabled'] = TRUE;
      $form['custom']['fc_date']['#type'] = 'hidden';
      $form['custom']['fc_date_field']['#disabled'] = TRUE;
    }
    if (empty($date_fields)) {
      $form['custom']['fc_date']['#type'] = 'hidden';
      $form['custom']['fc_date_field']['#disabled'] = TRUE;
    }
  }

  function render($row) {
    return theme($this->theme_functions(), array(
      'view' => $this->view,
      'options' => $this->options,
      'row' => $row,
    ));
  }
}
