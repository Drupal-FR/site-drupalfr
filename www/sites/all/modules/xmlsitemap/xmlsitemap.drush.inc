<?php
// $Id: xmlsitemap.drush.inc,v 1.1.2.14 2010/02/16 20:00:02 davereid Exp $

/**
 * @file
 * Drush integration functions for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * Implements hook_drush_command().
 */
function xmlsitemap_drush_command() {
  $items['xmlsitemap-regenerate'] = array(
    'description' => 'Regenerate the XML sitemap files.',
    'callback' => 'drush_xmlsitemap_regenerate',
    'drupal dependencies' => array('xmlsitemap'),
  );
  $items['xmlsitemap-rebuild'] = array(
    'description' => 'Dump and re-process all possible XML sitemap data, and then regenerate the files.',
    'callback' => 'drush_xmlsitemap_rebuild',
    'drupal dependencies' => array('xmlsitemap'),
  );
  $items['xmlsitemap-index'] = array(
    'description' => 'Process un-indexed XML sitemap links.',
    'callback' => 'drush_xmlsitemap_index',
    'drupal dependencies' => array('xmlsitemap'),
    'options' => array(
      '--limit' => 'The limit of links of each type to process. Default value: ' . variable_get('xmlsitemap_batch_limit', 100),
    ),
  );
  return $items;
}

/**
 * Regenerate the sitemap files from existing data.
 */
function drush_xmlsitemap_regenerate() {
  module_load_include('inc', 'xmlsitemap', 'xmlsitemap.generate');
  xmlsitemap_regenerate();

  $vars = array(
    '@timer' => timer_read('xmlsitemap_regenerate'),
    '@memory-peak' => format_size(memory_get_peak_usage(TRUE)),
  );
  drush_print(dt('XML sitemap files regenerated in @timer ms. Peak memory usage: @memory-peak.', $vars));
}

/**
 * Dump and rebuild all the sitemap data, then regenerate the files.
 */
function drush_xmlsitemap_rebuild() {
  module_load_include('inc', 'xmlsitemap', 'xmlsitemap.generate');
  timer_start('xmlsitemap_rebuild');

  // Set the rebuild flag incase something fails during the rebuild.
  variable_set('xmlsitemap_rebuild_needed', TRUE);

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

  // Build the batch array.
  $batch = xmlsitemap_rebuild_batch(array_keys($options), $callbacks, TRUE);
  batch_set($batch);

  // We need to manually set the progressive variable again.
  // @todo Remove when http://drupal.org/node/638712 is fixed.
  $batch =& batch_get();
  $batch['progressive'] = FALSE;

  // Run the batch process.
  batch_process();

  $vars = array(
    '@timer' => timer_read('xmlsitemap_rebuild'),
    '@memory-peak' => format_size(memory_get_peak_usage(TRUE)),
  );
  drush_print(dt('XML sitemap files rebuilt in @timer ms. Peak memory usage: @memory-peak.', $vars));
}

/**
 * Process un-indexed XML sitemap links.
 */
function drush_xmlsitemap_index() {
  $limit = (int) drush_get_option('limit', variable_get('xmlsitemap_batch_limit', 100));
  $count_before = db_result(db_query("SELECT COUNT(id) FROM {xmlsitemap}"));

  module_invoke_all('xmlsitemap_index_links', $limit);
  $count_after = db_result(db_query("SELECT COUNT(id) FROM {xmlsitemap}"));

  if ($count_after == $count_before) {
    drush_print(dt('No new XML sitemap links to index.'));
  }
  else {
    drush_print(dt('Indexed @count new XML sitemap links.', array('@count' => $count_after - $count_before)));
  }
}
