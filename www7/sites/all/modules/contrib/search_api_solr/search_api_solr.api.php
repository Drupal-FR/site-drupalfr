<?php

/**
 * @file
 * Hooks provided by the Search API Solr search module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Lets modules alter a Solr search request before sending it.
 *
 * Apache_Solr_Service::search() is called afterwards with these parameters.
 * Please see this method for details on what should be altered where and what
 * is set afterwards.
 *
 * @param array $call_args
 *   An associative array containing all three arguments to the
 *   SearchApiSolrConnectionInterface::search() call ("query", "params" and
 *   "method") as references.
 * @param SearchApiQueryInterface $query
 *   The SearchApiQueryInterface object representing the executed search query.
 */
function hook_search_api_solr_query_alter(array &$call_args, SearchApiQueryInterface $query) {
  if ($query->getOption('foobar')) {
    $call_args['params']['foo'] = 'bar';
  }
}

/**
 * Change the way the index's field names are mapped to Solr field names.
 *
 * @param SearchApiIndex $index
 *   The index whose field mappings are altered.
 * @param array $fields
 *   An associative array containing the index field names mapped to their Solr
 *   counterparts. The special fields 'search_api_id' and 'search_api_relevance'
 *   are also included.
 */
function hook_search_api_solr_field_mapping_alter(SearchApiIndex $index, array &$fields) {
  if ($index->entity_type == 'node' && isset($fields['body:value'])) {
    $fields['body:value'] = 'text';
  }
}

/**
 * Alter Solr documents before they are sent to Solr for indexing.
 *
 * @param array $documents
 *   An array of SearchApiSolrDocument objects ready to be indexed, generated
 *   from $items array.
 * @param SearchApiIndex $index
 *   The search index for which items are being indexed.
 * @param array $items
 *   An array of items being indexed.
 */
function hook_search_api_solr_documents_alter(array &$documents, SearchApiIndex $index, array $items) {
  // Adds a "foo" field with value "bar" to all documents.
  foreach ($documents as $document) {
    $document->setField('foo', 'bar');
  }
}

/**
 * Lets modules alter the search results returned from a Solr search.
 *
 * @param array $results
 *   The results array that will be returned for the search.
 * @param SearchApiQueryInterface $query
 *   The SearchApiQueryInterface object representing the executed search query.
 * @param object $response
 *   The Solr response object.
 */
function hook_search_api_solr_search_results_alter(array &$results, SearchApiQueryInterface $query, $response) {
  if (isset($response->facet_counts->facet_fields->custom_field)) {
    // Do something with $results.
  }
}

/**
 * Lets modules alter a Solr search request for a multi-index search.
 *
 * SearchApiSolrConnectionInterface::search() is called afterwards with these
 * parameters. Please see this method for details on what should be altered
 * where and what is set afterwards.
 *
 * @param array $call_args
 *   An associative array containing all three arguments to the
 *   SearchApiSolrConnectionInterface::search() call ("query", "params" and
 *   "method") as references.
 * @param SearchApiMultiQueryInterface $query
 *   The object representing the executed search query.
 */
function hook_search_api_solr_multi_query_alter(array &$call_args, SearchApiMultiQueryInterface $query) {
  if ($query->getOption('foobar')) {
    $call_args['params']['foo'] = 'bar';
  }
}

/**
 * @} End of "addtogroup hooks".
 */
