<?php
/**
 * @file
 * drupalfr_user.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function drupalfr_user_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_biography'
  $field_bases['field_biography'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_biography',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 1,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_company'
  $field_bases['field_company'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_company',
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'annuaire' => 'annuaire',
        'article' => 0,
        'page' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 1,
    'type' => 'node_reference',
  );

  // Exported field_base: 'field_events'
  $field_bases['field_events'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_events',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'drupalcamp-paris-1' => 'J’ai participé au DrupalCamp du 12 janvier 2007 à Paris',
        'drupalcamp-paris-2' => 'J’ai participé au DrupalCamp du 19 avril 2008 à Paris',
        'drupalcamp-paris-3' => 'J\'ai participé au DrupalCamp + Drupalngo d\'octobre - novembre 2008 à Paris',
        'drupalcamp-lyon' => 'J\'ai participé au DrupalCamp du 28 mars 2009 à Lyon',
        'drupalcamp-paris-4' => 'J’ai participé au DrupalCamp du 1 septembre 2009 à Paris',
        'drupalcamp-nantes' => 'J’ai participé au DrupalCamp du 29 janvier 2011 à Nantes',
        'drupalcamp-paris-5' => 'J’ai participé au DrupalCamp du 18 juin 2011 à Paris',
        'drupalcamp-toulouse' => 'J’ai participé au DrupalCamp du 26 novembre 2011 à Toulouse',
        'drupalcamp-lyon-2' => 'J’ai participé au DrupalCamp du 26 mai 2012 à Lyon',
        'dcparis13' => 'J\'ai participé au Drupalcamp Paris 2013',
        'dcsoleil14' => 'J\'ai participé au Drupalcamp Soleil 2014',
        'drupalcon-barcelona' => 'J’ai participé à la conférence Drupal 2007 (Barcelone, Espagne)',
        'drupalcon-brussels' => 'J’ai participé à la conférence Drupal 2006 (Bruxelles, Belgique)',
        'drupalcon-paris' => 'J\'ai participé à la conférence Drupal à Paris en 2009',
        'drupalcon-szeged' => 'J’ai participé à la conférence Drupal 2008 (Szeged, Hongrie)',
        'drupalcon-sa' => 'J\'ai participé à la conférence Drupal à San Francisco en 2010',
        'drupalcon-cphg' => 'J\'ai participé à la conférence Drupal à Copenhagen en 2010',
        'drupalcon-chicago' => 'J\'ai participé à la conférence Drupal à Chicago en 2011',
        'drupalcon-londre' => 'J\'ai participé à la conférence Drupal à Londres en 2011',
        'drupalcon-denver' => 'J\'ai participé à la conférence Drupal à Denver en 2012',
        'drupalcon-munich' => 'J\'ai participé à la conférence Drupal à Munich en 2012',
        'drupalcon-sydney' => 'J\'ai participé à la conférence Drupal à Sydney en 2013',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 1,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_icq'
  $field_bases['field_icq'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_icq',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 1,
    'type' => 'text',
  );

  // Exported field_base: 'field_jabber'
  $field_bases['field_jabber'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_jabber',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 1,
    'type' => 'text',
  );

  // Exported field_base: 'field_msn'
  $field_bases['field_msn'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_msn',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'email',
    'settings' => array(),
    'translatable' => 1,
    'type' => 'email',
  );

  // Exported field_base: 'field_nickname'
  $field_bases['field_nickname'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_nickname',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 1,
    'type' => 'text',
  );

  // Exported field_base: 'field_picture'
  $field_bases['field_picture'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_picture',
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => 3798,
      'uri_scheme' => 'public',
    ),
    'translatable' => 1,
    'type' => 'image',
  );

  // Exported field_base: 'field_realname'
  $field_bases['field_realname'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_realname',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 1,
    'type' => 'text',
  );

  // Exported field_base: 'field_website'
  $field_bases['field_website'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_website',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => 'nofollow',
        'target' => 'default',
      ),
      'class' => '',
      'display' => array(
        'url_cutoff' => 40,
      ),
      'enable_tokens' => 0,
      'rel' => '',
      'target' => 'default',
      'title' => 'none',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 1,
    'type' => 'link_field',
  );

  return $field_bases;
}
