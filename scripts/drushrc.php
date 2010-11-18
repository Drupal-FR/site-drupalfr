<?php

/*
 * Customize this associative array with your own tables. This is the list of
 * tables whose *data* is skipped by the 'sql-dump' and 'sql-sync' commands when
 * a structure-tables-key is provided. You may add new tables to the existing
 * array or add a new element.
 */
$options['structure-tables'] = array(
 'sanitize' => array(
    'cache', 'cache_admin_menu', 'cache_apachesolr',
    'cache_block', 'cache_content', 'cache_filter', 'cache_form',
    'cache_menu', 'cache_mollom', 'cache_page', 'cache_rules',
    'cache_update', 'cache_views', 'cache_views_data', 'devel_queries',
    'devel_times', 'flood', 'history', 'search_dataset',
    'search_index', 'search_total', 'search_node_links', 'sessions',
    'watchdog', 'old_revisions', 'access', 'apachesolr_search_node',
    'semaphore'
  ),
);

