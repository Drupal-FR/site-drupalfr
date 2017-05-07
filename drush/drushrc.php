<?php

/**
 * List of tables whose *data* is skipped by the 'sql-dump' and 'sql-sync'
 * commands when the "--structure-tables-key=common" option is provided.
 * You may add specific tables to the existing array or add a new element.
 */
$options['structure-tables']['common'] = [
  'cache',
  'cache_*',
  'history',
  'search_*',
  'sessions',
  'watchdog',
  'webprofiler',
];
