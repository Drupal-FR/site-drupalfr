<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _features_test_content_default_fields() {
  $fields = array();

  // Exported field: field_features_test
  $fields[] = array(
    'field_name' => 'field_features_test',
    'type_name' => 'features_test',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_features_test][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Test',
      'weight' => '21',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  array(
    t('Test'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_filter_default_formats().
 */
function _features_test_filter_default_formats() {
  $formats = array();

  // Exported format: features_test
  $formats[] = array(
    'name' => 'features_test',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
    'filters' => array(
      '0' => array(
        'module' => 'filter',
        'delta' => '2',
        'weight' => '0',
      ),
      '1' => array(
        'module' => 'filter',
        'delta' => '0',
        'weight' => '1',
      ),
      '2' => array(
        'module' => 'filter',
        'delta' => '1',
        'weight' => '2',
      ),
      '3' => array(
        'module' => 'filter',
        'delta' => '3',
        'weight' => '10',
      ),
    ),
  );

  return $formats;
}

/**
 * Helper to implementation of hook_imagecache_default_presets().
 */
function _features_test_imagecache_default_presets() {
  $items = array(
    'features_test' => array(
      'presetname' => 'features_test',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '100%',
            'height' => '100%',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _features_test_user_default_permissions() {
  $permissions = array();

  // Exported permission: create features_test content
  $permissions[] = array(
    'name' => 'create features_test content',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  return $permissions;
}
