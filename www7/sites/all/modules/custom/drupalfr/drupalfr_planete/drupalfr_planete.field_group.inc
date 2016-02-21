<?php
/**
 * @file
 * drupalfr_planete.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function drupalfr_planete_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_planet|user|user|form';
  $field_group->group_name = 'group_user_planet';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Planète',
    'weight' => '4',
    'children' => array(
      0 => 'field_planete_rss',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-user-planet field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_user_planet|user|user|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Planète');

  return $field_groups;
}
