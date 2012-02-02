<?php
// $Id: views_plugin_style_fullcalendar.inc,v 1.7.2.5 2011/02/09 16:10:06 timplunkett Exp $

/**
 * @file
 * Contains the fullcalendar style plugin.
 */

class views_plugin_style_fullcalendar extends views_plugin_style {
  function option_definition() {
    $options = parent::option_definition();
    $options['display'] = array(
      'contains' => array(
        'fc_view' => array('default' => 'month'),
        'fc_firstday' => array('default' => '0'),
        'fc_weekmode' => array('default' => 'fixed'),
      ),
    );
    $options['modules'] = array(
      'contains' => array(
        'fc_theme' => array('default' => FALSE),
        'fc_url_colorbox' => array('default' => FALSE),
        'fc_url_colorbox_class' => array('default' => '#content'),
        'fc_url_colorbox_width' => array('default' => '80%'),
        'fc_url_colorbox_height' => array('default' => '80%'),
        'fc_window' => array('default' => FALSE),
      ),
    );
    $options['header'] = array(
      'contains' => array(
        'fc_left' => array('default' => 'today prev,next'),
        'fc_center' => array('default' => 'title'),
        'fc_right' => array('default' => 'month agendaWeek agendaDay'),
      ),
    );
    $options['times'] = array(
      'contains' => array(
        'fc_timeformat' => array('default' => 'h:mm{ - h:mm}'),
        'fc_clock' => array('default' => FALSE),
        'fc_default_date' => array('default' => FALSE),
        'fc_date' => array('default' => array(
          'year' => '',
          'month' => '',
          'day' => '',
        )),
      ),
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display settings'),
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['display']['fc_view'] = array(
      '#type' => 'select',
      '#title' => t('Initial display'),
      '#options' => array(
        'month' => 'Month',
        'agendaWeek' => 'Week (Agenda)',
        'basicWeek' => 'Week (Basic)',
        'agendaDay' => 'Day (Agenda)',
        'basicDay' => 'Day (Basic)',
      ),
      '#description' => l(t('More info'), 'http://arshaw.com/fullcalendar/docs/views/Available_Views', array('attributes' => array('target' => '_blank'))),
      '#default_value' => $this->options['display']['fc_view'],
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
    );
    $form['display']['fc_firstday'] = array(
      '#type' => 'select',
      '#title' => t('Week starts on'),
      '#options' => array(
        '0' => t('Sunday'),
        '1' => t('Monday'),
      ),
      '#default_value' => $this->options['display']['fc_firstday'],
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
    );
    $form['display']['fc_weekmode'] = array(
      '#type' => 'select',
      '#title' => t('Week mode'),
      '#options' => array(
        'fixed' => 'Fixed',
        'liquid' => 'Liquid',
        'variable' => 'Variable',
      ),
      '#default_value' => $this->options['display']['fc_weekmode'],
      '#description' => l(t('More info'), 'http://arshaw.com/fullcalendar/docs/display/weekMode', array('attributes' => array('target' => '_blank'))),
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
    );
    $form['header'] = array(
      '#type' => 'fieldset',
      '#title' => t('Header settings'),
      '#description' => l(t('More info'), 'http://arshaw.com/fullcalendar/docs/display/header', array('attributes' => array('target' => '_blank'))),
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['header']['fc_left'] = array(
      '#type' => 'textfield',
      '#title' => t('Left'),
      '#default_value' => $this->options['header']['fc_left'],
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#size' => '20',
    );
    $form['header']['fc_center'] = array(
      '#type' => 'textfield',
      '#title' => t('Center'),
      '#default_value' => $this->options['header']['fc_center'],
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#size' => '20',
    );
    $form['header']['fc_right'] = array(
      '#type' => 'textfield',
      '#title' => t('Right'),
      '#default_value' => $this->options['header']['fc_right'],
      '#prefix' => '<div class="views-left-40">',
      '#suffix' => '</div>',
      '#size' => '30',
    );
    $form['times'] = array(
      '#type' => 'fieldset',
      '#title' => t('Time/date settings'),
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['times']['fc_timeformat'] = array(
      '#type' => 'textfield',
      '#title' => t('Time format'),
      '#description' => l(t('More info'), 'http://arshaw.com/fullcalendar/docs/utilities/formatDate', array('attributes' => array('target' => '_blank'))),
      '#default_value' => $this->options['times']['fc_timeformat'],
    );
    $form['times']['fc_clock'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use 24 hour display'),
      '#default_value' => $this->options['times']['fc_clock'],
    );
    $form['times']['fc_default_date'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use a custom default date'),
      '#default_value' => $this->options['times']['fc_default_date'],
    );
    $form['times']['fc_date'] = array(
      '#type' => 'date',
      '#title' => t('Custom default date'),
      '#default_value' => $this->options['times']['fc_date'],

      '#prefix' => '<div id="edit-style-options-times-fc-date-wrapper"><div id="edit-style-options-times-fc-date">',
      '#suffix' => '</div></div>',
      '#process' => array('form_process_date', 'ctools_dependent_process'),
      '#dependency' => array('edit-style-options-times-fc-default-date' => array(1)),
    );
    $form['modules'] = array(
      '#type' => 'fieldset',
      '#title' => t('Visual settings'),
      '#attributes' => array(
        'class' => 'clear-block',
      ),
    );
    $form['modules']['fc_theme'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use jQuery UI Theme'),
      '#default_value' => $this->options['modules']['fc_theme'],
    );
    if (module_exists('colorbox')) {
      $form['modules']['fc_url_colorbox'] = array(
        '#type' => 'checkbox',
        '#title' => t('Open events in colorbox'),
        '#default_value' => $this->options['modules']['fc_url_colorbox'],
      );
      $form['modules']['fc_url_colorbox_class'] = array(
        '#type' => 'textfield',
        '#title' => t('Classname or ID selector'),
        '#default_value' => $this->options['modules']['fc_url_colorbox_class'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('edit-style-options-modules-fc-url-colorbox' => array(1)),
      );
      $form['modules']['fc_url_colorbox_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Width'),
        '#default_value' => $this->options['modules']['fc_url_colorbox_width'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('edit-style-options-modules-fc-url-colorbox' => array(1)),
      );
      $form['modules']['fc_url_colorbox_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Height'),
        '#default_value' => $this->options['modules']['fc_url_colorbox_height'],
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('edit-style-options-modules-fc-url-colorbox' => array(1)),
      );
    }
    $form['modules']['fc_window'] = array(
      '#type' => 'checkbox',
      '#title' => t('Open events in same window'),
      '#default_value' => $this->options['modules']['fc_window'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-style-options-modules-fc-url-colorbox' => array(0)),
    );
  }
  function validate() {
    $errors = parent::validate();

    $field_options = $this->display->handler->get_field_labels();
    $date_fields = _fullcalendar_parse_fields($this->display->handler->get_handlers('field'), $field_options);

    if (empty($date_fields)) {
      $errors[] = t('Display "@display" requires at least one date field.', array('@display' => $this->display->display_title));
    }

    return $errors;
  }
}
