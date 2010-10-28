<?php
// $Id: xmlsitemap.generate.inc,v 1.1.2.8 2010/03/05 14:07:54 davereid Exp $

/**
 * @file
 * Sitemap generation and rebuilding functions for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * Given an internal Drupal path, return the alias for the path.
 *
 * This is similar to drupal_get_path_alias(), but designed to fetch all alises
 * at once so that only one database query is executed instead of several or
 * possibly thousands during sitemap generation.
 *
 * @param $path
 *   An internal Drupal path.
 * @param $language
 *   A language code to look use when looking up the paths.
 */
function xmlsitemap_get_path_alias($path, $language) {
  static $aliases;
  static $last_language;

  if (!isset($aliases)) {
    $aliases['all'] = array();
    $query = db_query("SELECT src, dst FROM {url_alias} WHERE language = '' ORDER BY pid");
    while ($alias = db_fetch_array($query)) {
      $aliases['all'][$alias['src']] = $alias['dst'];
    }
  }
  if ($language && $last_language != $language) {
    unset($aliases[$last_language]);
    $aliases[$language] = array();
    $query = db_query("SELECT src, dst FROM {url_alias} WHERE language = '%s' ORDER BY pid", $language);
    while ($alias = db_fetch_array($query)) {
      $aliases[$language][$alias['src']] = $alias['dst'];
    }
    $last_language = $language;
  }

  if ($language && isset($aliases[$language][$path])) {
    return $aliases[$language][$path];
  }
  elseif (isset($aliases['all'][$path])) {
    return $aliases['all'][$path];
  }
  else {
    return $path;
  }
}

function xmlsitemap_get_url_options_from_context(array $context) {
  $options = module_invoke_all('xmlsitemap_context_url_options', $context);
  drupal_alter('xmlsitemap_context_url_options', $options, $context);
  return $options;
}

/**
 * @todo Replace with table of sitemap objects.
 */
function xmlsitemap_get_contexts() {
  $contexts = array();

  if (module_exists('xmlsitemap_i18n')) {
    foreach (xmlsitemap_var('languages') as $langcode) {
      $contexts[] = array('language' => $langcode);
    }
  }
  else {
    // Add an empty, default context.
    $contexts[] = array();
  }

  $hashes = array_map('md5', array_map('serialize', $contexts));
  $contexts = array_combine($hashes, $contexts);

  return $contexts;
}

/**
 * Delete and regenerate the sitemap files.
 */
function xmlsitemap_regenerate() {
  _xmlsitemap_regenerate_before();

  // Generate the sitemap pages.
  $contexts = xmlsitemap_get_contexts();
  $chunk_count = xmlsitemap_get_chunk_count(TRUE);
  foreach ($contexts as $context) {
    // Generate index.
    if ($chunk_count > 1) {
      xmlsitemap_generate('index', $context);
    }
    for ($i = 1; $i <= $chunk_count; $i++) {
      xmlsitemap_generate($i, $context);
    }
  }

  _xmlsitemap_regenerate_after();
}

/**
 * Perform operations before rebuilding the sitemap.
 */
function _xmlsitemap_regenerate_before() {
  // Attempt to increase the available processing time and memory limit.
  @set_time_limit(240);
  _xmlsitemap_set_memory_limit();

  // Set a timer so we can track how long this takes.
  timer_start('xmlsitemap_regenerate');

  // Get the current memory usage so we can track how much memory is used.
  _xmlsitemap_get_memory_usage(TRUE);

  // Clear all cached sitemap files.
  xmlsitemap_clear_directory();
  xmlsitemap_check_directory();

  // Clear the maximum chunk and file size variables.
  variable_set('xmlsitemap_max_chunks', 0);
  variable_set('xmlsitemap_max_filesize', 0);
}

function _xmlsitemap_get_memory_usage($start = FALSE) {
  static $memory_start;
  $current = 0;
  if (function_exists('memory_get_peak_usage')) {
    $current = memory_get_peak_usage(TRUE);
  }
  if (function_exists('memory_get_usage')) {
    $current = version_compare(PHP_VERSION, '5.2') ? memory_get_usage(TRUE) : memory_get_usage();
  }
  if (!isset($memory_start) || $start) {
    $memory_start = $current;
  }
  return $current - $memory_start;
}

