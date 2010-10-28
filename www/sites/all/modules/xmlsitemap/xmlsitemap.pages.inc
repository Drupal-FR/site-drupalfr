<?php
// $Id: xmlsitemap.pages.inc,v 1.1.2.41 2010/03/02 18:18:07 davereid Exp $

/**
 * @file
 * Page callbacks for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * Get the sitemap context of the current request.
 */
function xmlsitemap_get_current_context() {
  static $context;

  if (!isset($context)) {
    $context = module_invoke_all('xmlsitemap_context');
    drupal_alter('xmlsitemap_context', $context);
    asort($context);
  }

  return $context;
}

/**
 * Validate the context and use the default context if it fails validation.
 *
 * @todo Merge into xmlsitemap_get_current_context()?
 * @todo Use real default context variable instead of hard-coded default.
 */
function xmlsitemap_context_check(array $context) {
  $hash = md5(serialize($context));

  $default = array();
  if (module_exists('xmlsitemap_i18n')) {
    $default['language'] = language_default('language');
  }
  $default_hash = md5(serialize($default));

  module_load_include('inc', 'xmlsitemap', 'xmlsitemap.generate');
  $valid_contexts = array_keys(xmlsitemap_get_contexts());

  if (in_array($hash, $valid_contexts)) {
    return $context;
  }
  elseif (in_array($default_hash, $valid_contexts)) {
    return $default;
  }
  else {
    trigger_error("Could not find fallback XML sitemap context. Context $hash: " . print_r($context, TRUE) . ". Default context $default_hash: " . print_r($default, TRUE), E_USER_ERROR);
    return array();
  }
}

/**
 * Get the sitemap chunk/page of the current request.
 */
function xmlsitemap_get_current_chunk(array $context) {
  // Check if we should be displaing the index.
  if (!isset($_GET['page']) || !is_numeric($_GET['page'])) {
    $index_file = xmlsitemap_get_file_from_context($context, 'index');
    if (file_exists($index_file)) {
      return 'index';
    }
    else {
      return 1;
    }
  }
  else {
    return (int) $_GET['page'];
  }
}

/**
 * Output a sitemap page.
 *
 * @see xmlsitemap_get_current_context()
 * @see xmlsitemap_get_current_chunk()
 * @see xmlsitemap_get_file_from_context()
 * @see xmlsitemap_output_file()
 */
function xmlsitemap_output_chunk() {
  $original_context = xmlsitemap_get_current_context();
  $context = xmlsitemap_context_check($original_context);
  $chunk = xmlsitemap_get_current_chunk($context);
  $file = xmlsitemap_get_file_from_context($context, $chunk);

  // Provide debugging information if enabled.
  if (variable_get('xmlsitemap_developer_mode', 0) && isset($_GET['debug'])) {
    $output = array();
    $output[] = "Chunk: $chunk";
    $output[] = "Original context: " . print_r($original_context, TRUE);
    $output[] = "Context: " . print_r($context, TRUE);
    $output[] = "Cache file location: $file";
    $output[] = "Cache file exists: " . (file_exists($file) ? 'Yes' : 'No');
    return implode('<br />', $output);
  }

  return xmlsitemap_output_file($file);
}

/**
 * Output the contents of a file to the browser and check caching headers.
 */
function xmlsitemap_output_file($file, array $headers = array()) {
  if (!file_exists($file) || !is_readable($file)) {
    return drupal_not_found();
  }

  $mtime = filemtime($file);
  $last_modified = gmdate(DATE_RFC1123, $mtime);
  $etag = '"' . md5($last_modified) . '"';

  // See if the client has provided the required HTTP headers.
  $if_modified_since = isset($_SERVER['HTTP_IF_MODIFIED_SINCE']) ? stripslashes($_SERVER['HTTP_IF_MODIFIED_SINCE']) : FALSE;
  $if_none_match = isset($_SERVER['HTTP_IF_NONE_MATCH']) ? stripslashes($_SERVER['HTTP_IF_NONE_MATCH']) : FALSE;
  if ($if_modified_since && $if_none_match && $if_none_match == $etag && $if_modified_since == $last_modified) {
    header('HTTP/1.1 304 Not Modified');
    // All 304 responses must send an etag if the 200 response for the same object contained an etag
    header('Etag: ' . $etag);
    exit;
  }

  $headers += array(
    'Content-type: text/xml; charset=utf-8',
    //'Content-length: ' . filesize($file),
    'Last-modified: ' . $last_modified,
    'Etag: ' . $etag,
    'Expires: ' . gmdate(DATE_RFC1123, $mtime + variable_get('xmlsitemap_minimum_lifetime', 0)),
    'Cache-Control: must-revalidate',
    'X-Robots-Tag: noindex, follow',
  );

  // Transfer the file as output.
  xmlsitemap_file_transfer($file, $headers);
}

/**
 * Modified version of file_transfer() that invokes hook_exit()s afterwards.
 *
 * @see file_transfer()
 */
function xmlsitemap_file_transfer($source, $headers) {
  if (ob_get_level()) {
    ob_end_clean();
  }

  foreach ($headers as $header) {
    drupal_set_header($header);
  }

  // Transfer file in 16 KB chunks to save memory usage.
  if ($handle = fopen($source, 'rb')) {
    while (!feof($handle)) {
      print fread($handle, 1024*16);
    }
    fclose($handle);
  }
  else {
    drupal_not_found();
  }

  module_invoke_all('exit');
  exit();
}

/**
 * Output an XML transformation file for the sitemap XML.
 */
function xmlsitemap_output_xsl() {
  // Read the XSL content from the file.
  $module_path = drupal_get_path('module', 'xmlsitemap');
  $xsl_content = file_get_contents($module_path . '/xsl/xmlsitemap.xsl');

  // Make sure the strings in the XSL content are translated properly.
  $replacements = array(
    'Sitemap file' => t('Sitemap file'),
    'Generated by the <a href="http://drupal.org/project/xmlsitemap">Drupal XML sitemap module</a>.' => t('Generated by the <a href="@link-xmlsitemap">Drupal XML sitemap module</a>.', array('@link-xmlsitemap' => 'http://drupal.org/project/xmlsitemap')),
    'Number of sitemaps in this index' => t('Number of sitemaps in this index'),
    'Click on the table headers to change sorting.' => t('Click on the table headers to change sorting.'),
    'Sitemap URL' => t('Sitemap URL'),
    'Last modification date' => t('Last modification date'),
    'Number of URLs in this sitemap' => t('Number of URLs in this sitemap'),
    'URL location' => t('URL location'),
    'Change frequency' => t('Change frequency'),
    'Priority' => t('Priority'),
    '[jquery]' => base_path() . 'misc/jquery.js',
    '[jquery-tablesort]' => base_path() . $module_path . '/xsl/jquery.tablesorter.min.js',
    '[xsl-js]' => base_path() . $module_path . '/xsl/xmlsitemap.xsl.js',
    '[xsl-css]' => base_path() . $module_path . '/xsl/xmlsitemap.xsl.css',
  );
  $xsl_content = strtr($xsl_content, $replacements);

  // Output the XSL content.
  drupal_set_header('Content-type: application/xml; charset=utf-8');
  drupal_set_header('X-Robots-Tag: noindex, follow');
  print $xsl_content;
}
