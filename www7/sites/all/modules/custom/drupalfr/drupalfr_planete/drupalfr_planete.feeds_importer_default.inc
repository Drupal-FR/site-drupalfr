<?php

/**
 * @file
 * drupalfr_planete.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function drupalfr_planete_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'planet';
  $feeds_importer->config = array(
    'name' => 'Planet RSS feed',
    'description' => 'Planet RSS feed for users',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => 1,
        'use_pubsubhubbub' => 0,
        'designated_hub' => '',
        'request_timeout' => NULL,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => '17030',
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'tags',
            'target' => 'field_planete_tags',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'timestamp',
            'target' => 'created',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'url',
            'target' => 'field_planete_url:url',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'title',
            'target' => 'field_planete_url:title',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => '3',
        'authorize' => 0,
        'skip_hash_check' => 0,
        'bundle' => 'planete',
      ),
    ),
    'content_type' => 'planete_feed_source',
    'update' => 0,
    'import_period' => '3600',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['planet'] = $feeds_importer;

  return $export;
}
