<?php

/**
 * @file
 * Integration with the Feeds module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 */
function addressfield_feeds_processor_targets_alter(&$targets, $entity_type, $bundle) {
  foreach (field_info_instances($entity_type, $bundle) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'addressfield') {
      foreach ($info['columns'] as $sub_field => $schema_info) {
        $name_label = $instance['label'] . ': ' . drupal_ucfirst(str_replace('_', ' ', $sub_field));
        $targets[$name . ':' . $sub_field] = array(
          'name' => $name_label,
          'callback' => 'addressfield_set_target',
          'real_target' => $info['field_name'],
          'description' => $schema_info['description'],
        );
      }
    }
  }
}

/**
 * Callback for hook_feeds_processor_targets_alter().
 *
 * @param $source
 *   Field mapper source settings.
 * @param $entity
 *   An entity object, for instance a node object.
 * @param $target
 *   A string identifying the target on the node.
 * @param $values
 *   The value to populate the target with.
 * @param array $mapping
 *  Associative array of the mapping settings from the per mapping
 *  configuration form.
 */
function addressfield_set_target($source, $entity, $target, $values, $mapping) {
  $language = $mapping['language'];
  list($field_name, $sub_field) = explode(':', $target, 2);

  // Field info and instance are required for setting default values.
  $entity_type = $source->importer->processor->entityType();
  list(, , $bundle_name) = entity_extract_ids($entity_type, $entity);
  $info = field_info_field($field_name);
  $instance = field_info_instance($entity_type, $field_name, $bundle_name);

  // Convert the values into an array if it isn't one already to correspond to
  // Drupal's handling of field value arrays.
  if (!is_array($values)) {
    $values = array($values);
  }

  // If the field is already set on the given entity, update the existing value
  // array. Otherwise start with a fresh field value array.
  $field = isset($entity->{$field_name}) ? $entity->{$field_name} : array();

  // Loop over the field values array...
  foreach ($values as $delta => $value) {
    // Set defaults for new values.
    if (!isset($field[$language][$delta])) {
      $field[$language][$delta] = addressfield_default_values($info, $instance);
    }
    $field[$language][$delta][$sub_field] = $value;
  }

  $entity->{$field_name} = $field;
}
