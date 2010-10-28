<?php
// $Id: xmlsitemap.admin.inc,v 1.1.2.93 2010/02/19 02:19:55 davereid Exp $

/**
 * @file
 * Administrative page callbacks for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * Form builder; Administration settings form.
 *
 * @see system_settings_form()
 * @see xmlsitemap_settings_form_validate()
 */
function xmlsitemap_settings_form() {
  $form['xmlsitemap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['xmlsitemap']['xmlsitemap_minimum_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Minimum sitemap lifetime'),
    '#options' => array(0 => t('No minimum')) + drupal_map_assoc(array(300, 900, 1800, 3600, 10800, 21600, 43200, 86400, 172800, 259200, 604800), 'format_interval'),
    '#default_value' => xmlsitemap_var('minimum_lifetime')
  );
  $form['xmlsitemap']['xmlsitemap_xsl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include a stylesheet in the sitemaps for humans.'),
    '#default_value' => xmlsitemap_var('xsl'),
    '#description' => t('Using the stylesheet will add formatting and tables with sorting to make it easier to view the XML sitemap data instead of viewing raw XML output. Search engines will ignore any formatting.')
  );
  $form['xmlsitemap']['xmlsitemap_languages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Generate sitemaps for the following languages'),
    '#options' => array(language_default('language') => language_default('name')),
    '#default_value' => xmlsitemap_var('languages'),
    '#process' => array('expand_checkboxes', '_xmlsitemap_process_language_checkboxes'),
    '#description' => !module_exists('xmlsitemap_i18n') ? t('To enable multilingual features, enable the XML sitemap internationalization module.') : '',
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('xmlsitemap_developer_mode', 0),
    '#weight' => 10,
  );
  //$form['advanced']['xmlsitemap_gz'] = array(
  //  '#type' => 'checkbox',
  //  '#title' => t('Generate additional compressed sitemaps using gzip.'),
  //  '#default_value' => xmlsitemap_var('gz'),
  //  '#disabled' => !function_exists('gzencode'),
  //);
  $form['advanced']['xmlsitemap_chunk_size'] = array(
    '#type' => 'select',
    '#title' => t('Number of links in each sitemap page'),
    '#options' => array('auto' => t('Automatic (recommended)')) + drupal_map_assoc(array(100, 500, 1000, 2500, 5000, 10000, 25000, 50000)),
    '#default_value' => xmlsitemap_var('chunk_size'),
    // @todo This description is not clear.
    '#description' => t('If there are problems with rebuilding the sitemap, you may want to manually set this value. If you have more than 50,000 links, an index with multiple sitemap pages will be generated. There is a maximum of 1000 sitemap pages.'),
  );
  $form['advanced']['xmlsitemap_batch_limit'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of sitemap links to process at once'),
    '#options' => drupal_map_assoc(array(5, 10, 25, 50, 100, 250, 500, 1000, 2500, 5000)),
    '#default_value' => xmlsitemap_var('batch_limit'),
    '#description' => t('If you have problems running cron or rebuilding the sitemap, you may want to lower this value.'),
  );
  file_check_directory(xmlsitemap_get_directory(), FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS, 'xmlsitemap_path');
  $form['advanced']['xmlsitemap_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Sitemap cache directory'),
    '#default_value' => variable_get('xmlsitemap_path', 'xmlsitemap'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('Subdirectory where the sitemap data will be stored. This folder <strong>must not be shared</strong> with any other Drupal site or install using XML sitemap.'),
    '#field_prefix' =>  file_directory_path() . '/',
    '#required' => TRUE,
  );
  $form['advanced']['xmlsitemap_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#default_value' => xmlsitemap_var('base_url'),
    '#size' => 30,
    '#description' => t('This is the base URL for links generated in the sitemap.'),
    '#required' => TRUE,
  );
  $form['advanced']['xmlsitemap_lastmod_format'] = array(
    '#type' => 'select',
    '#title' => t('Last modification date format'),
    '#options' => array(
      'Y-m-d' => t('Short'),
      'Y-m-d\TH:i\Z' => t('Medium'),
      'c' => t('Long'),
    ),
    '#default_value' => variable_get('xmlsitemap_lastmod_format', 'Y-m-d\TH:i\Z'),
  );
  foreach ($form['advanced']['xmlsitemap_lastmod_format']['#options'] as $key => &$label) {
    $label .= ' (' . gmdate($key, REQUEST_TIME) . ')';
  }
  $form['advanced']['xmlsitemap_developer_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable developer mode to expose additional settings.'),
    '#default_value' => variable_get('xmlsitemap_developer_mode', 0),
  );

  $entities = xmlsitemap_get_link_info(NULL, TRUE);
  foreach ($entities as $entity => $entity_info) {
    $form[$entity] = array(
      '#type' => 'fieldset',
      '#title' => $entity_info['label'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'xmlsitemap_settings',
      '#weight' => 30,
    );

    if (!empty($entity_info['bundles'])) {
      // If this entity has bundles, show a bundle setting summary.
      xmlsitemap_add_form_entity_summary($form[$entity], $entity, $entity_info);
    }

    if (!empty($entity_info['xmlsitemap']['settings callback'])) {
      // Add any entity-specific settings.
      $entity_info['xmlsitemap']['settings callback']($form[$entity]);
    }

    // Ensure that the entity fieldset is not shown if there are no accessible
    // sub-elements.
    module_load_include('inc', 'xmlsitemap');
    $form[$entity]['#access'] = (bool) xmlsitemap_element_get_visible_children($form[$entity]);
  }

  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';
  $form['#validate'][] = 'xmlsitemap_settings_form_validate';
  $form['#submit'][] = 'xmlsitemap_settings_form_submit';
  $form['array_filter'] = array('#type' => 'value', '#value' => TRUE);

  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 100;
  return $form;
}

