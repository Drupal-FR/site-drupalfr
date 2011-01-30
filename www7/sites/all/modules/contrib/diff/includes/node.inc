<?php
// $Id: node.inc,v 1.1.2.2.2.2 2010/09/23 19:27:36 yhahn Exp $

/**
 * @file
 * Implementation of hook_diff() for node.module (body and title).
 */

/**
 * Implementation of hook_diff() for node.module (body and title).
 */
function node_diff($old_node, $new_node) {

  $result = array();
  $type = node_type_get_type($new_node);
  $result['title'] = array(
    '#name' => t($type->title_label),
    '#old' => array($old_node->title),
    '#new' => array($new_node->title),
    '#weight' => -5,
    '#format' => array(
      'show_header' => FALSE,
    )
  );

  // @TODO: abstract this to work with all field types and/or split this
  // integration out to be more generic.
  $instances = field_info_instances('node', field_extract_bundle('node', $type));
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    $langcode = field_language('node', $new_node, $field_name);
    if (isset($new_node->{$field_name}[$langcode])) {
      foreach (array_keys($new_node->{$field_name}[$langcode]) as $delta) {
        if (isset($new_node->{$field_name}[$langcode][$delta]['value'])) {
          $view_old = $old_node->{$field_name}[$langcode][$delta]['value'];
          $view_new = $new_node->{$field_name}[$langcode][$delta]['value'];
          $result["{$field_name}_{$delta}"] = array(
            '#name' => t($instance['label']),
            '#old' => explode("\n", $view_old),
            '#new' => explode("\n", $view_new),
          );
        }
      }
    }
  }
  return $result;
}