function _xmlsitemap_get_optimal_memory_limit() {
  static $optimal_limit;
  if (!isset($optimal_limit)) {
    // Set the base memory amount from the provided core constant.
    $optimal_limit = parse_size(DRUPAL_MINIMUM_PHP_MEMORY_LIMIT);

    // Add memory based on the chunk size.
    $optimal_limit += xmlsitemap_get_chunk_size() * 500;

    // Add memory for storing the url aliases.
    $aliases = db_result(db_query("SELECT COUNT(pid) FROM {url_alias}"));
    $optimal_limit += $aliases * 250;
  }
  return $optimal_limit;
}

/**
 * Calculate the optimal memory level for sitemap generation.
 */
function _xmlsitemap_set_memory_limit() {
  $memory_limit = @ini_get('memory_limit');
  if ($memory_limit && $memory_limit != -1) {
    $optimal_limit = _xmlsitemap_get_optimal_memory_limit();
    if (parse_size($memory_limit) < $optimal_limit) {
      @ini_set('memory_limit', $optimal_limit);
    }
  }
}

/**
 * Perform operations after rebuilding the sitemap.
 */
function _xmlsitemap_regenerate_after() {
  // Show a watchdog message that the sitemap was regenerated.
  watchdog('xmlsitemap',
    'XML sitemap files regenerated in @timer ms. Peak memory usage: @memory-peak.',
    array(
      '@timer' => timer_read('xmlsitemap_regenerate'),
      '@memory-peak' => format_size(memory_get_peak_usage(TRUE)),
    ),
    WATCHDOG_NOTICE,
    l(t('View sitemap'), 'sitemap.xml')
  );

  // Unset the regenerate flag.
  variable_set('xmlsitemap_regenerate_needed', FALSE);

  variable_set('xmlsitemap_generated_last', REQUEST_TIME);
}

/**
 * Fetch the data from {xmlsitemap}, generates the sitemap, then caches it.
 *
 * @param $chunk
 *   An integer representing the integer of the sitemap page chunk.
 * @param $language
 *   A language object, defaults to the default language.
 * @return
 *   TRUE on success; otherwise FALSE
 *
 * @todo Revise/simplify or remove the function.
 */
function xmlsitemap_generate($chunk, array $context) {
  if (($chunk == 'index' && xmlsitemap_get_chunk_count() < 2) || ($chunk != 'index' && (!is_numeric($chunk) || $chunk > xmlsitemap_get_chunk_count()))) {
    // Don't bother translating this string.
    trigger_error('Improper condition hit in xmlsitemap_generate(). Chunk: ' . $chunk . ', Chunk Count: ' . xmlsitemap_get_chunk_count());
    return FALSE;
  }

  $file = xmlsitemap_get_file_from_context($context, $chunk);

  if (!$handle = fopen($file, 'wb')) {
    trigger_error(t('Could not open file @file for writing.', array('@file' => $file)));
    return FALSE;
  }

  $status = TRUE;
  if ($chunk == 'index') {
    xmlsitemap_generate_index($handle, $status, $context);
  }
  else {
    xmlsitemap_generate_chunk($handle, $status, $chunk, $context);
  }
  fclose($handle);

  // Track the maximum filesize.
  $filesize = filesize($file);
  if ($filesize > variable_get('xmlsitemap_max_filesize', 0)) {
    variable_set('xmlsitemap_max_filesize', $filesize);
  }

  if (!$status) {
    trigger_error(t('Unknown error occurred while writing to file @file.', array('@file' => $file)));
  }
  elseif (xmlsitemap_var('gz')) {
    $file_gz = $file . '.gz';
    file_put_contents($file_gz, gzencode(file_get_contents($file), 9));
  }

  return $status;
}

/**
 * Write the proper XML sitemap header.
 *
 * @param $type
 * @param $handle
 *   A file system pointer resource that is typically created using fopen().
 * @param $status
 * @param $language
 */