/**
 * Show a link to each languages' sitemap and disable the default language
 * checkbox.
 */
function _xmlsitemap_process_language_checkboxes($element) {
  foreach (element_children($element) as $key) {
    if ($key == language_default('language')) {
      $element[$key]['#disabled'] = TRUE;
      $element[$key]['#default_value'] = TRUE;
      $element[$key]['#weight'] = -1;
    }
    $link = url('sitemap.xml', array('absolute' => TRUE, 'language' => xmlsitemap_language_load($key)));
    $element[$key]['#description'] = l($link, $link);
  }
  return $element;
}

/**
 * Form validator; Check the sitemap files directory.
 *
 * @see xmlsitemap_settings_form()
 */
function xmlsitemap_settings_form_validate($form, &$form_state) {
  // Check that the chunk size will not create more than 1000 chunks.
  $chunk_size = $form_state['values']['xmlsitemap_chunk_size'];
  if ($chunk_size != 'auto' && $chunk_size != 50000 && (xmlsitemap_get_link_count() / $chunk_size) > 1000) {
    form_set_error('xmlsitemap_chunk_size', t('The sitemap page link count of @size will create more than 1,000 sitemap pages. Please increase the link count.', array('@size' => $chunk_size)));
  }

  $base_url = &$form_state['values']['xmlsitemap_base_url'];
  $base_url = rtrim($base_url, '/');
  if ($base_url != '' && !valid_url($base_url, TRUE)) {
    form_set_error('xmlsitemap_base_url', t('Invalid base URL.'));
  }
}

/**
 * Submit handler;
 *
 * @see xmlsitemap_settings_form()
 */
function xmlsitemap_settings_form_submit($form, $form_state) {
  // Save any changes to the frontpage link.
  xmlsitemap_save_link(array('type' => 'frontpage', 'id' => 0, 'loc' => ''));
}

/**
 * Menu callback; Confirm rebuilding of the sitemap.
 *
 * @see xmlsitemap_rebuild_form_submit()
 */
