<?php
/**
 * @file
 * drupalfr_planete.feeds_importer_default.inc
 */

/**
 * Implementation of hook_feeds_importer_default().
 */
function drupalfr_planete_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'drupalfr_planete';
  $feeds_importer->config = array(
    'name' => 'Planète Drupal FR',
    'description' => 'Import des blogs utilisateurs',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => 1,
        'use_pubsubhubbub' => 1,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'blog_post',
        'expire' => '-1',
        'author' => '1',
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'tags',
            'target' => 'taxonomy_vocabulary_5',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => '3',
      ),
    ),
    'content_type' => 'blog_planete',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['drupalfr_planet'] = $feeds_importer;

  return $export;
}