function xmlsitemap_generate_chunk_header($type, $handle, &$status, array $context) {
  $output = '<?xml version="1.0" encoding="UTF-8"?>' . PHP_EOL;
  $url_options = xmlsitemap_get_url_options_from_context($context);

  // Add the stylesheet link.
  if (xmlsitemap_var('xsl')) {
    $xsl_url = url('sitemap.xsl', $url_options);
    $output .= '<?xml-stylesheet type="text/xsl" href="' . $xsl_url . '"?>' . PHP_EOL;
  }

  $output .= '<' . $type . ' xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">' . PHP_EOL;

  // This is the full XML header required for schema validation.
  //$schemas = array('sitemapindex' => 'siteindex.xsd', 'urlset' => 'sitemap.xsd');
  //$output .= '<' . $type . ' xmlns="http://www.sitemaps.org/schemas/sitemap/0.9"' . PHP_EOL;
  //$output .= '  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"' . PHP_EOL;
  //$output .= '  xsi:schemaLocation="http://www.sitemaps.org/schemas/sitemap/0.9' . PHP_EOL;
  //$output .= '  http://www.sitemaps.org/schemas/sitemap/0.9/' . $schemas[$type] . '">' . PHP_EOL;

  $status &= (bool) fwrite($handle, $output);
  return $status;
}

/**
 * Generate one page (chunk) of the sitemap.
 *
 * @param $handle
 *   A file system pointer resource that is typically created using fopen().
 * @param $status
 *   A boolean that will be altered by reference with the success status of
 *   writing to $handle.
 * @param $chunk
 *   An integer representing the integer of the sitemap page chunk.
 * @param $language
 *   A language object for the sitemap chunk.
 */
function xmlsitemap_generate_chunk($handle, &$status, $chunk, array $context) {
  $url_options = xmlsitemap_get_url_options_from_context($context);
  $url_options += array(
    'absolute' => TRUE,
    'base_url' => xmlsitemap_var('base_url'),
    'alias' => TRUE,
    'language' => language_default(),
  );

  $last_url = '';
  $lastmod_format = variable_get('xmlsitemap_lastmod_format', 'Y-m-d\TH:i\Z');

  $query = array(
    'SELECT'   => 'SELECT x.loc, x.lastmod, x.changefreq, x.changecount, x.priority, x.language',
    'FROM'     => 'FROM {xmlsitemap} x',
    'WHERE'    => 'WHERE x.access = 1 AND x.status = 1',
    'ORDER BY' => 'ORDER BY x.language, x.loc',
  );
  $args = array();

  // Allow other modules to alter the sitemap query SQL and arguments.
  static $alter;
  if (!isset($alter)) {
    // Skip altering if there are no modules to invoke.
    $alter = (bool) module_implements('xmlsitemap_query_alter');
  }
  if ($alter) {
    $data = &$query;
    $data['__drupal_alter_by_ref'] = array(&$args);
    drupal_alter('xmlsitemap_query', $data, $context);
  }

  $sql = implode($query, ' ');
  $offset = max($chunk - 1, 0) * xmlsitemap_get_chunk_size();
  $limit = xmlsitemap_get_chunk_size();
  $query = db_query_range($sql, $args, $offset, $limit);

  // Add the XML header and XSL if desired.
  xmlsitemap_generate_chunk_header('urlset', $handle, $status, $context);

  while ($link = db_fetch_array($query)) {
    $link['language'] = $link['language'] ? xmlsitemap_language_load($link['language']) : $url_options['language'];
    $link['alias'] = xmlsitemap_get_path_alias($link['loc'], $link['language']->language);
    $link_url = url($link['alias'], array('language' => $link['language']) + $url_options);

    // Skip this link if it was a duplicate of the last one.
    // @todo Figure out a way to do this before generation so we can report
    // back to the user about this.
    if ($link_url == $last_url) {
      continue;
    }
    else {
      $last_url = $link_url;
    }

    $link_output = '<url><loc>' . $link_url . '</loc>';
    if ($link['lastmod']) {
      $link_output .= '<lastmod>' . gmdate($lastmod_format, $link['lastmod']) . '</lastmod>';
      // If the link has a lastmod value, update the changefreq so that links
      // with a short changefreq but updated two years ago show decay.
      // We use abs() here just incase items were created on this same cron run
      // because lastmod would be greater than REQUEST_TIME.
      $link['changefreq'] = (abs(REQUEST_TIME - $link['lastmod']) + $link['changefreq']) / 2;
    }
    if ($link['changefreq']) {
      $link_output .= '<changefreq>' . xmlsitemap_get_changefreq($link['changefreq']) . '</changefreq>';
    }
    if (isset($link['priority']) && $link['priority'] != 0.5) {
      // Don't output the priority value for links that have 0.5 priority. This
      // is the default 'assumed' value if priority is not included as per the
      // sitemaps.org specification.
      $link_output .= '<priority>' . number_format($link['priority'], 1) . '</priority>';
    }
    $link_output .= '</url>' . PHP_EOL;
    $status &= (bool) fwrite($handle, $link_output);
  }

  // Close the XML file.
  $status &= (bool) fwrite($handle, '</urlset>' . PHP_EOL);

  return $status;
}