function xmlsitemap_rebuild_form() {
  if (!$_POST && !variable_get('xmlsitemap_rebuild_needed', FALSE)) {
    if (!variable_get('xmlsitemap_regenerate_needed', FALSE)) {
      drupal_set_message(t('Your sitemap is up to date and does not need to be rebuilt.'), 'error');
    }
    else {
      $_REQUEST += array('destination' => 'admin/settings/xmlsitemap');
      drupal_set_message(t('A rebuild is not necessary. If you are just wanting to regenerate the XML sitemap files, you can <a href="@link-cron">run cron manually</a>.', array('@link-cron' => url('admin/reports/status/run-cron', array('query' => drupal_get_destination())))), 'warning');
    }
  }

  // Build the list of rebuildable entities.
  $entities = xmlsitemap_get_link_info();
  $callbacks = $options = array();
  foreach ($entities as $entity => $info) {
    if (empty($info['xmlsitemap']['rebuild callback'])) {
      // If the entity is missing a rebuild callback, skip.
      continue;
    }
    if (!empty($info['object keys']['bundle']) && !xmlsitemap_get_link_type_enabled_bundles($entity)) {
      // If the entity has bundles, but no enabled bundles, skip since
      // rebuilding wouldn't get any links.
      continue;
    }
    else {
      // Build the list of callbacks and options for the form and batch
      // processing.
      $callbacks[$entity] = $info['xmlsitemap']['rebuild callback'];
      $options[$entity] = $info['label'];
    }
  }

  $form['callbacks'] = array(
    '#type' => 'value',
    '#value' => $callbacks,
  );
  $form['entities'] = array(
    '#type' => 'select',
    '#title' => t("Select which link types you would like to rebuild"),
    '#description' => t('If no link types are selected, the sitemap files will just be regenerated.'),
    '#multiple' => TRUE,
    '#options' => $options,
    '#default_value' => variable_get('xmlsitemap_rebuild_needed', FALSE) || !variable_get('xmlsitemap_developer_mode', 0) ? array_keys($options) : array(),
    '#access' => variable_get('xmlsitemap_developer_mode', 0),
  );
  $form['save_custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save and restore any custom inclusion and priority links.'),
    '#default_value' => TRUE,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to rebuild the sitemap?'),
    'admin/settings/xmlsitemap',
    '',
    t('Rebuild sitemap'),
    t('Cancel')
  );
}

/**
 * Submit handler; Starts the sitemap rebuild batch.
 *
 * @see xmlsitemap_rebuild_form()
 * @see xmlsitemap_rebuild_batch()
 */
function xmlsitemap_rebuild_form_submit($form, &$form_state) {
  // Set the rebuild flag incase something fails during the rebuild.
  variable_set('xmlsitemap_rebuild_needed', TRUE);

  module_load_include('inc', 'xmlsitemap', 'xmlsitemap.generate');
  $batch = xmlsitemap_rebuild_batch($form_state['values']['entities'], $form_state['values']['callbacks'], $form_state['values']['save_custom']);
  batch_set($batch);
  $form_state['redirect'] = 'admin/settings/xmlsitemap';
}

/**
 * Add a table summary for an entity and its bundles.
 */
