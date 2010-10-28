<?php
// $Id: xmlsitemap.api.php,v 1.1.2.17 2010/02/09 20:51:50 davereid Exp $

/**
 * @file
 * Hooks provided by the XML sitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide information on the type of links this module provides.
 */
function hook_xmlsitemap_link_info() {
  return array(
    'mymodule' => array(
      'label' => 'My module',
      'base table' => 'mymodule',
      'object keys' => array(
        // Primary ID key on {base table}
        'id' => 'myid',
        // Subtype key on {base table}
        'bundle' => 'mysubtype',
      ),
      'path callback' => 'mymodule_path',
      'bundle label' => t('Subtype name'),
      'bundles' => array(
        'mysubtype1' => array(
          'label' => t('My subtype 1'),
          'admin' => array(
            'real path' => 'admin/settings/mymodule/mysubtype1/edit',
            'access arguments' => array('administer mymodule'),
          ),
          'xmlsitemap' => array(
            'status' => 1,
            'priority' => 0.5,
          ),
        ),
      ),
      'xmlsitemap' => array(
        // Callback function to take an array of IDs and save them as sitemap
        // links.
        'process callback' => '',
        // Callback function used in batch API for rebuilding all links.
        'rebuild callback' => '',
        // Callback function called from the XML sitemap settings page.
        'settings callback' => '',
      )
    ),
  );
}

/**
 * Alter the data of a sitemap link before the link is saved.
 *
 * @param $link
 *   An array with the data of the sitemap link.
 */
function hook_xmlsitemap_link_alter(&$link) {
  if ($link['type'] == 'mymodule') {
    $link['priority'] += 0.5;
  }
}

/**
 * Alter the query selecting data from {xmlsitemap} during sitemap generation.
 *
 * Do not alter LIMIT or OFFSET as the query will be passed through
 * db_query_range() with a set limit and offset.
 *
 * @param $query
 *   An array of a query object, keyed by SQL keyword (SELECT, FROM, WHERE, etc).
 * @param $args
 *   An array of arguments to be passed to db_query() with $query.
 * @param $context
 *   An array of sitemap context being used for generation.
 */
function hook_xmlsitemap_query_alter(array &$query, array &$args, array $context) {
  if (isset($context['language'])) {
    $query['WHERE'] .= " AND x.language = '%s'";
    $args[] = $context['language'];
  }
}

/**
 * @} End of "addtogroup hooks".
 */