/**
 * Generate the index sitemap.
 *
 * @param $handle
 *   A file system pointer resource that is typically created using fopen().
 * @param $status
 * @param $language
 *   A language object, defaults to the default language.
 */
function xmlsitemap_generate_index($handle, &$status, $context) {
  $url_options = xmlsitemap_get_url_options_from_context($context);
  $url_options += array(
    'absolute' => TRUE,
    'base_url' => xmlsitemap_var('base_url'),
    'alias' => TRUE,
    'language' => language_default(),
  );

  $chunk_count = xmlsitemap_get_chunk_count(TRUE);
  $lastmod_format = variable_get('xmlsitemap_lastmod_format', 'Y-m-d\TH:i\Z');

  // Add the XML header and XSL if desired.
  xmlsitemap_generate_chunk_header('sitemapindex', $handle, $status, $context);

  for ($i = 1; $i <= $chunk_count; $i++) {
    $output = '<sitemap>';
    $url_options['query']['page'] = $i;
    $output .= '<loc>' . url('sitemap.xml', $url_options) . '</loc>';
    // @todo Use the actual lastmod value of the chunk file.
    $output .= '<lastmod>' . gmdate($lastmod_format, REQUEST_TIME) . '</lastmod>';
    $output .= '</sitemap>' . PHP_EOL;
    $status &= (bool) fwrite($handle, $output);
  }

  // Close the XML file.
  $status &= (bool) fwrite($handle, '</sitemapindex>' . PHP_EOL);

  return $status;
}

/**
 * Batch information callback.
 */
function xmlsitemap_rebuild_batch(array $entities, array $callbacks, $save_custom = FALSE) {
  $batch = array(
    'operations' => array(),
    'finished' => 'xmlsitemap_rebuild_batch_finished',
    'title' => t('Rebuilding Sitemap'),
    'file' => drupal_get_path('module', 'xmlsitemap') . '/xmlsitemap.generate.inc',
  );

  // Purge any links first.
  $batch['operations'][] = array('xmlsitemap_rebuild_batch_clear', array($entities, (bool) $save_custom));

  // Fetch all the sitemap links and save them to the {xmlsitemap} table.
  foreach ($entities as $entity) {
    $batch['operations'][] = array($callbacks[$entity], array($entity));
  }

  // Generate all the sitemap pages for each context.
  $batch['operations'][] = array('_xmlsitemap_regenerate_before', array());
  $contexts = xmlsitemap_get_contexts();
  foreach ($contexts as $context) {
    $batch['operations'][] = array('xmlsitemap_rebuild_batch_generate', array($context));
  }
  $batch['operations'][] = array('_xmlsitemap_regenerate_after', array());

  return $batch;
}

/**
 * Batch callback; clear sitemap links for entites.
 */