function xmlsitemap_add_form_entity_summary(&$form, $entity, array $entity_info) {
  $priorities = xmlsitemap_get_priority_options(NULL, FALSE);
  $statuses = xmlsitemap_get_status_options(NULL);
  $destination = drupal_get_destination();

  $rows = array();
  $totals = array('total' => 0, 'indexed' => 0, 'visible' => 0);
  foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
    // Fetch current per-bundle link total and indexed counts.
    $status = xmlsitemap_get_link_type_indexed_status($entity, $bundle);
    $totals['total'] += $status['total'];
    $totals['indexed'] += $status['indexed'];
    $totals['visible'] += $status['visible'];

    $row = array();
    if (xmlsitemap_can_admin_bundle($entity, $bundle) && $path = xmlsitemap_get_bundle_path($entity, $bundle)) {
      $options = array('query' => $destination);
      if (module_exists('ctools') && module_exists('dialog')) {
        dialog_add_js();
        $path = "admin/settings/xmlsitemap/settings/$entity/$bundle/nojs";
        $options['attributes'] = array('class' => 'ctools-use-dialog');
      }
      $row[] = l($bundle_info['label'], $path, $options);
    }
    else {
      $row[] = $bundle_info['label'];
    }
    $row[] = $statuses[$bundle_info['xmlsitemap']['status'] ? 1 : 0];
    $row[] = $priorities[number_format($bundle_info['xmlsitemap']['priority'], 1)];
    $row[] = $status['total'];
    $row[] = $status['indexed'];
    $row[] = $status['visible'];
    $rows[] = $row;
  }

  if ($rows) {
    $header = array(
      $entity_info['bundle label'],
      t('Inclusion'),
      t('Priority'),
      t('Available'),
      t('Indexed'),
      t('Visible'),
    );
    $rows[] = array(
      array(
        'data' => t('Totals'),
        'colspan' => 3,
        'header' => TRUE,
      ),
      array(
        'data' => $totals['total'],
        'header' => TRUE,
      ),
      array(
        'data' => $totals['indexed'],
        'header' => TRUE,
      ),
      array(
        'data' => $totals['visible'],
        'header' => TRUE,
      ),
    );
    $form['summary'] = array(
      '#value' => theme('table', $header, $rows),
    );
  }
}

/**
 * Add the link type XML sitemap options to the link type's form.
 *
 * Caller is responsible for ensuring xmlsitemap_link_bundle_settings_save()
 * is called during submission.
 */
function xmlsitemap_add_link_bundle_settings(array &$form, array &$form_state, $entity, $bundle) {
  $form += xmlsitemap_link_bundle_settings_form($form_state, $entity, $bundle, FALSE);

  // Manually call drupal_alter() on the $options form since it was not called
  // using drupal_get_form().
  $form_id = 'xmlsitemap_link_bundle_settings_form';
  $data = &$form;
  $data['__drupal_alter_by_ref'] = array(&$form_state);
  drupal_alter('form_'. $form_id, $data);
  $data['__drupal_alter_by_ref'] = array(&$form_state);
  drupal_alter('form', $data, $form_id);

  // Make the base element a fieldset.
  $form['xmlsitemap'] += array(
    '#type' => 'fieldset',
    '#title' => t('XML sitemap'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('administer xmlsitemap'),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.js',
      ),
    ),
  );
}

function xmlsitemap_link_bundle_settings_form(array $form_state, $entity, $bundle, $buttons = TRUE) {
  $settings = xmlsitemap_link_bundle_load($entity, $bundle);

  $form['xmlsitemap'] = array(
    '#tree' => TRUE,
    '#entity' => $entity,
    '#bundle' => $bundle,
  );
  $form['xmlsitemap']['description'] = array(
    '#prefix' => '<div class="description">',
    '#suffix' => '</div>',
    '#value' => t('Changing these type settings will affect any items of this type that have either inclusion or priority set to default.'),
  );
  $form['xmlsitemap']['status'] = array(
    '#type' => 'select',
    '#title' => t('Inclusion'),
    '#options' => xmlsitemap_get_status_options(),
    '#default_value' => $settings['status'],
  );
  $form['xmlsitemap']['priority'] = array(
    '#type' => 'select',
    '#title' => t('Default priority'),
    '#options' => xmlsitemap_get_priority_options(),
    '#default_value' => $settings['priority'],
    '#process' => array('form_expand_ahah', 'ctools_dependent_process'),
    '#dependency' => array('edit-xmlsitemap-status' => array(1)),
  );

  // Hide the priority field if the link is excluded.
  if (module_exists('ctools')) {
    ctools_include('dependent');
  }

  if ($buttons) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 100,
    );
    if (!empty($form_state['ajax'])) {
      $form['submit']['#attributes']['class'] = 'ctools-dialog-button';
    }
  }

  return $form;
}

