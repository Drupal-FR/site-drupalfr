<?php

/**
 * @file
 * Provide views data for modules making use of the entity CRUD API.
 */

/**
 * Implements hook_views_data()
 */
function entity_views_data() {
  $data = array();
  foreach (entity_crud_get_info() as $type => $info) {
    // Only enable the integration by default, it we know the module providing
    // the entity and it does not provide views integration yet.
    if (!isset($info['views controller class'])) {
      $info['views controller class'] = isset($info['module']) && !module_hook($info['module'], 'views_data') ? 'EntityDefaultViewsController' : FALSE;
    }
    if ($info['views controller class']) {
      $controller = new $info['views controller class']($type);
      // Relationship data may return views data for already existing tables,
      // so merge results on the second level.
      foreach ($controller->views_data() as $table => $table_data) {
        $data += array($table => array());
        $data[$table] = array_merge($data[$table], $table_data);
      }
    }
  }

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function entity_views_plugins() {
  foreach (views_fetch_data() as $table => $data) {
    if (!empty($data['table']['base']['entity type'])) {
      $base_tables[] = $table;
    }
  }
  if (!empty($base_tables)) {
    return array(
      'row' => array(
        'entity' => array(
          'title' => t('Entity'),
          'help' => t('Displays a single entity in a specific view mode (e.g. teaser).'),
          'handler' => 'entity_plugin_row_entity_view',
          'uses fields' => FALSE,
          'uses options' => TRUE,
          'type' => 'normal',
          'base' => $base_tables,
        ),
      ),
    );
  }
}

/**
 * Default controller for generating basic views integration.
 *
 * The controller tries to generate suiting views integration for the entity
 * based upon the schema information of its base table and the provided entity
 * property information.
 * For that it is possible to map a property name to its schema/views field
 * name by adding a 'schema field' key with the name of the field as value to
 * the property info.
 */
class EntityDefaultViewsController {

  protected $type, $info, $relationships;

  public function __construct($type) {
    $this->type = $type;
    $this->info = entity_get_info($type);
  }

  /**
   * Defines the result for hook_views_data().
   */
  public function views_data() {
    $data = array();
    $this->relationships = array();

    if (!empty($this->info['base table'])) {
      $table = $this->info['base table'];
      // Define the base group of this table. Fields that don't
      // have a group defined will go into this field by default.
      $data[$table]['table']['group']  = drupal_ucfirst($this->info['label']);

      $data[$table]['table']['base'] = array(
        'field' => $this->info['entity keys']['id'],
        'title' => drupal_ucfirst($this->info['label']),
        // @todo: Support an entity info description key or such?
        'help' => '',
        'entity type' => $this->type,
      );
      $data[$table] += $this->schema_fields();

      // Add in any reverse-relationships which have been determined.
      $data += $this->relationships;
    }
    return $data;
  }

  /**
   * Try to come up with some views fields with the help of the schema and
   * the entity property information.
   */
  protected function schema_fields() {
    $schema = drupal_get_schema($this->info['base table']);
    $properties = entity_get_property_info($this->type) + array('properties' => array());
    $data = array();

    foreach ($properties['properties'] as $name => $property_info) {
      if (isset($property_info['schema field']) && isset($schema['fields'][$property_info['schema field']])) {
        if ($views_info = $this->map_from_schema_info($name, $schema['fields'][$property_info['schema field']], $property_info)) {
          $data[$name] = $views_info;
        }
      }
    }
    return $data;
  }

  /**
   * Comes up with views information based on the given schema and property
   * info.
   */
  protected function map_from_schema_info($property_name, $schema_field_info, $property_info) {
    $type = isset($property_info['type']) ? $property_info['type'] : 'text';
    $views_field_name = $property_info['schema field'];

    $return = array();

    if (!empty($schema_field_info['serialize'])) {
      return FALSE;
    }

    $description = array(
      'title' => $property_info['label'],
      'help' => $property_info['description'],
    );

      // Add in relationships to related entities.
    if (($info = entity_get_info($type)) && !empty($info['base table'])) {

      // Prepare reversed relationship data.
      $label_lowercase = drupal_strtolower($this->info['label'][0]) . drupal_substr($this->info['label'], 1);
      $property_label_lowercase = drupal_strtolower($property_info['label'][0]) . drupal_substr($property_info['label'], 1);

      $this->relationships[$info['base table']][$this->info['base table']] = array(
        'title' => $this->info['label'],
        'help' => t("Associated @label via the @label's @property.", array('@label' => $label_lowercase, '@property' => $property_label_lowercase)),
        'relationship' => array(
          'label' => $this->info['label'],
          'handler' => 'views_handler_relationship',
          'base' => $this->info['base table'],
          'base field' => $views_field_name,
          'relationship field' => isset($info['entity keys']['name']) ? $info['entity keys']['name'] : $info['entity keys']['id'],
        ),
      );

      $return['relationship'] = array(
        'label' => drupal_ucfirst($info['label']),
        'handler' => 'views_handler_relationship',
        'base' => $info['base table'],
        'base field' => isset($info['entity keys']['name']) ? $info['entity keys']['name'] : $info['entity keys']['id'],
        'relationship field' => $views_field_name,
      );

      // Add in direct field/filters/sorts for the id itself too.
      $type = isset($info['entity keys']['name']) ? 'token' : 'integer';
      // Append the views-field-name to the title if it is different to the
      // property name.
      if ($property_name != $views_field_name) {
        $description['title'] .= ' ' . $views_field_name;
      }
    }

    switch ($type) {
      case 'token':
      case 'text':
        $return += $description + array(
          'field' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_field',
            'click sortable' => TRUE,
           ),
          'sort' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_sort',
          ),
          'filter' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_filter_string',
          ),
          'argument' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_argument_string',
          ),
        );

      case 'decimal':
      case 'integer':
        $return += $description + array(
          'field' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_field_numeric',
            'click sortable' => TRUE,
           ),
          'sort' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_sort',
          ),
          'filter' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_filter_numeric',
          ),
          'argument' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_argument_numeric',
          ),
        );

      case 'date':
        $return += $description + array(
          'field' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_field_date',
            'click sortable' => TRUE,
           ),
          'sort' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_sort_date',
          ),
          'filter' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_filter_date',
          ),
          'argument' => array(
            'real field' => $views_field_name,
            'handler' => 'views_handler_argument_date',
          ),
        );
    }

    // If there is an options list callback, add to the filter.
    if (isset($return['filter']) && !empty($property_info['options list'])) {
      $return['filter']['handler'] = 'views_handler_filter_in_operator';
      $return['filter']['options callback'] = array('EntityDefaultViewsController', 'optionsListCallback');
      $return['filter']['options arguments'] = array($this->type, $property_name, 'view');
    }
    return $return;
  }

  /**
   * A callback returning property options, suitable to be used as views options callback.
   */
  public static function optionsListCallback($type, $selector, $op = 'view') {
    $wrapper = entity_metadata_wrapper($type, NULL);
    $parts = explode(':', $selector);
    foreach ($parts as $part) {
      $wrapper = $wrapper->get($part);
    }
    return $wrapper->optionsList($op);
  }
}
