<?php
/**
 * @file
 * drupalfr_user.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function drupalfr_user_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_drupal_settings|user|user|form';
  $field_group->group_name = 'group_user_drupal_settings';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Paramètres Drupal',
    'weight' => '0',
    'children' => array(
      0 => 'googleanalytics',
      1 => 'mollom',
      2 => 'redirect',
      3 => 'account',
      4 => 'timezone',
      5 => 'signature_settings',
      6 => 'contact',
      7 => 'locale',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-user-drupal-settings field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_user_drupal_settings|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_drupal_settings|user|user|full';
  $field_group->group_name = 'group_user_drupal_settings';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Paramètres Drupal',
    'weight' => '5',
    'children' => array(),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(),
    ),
  );
  $field_groups['group_user_drupal_settings|user|user|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_drupal|user|user|form';
  $field_group->group_name = 'group_user_drupal';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Drupal',
    'weight' => '3',
    'children' => array(
      0 => 'field_events',
      1 => 'field_drupal_org',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-user-drupal field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_user_drupal|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_drupal|user|user|full';
  $field_group->group_name = 'group_user_drupal';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Drupal',
    'weight' => '3',
    'children' => array(
      0 => 'field_events',
      1 => 'field_drupal_org',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Drupal',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_user_drupal|user|user|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_information|user|user|form';
  $field_group->group_name = 'group_user_information';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Informations',
    'weight' => '1',
    'children' => array(
      0 => 'field_biography',
      1 => 'field_company',
      2 => 'field_picture',
      3 => 'field_realname',
      4 => 'field_website',
      5 => 'field_membership_number',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-user-information field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_user_information|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_information|user|user|full';
  $field_group->group_name = 'group_user_information';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Informations',
    'weight' => '1',
    'children' => array(
      0 => 'field_biography',
      1 => 'field_company',
      2 => 'field_picture',
      3 => 'field_realname',
      4 => 'field_website',
      5 => 'adherent',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Informations',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_user_information|user|user|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_newsletter|user|user|form';
  $field_group->group_name = 'group_user_newsletter';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Newsletters',
    'weight' => '5',
    'children' => array(
      0 => 'field_newsletter_adff',
      1 => 'field_newsletter_dc_soleil_2014',
      2 => 'field_newsletter_ddd_2015',
      3 => 'field_newsletter_drupalcamp',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-user-newsletter field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_user_newsletter|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_planet|user|user|full';
  $field_group->group_name = 'group_user_planet';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Planète',
    'weight' => '6',
    'children' => array(),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(),
    ),
  );
  $field_groups['group_user_planet|user|user|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_social|user|user|form';
  $field_group->group_name = 'group_user_social';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '2',
    'children' => array(
      0 => 'field_icq',
      1 => 'field_jabber',
      2 => 'field_msn',
      3 => 'field_nickname',
      4 => 'field_facebook',
      5 => 'field_google_plus',
      6 => 'field_linkedin',
      7 => 'field_twitter',
      8 => 'field_viadeo',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-user-social field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_user_social|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_user_social|user|user|full';
  $field_group->group_name = 'group_user_social';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '2',
    'children' => array(
      0 => 'field_icq',
      1 => 'field_jabber',
      2 => 'field_msn',
      3 => 'field_nickname',
      4 => 'field_twitter',
      5 => 'field_facebook',
      6 => 'field_linkedin',
      7 => 'field_viadeo',
      8 => 'field_google_plus',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Social',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_user_social|user|user|full'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Drupal');
  t('Informations');
  t('Newsletters');
  t('Paramètres Drupal');
  t('Planète');
  t('Social');

  return $field_groups;
}