function xmlsitemap_link_bundle_settings_form_submit($form, $form_state) {
  $entity = $form['xmlsitemap']['#entity'];
  $bundle = $form['xmlsitemap']['#bundle'];
  $info = xmlsitemap_get_link_info($entity);
  xmlsitemap_link_bundle_settings_save($entity, $bundle, $form_state['values']['xmlsitemap']);
  drupal_set_message(t('Settings for the @bundle-label %bundle have been saved.', array('@bundle-label' => drupal_strtolower($info['bundle label']), '%bundle' => $info['bundles'][$bundle]['label'])));
}

function xmlsitemap_bundle_settings_dialog($entity, $bundle, $ajax = FALSE) {
  $info = xmlsitemap_get_link_info($entity);

  if (!$ajax) {
    if ($path = xmlsitemap_get_bundle_path($entity, $bundle)) {
      $destination = drupal_get_destination();
      unset($_REQUEST['destination']);
      drupal_goto($path, array('query' => $destination));
    }
    return drupal_not_found();
  }

  ctools_include('ajax');
  if (!isset($info['bundles'][$bundle]) || !xmlsitemap_can_admin_bundle($entity, $bundle)) {
    $commands = array();
    $commands[] = dialog_command_display(t('Access denied'), t('You are not authorized to access this page.'));
    ctools_ajax_render($commands);
  }

  $form_state = array(
    'ajax' => TRUE,
    'title' => t('@bundle XML sitemap settings', array('@bundle' => $info['bundles'][$bundle]['label'])),
    'args' => array($entity, $bundle),
  );
  $output = dialog_form_wrapper('xmlsitemap_link_bundle_settings_form', $form_state);

  if (empty($output)) {
    $output[] = dialog_command_display(t('Settings saved'), t('Reloading...'));
    $output[] = ctools_ajax_command_reload();
  }
  ctools_ajax_render($output);
}

/**
 * Add a link's XML sitemap options to the link's form.
 *
 * @todo Add changefreq overridability.
 */
