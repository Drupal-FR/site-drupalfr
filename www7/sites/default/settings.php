<?php


/**
 * Database settings:
 */
$databases = array();

/**
 * Access control for update.php script.
 */
$update_free_access = FALSE;

/**
 * Salt for one-time login links and cancel links, form tokens, etc.
 */
$drupal_hash_salt = '';

/**
 * PHP settings:
 */
ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 2000000);
# ini_set('pcre.backtrack_limit', 200000);
# ini_set('pcre.recursion_limit', 200000);

// Necessary at least during the migration, to prevent Drush update-db from
// failing.
$conf['blocked_ips'] = array();

/**
 * Local settings (databases password, domain configuration, ...).
 */
require dirname(__FILE__) . '/settings.local.php';