function xmlsitemap_rebuild_batch_clear(array $entities, $save_custom, &$context) {
  if (!empty($entities)) {
    $sql = "DELETE FROM {xmlsitemap} WHERE type IN (" . db_placeholders($entities, 'varchar') . ')';

    // If we want to save the custom data, make sure to exclude any links
    // that are not using default inclusion or priority.
    if ($save_custom) {
      $sql .= ' AND status_override = 0 AND priority_override = 0';
    }

    db_query($sql, $entities);
  }

  $context['message'] = t('Purging links.');
}

/**
 * Batch callback; fetch and add the sitemap links for a specific entity.
 */
 function xmlsitemap_rebuild_batch_fetch($entity, &$context) {
  if (!isset($context['sandbox']['info'])) {
    $context['sandbox']['info'] = xmlsitemap_get_link_info($entity);
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['last_id'] = 0;
  }
  $info = $context['sandbox']['info'];

  // Build the generic query.
  $base_table = db_escape_table($info['base table']);
  $id_key = db_escape_string($info['object keys']['id']);
  $query = $args = $ids = array();
  $query['SELECT'] = "SELECT $id_key";
  $query['FROM']   = "FROM {{$base_table}}";
  $query['WHERE']  = "WHERE $id_key > %d";
  $args[] = $context['sandbox']['last_id'];

  if (!empty($info['object keys']['bundle'])) {
    $bundle_key = db_escape_string($info['object keys']['bundle']);
    $bundle_type = _xmlsitemap_get_field_type($info['base table'], $info['object keys']['bundle']);
    $bundles = xmlsitemap_get_link_type_enabled_bundles($entity);
    $query['WHERE'] .= " AND $bundle_key IN (" . db_placeholders($bundles, $bundle_type) . ")";
    $args = array_merge($args, $bundles);
  }

  if (!isset($context['sandbox']['max'])) {
    $count_query = $query;
    $count_query['SELECT'] = "SELECT COUNT($id_key)";
    $sql = implode(' ', $count_query);
    $context['sandbox']['max'] = (int) db_result(db_query($sql, $args));
    if (!$context['sandbox']['max']) {
      // If there are no items to process, skip everything else.
      return;
    }
  }

  // PostgreSQL cannot have the ORDERED BY in the count query.
  $query['ORDER BY'] = "ORDER BY $id_key";

  $sql = implode(' ', $query);
  $query = db_query_range($sql, $args, 0, variable_get('xmlsitemap_batch_limit', 100));
  while ($id = db_result($query)) {
    $ids[] = $id;
  }
  $info['xmlsitemap']['process callback']($ids);
  $context['sandbox']['last_id'] = end($ids);
  $context['sandbox']['progress'] += count($ids);
  $context['message'] = t('Now processing %entity @last_id (@progress of @count).', array('%entity' => $entity, '@last_id' => $context['sandbox']['last_id'], '@progress' => $context['sandbox']['progress'], '@count' => $context['sandbox']['max']));

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch callback; generate the sitemap chunks for a context.
 */
function xmlsitemap_rebuild_batch_generate(array $sitemap_context, array &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 1;
    $context['sandbox']['max'] = xmlsitemap_get_chunk_count(TRUE);
    if ($context['sandbox']['max'] > 1) {
      xmlsitemap_generate('index', $sitemap_context);
    }
    // Bump the max number up by one since it gets incremented after generation.
    $context['sandbox']['max']++;
  }

  xmlsitemap_generate($context['sandbox']['progress'], $sitemap_context);
  $context['message'] = t('Now generating sitemap page @chunk.', array('@chunk' => $context['sandbox']['progress']));
  $context['sandbox']['progress']++;

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch callback; sitemap rebuild finished.
 */
function xmlsitemap_rebuild_batch_finished($success, $results, $operations) {
  if ($success) {
    // Reset the rebuild flag since it was successful.
    variable_set('xmlsitemap_rebuild_needed', FALSE);
    drupal_set_message(t('The sitemap was rebuilt.'));
  }
  else {
    drupal_set_message(t('The sitemap was not successfully rebuilt.'), 'error');
  }
}