function xmlsitemap_add_form_link_options(&$form, $entity, $bundle, $id) {
  $info = xmlsitemap_get_link_info($entity);

  if (!$info || empty($info['bundles'][$bundle])) {
    return;
  }

  if (!$link = xmlsitemap_link_load($entity, $id)) {
    $link = array();
  }

  $settings = xmlsitemap_link_bundle_load($entity, $bundle);
  $link += array(
    'access' => 1,
    'status' => $settings['status'],
    'status_default' => $settings['status'],
    'status_override' => 0,
    'priority' => $settings['priority'],
    'priority_default' => $settings['priority'],
    'priority_override' => 0,
  );

  $form['xmlsitemap'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('XML sitemap'),
    '#collapsible' => TRUE,
    '#collapsed' => !$link['status_override'] && !$link['priority_override'],
    '#access' => user_access('administer xmlsitemap') || xmlsitemap_can_admin_bundle($entity, $bundle),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.js',
      ),
    ),
  );

  $bundle_info = $info['bundles'][$bundle];
  if (xmlsitemap_can_admin_bundle($entity, $bundle) && $path = xmlsitemap_get_bundle_path($entity, $bundle)) {
    $form['xmlsitemap']['description'] = array(
      '#prefix' => '<div class="description">',
      '#suffix' => '</div>',
      '#value' => t('The default XML sitemap settings for this @bundle can be changed <a href="@link-type">here</a>.', array('@bundle' => drupal_strtolower($info['bundle label']), '@link-type' => url($path, array('query' => drupal_get_destination())))),
    );
  }

  // Show a warning if the link is not accessible and will not be included in
  // the sitemap.
  if ($id && !$link['access']) {
    $form['xmlsitemap']['warning'] = array(
      '#type' => 'markup',
      '#prefix' => '<p><strong>',
      '#suffix' => '</strong></p>',
      '#value' => ('This item is not currently visible to anonymous users, so it will not be included in the sitemap.'),
    );
  }

  // Status field (inclusion/exclusion)
  $form['xmlsitemap']['status'] = array(
    '#type' => 'select',
    '#title' => t('Inclusion'),
    '#options' => xmlsitemap_get_status_options($link['status_default']),
    '#default_value' => $link['status_override'] ? $link['status'] : 'default',
  );
  $form['xmlsitemap']['status_default'] = array(
    '#type' => 'value',
    '#value' => $link['status_default'],
  );
  $form['xmlsitemap']['status_override'] = array(
    '#type' => 'value',
    '#value' => $link['status_override'],
  );

  // Priority field
  $form['xmlsitemap']['priority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#options' => xmlsitemap_get_priority_options($link['priority_default']),
    '#default_value' => $link['priority_override'] ? number_format($link['priority'], 1) : 'default',
    '#description' => t('The priority of this URL relative to other URLs on your site.'),
    '#process' => array('form_expand_ahah', 'ctools_dependent_process'),
    '#dependency' => array('edit-xmlsitemap-status' => array(1)),
  );
  if ($link['status_default']) {
    $form['xmlsitemap']['priority']['#dependency']['edit-xmlsitemap-status'][] = 'default';
  }
  $form['xmlsitemap']['priority_default'] = array(
    '#type' => 'value',
    '#value' => $link['priority_default'],
  );
  $form['xmlsitemap']['priority_override'] = array(
    '#type' => 'value',
    '#value' => $link['priority_override'],
  );

  // Other persistent fields.
  //$form['xmlsitemap']['lastmod'] = array(
  //  '#type' => 'value',
  //  '#value' => $node->xmlsitemap['lastmod'],
  //);
  //$form['xmlsitemap']['changefreq'] = array(
  //  '#type' => 'value',
  //  '#value' => $node->xmlsitemap['changefreq'],
  //);
  //$form['xmlsitemap']['changecount'] = array(
  //  '#type' => 'value',
  //  '#value' => $node->xmlsitemap['changecount'],
  //);

  // Hide the priority field if the link is excluded.
  if (module_exists('ctools')) {
    ctools_include('dependent');
  }

  // Add the submit handler to adjust the default values if selected.
  if (!in_array('xmlsitemap_process_form_link_options', $form['#submit'])) {
    array_unshift($form['#submit'], 'xmlsitemap_process_form_link_options');
  }
}

/**
 * Get a list of priority options.
 *
 * @param $default
 *   Include a 'default' option.
 * @param $guides
 *   Add helpful indicators for the highest, middle and lowest values.
 * @return
 *   An array of options.
 */
function xmlsitemap_get_priority_options($default = NULL, $guides = TRUE) {
  $options = array();
  $priorities = array(
    '1.0' => t('1.0'),
    '0.9' => t('0.9'),
    '0.8' => t('0.8'),
    '0.7' => t('0.7'),
    '0.6' => t('0.6'),
    '0.5' => t('0.5'),
    '0.4' => t('0.4'),
    '0.3' => t('0.3'),
    '0.2' => t('0.2'),
    '0.1' => t('0.1'),
    '0.0' => t('0.0'),
  );

  if (isset($default)) {
    $default = number_format($default, 1);
    $options['default'] = t('Default (@value)', array('@value' => $priorities[$default]));
  }

  // Add the rest of the options.
  $options += $priorities;

  if ($guides) {
    $options['1.0'] .= ' ' . t('(highest)');
    $options['0.5'] .= ' ' . t('(normal)');
    $options['0.0'] .= ' ' . t('(lowest)');
  }

  return $options;
}

/**
 * Get a list of priority options.
 *
 * @param $default
 *   Include a 'default' option.
 * @return
 *   An array of options.
 *
 * @see _xmlsitemap_translation_strings()
 */
function xmlsitemap_get_status_options($default = NULL) {
  $options = array();
  $statuses = array(
    1 => t('Included'),
    0 => t('Excluded'),
  );

  if (isset($default)) {
    $default = $default ? 1 : 0;
    $options['default'] = t('Default (@value)', array('@value' => drupal_strtolower($statuses[$default])));
  }

  $options += $statuses;

  return $options;
}
