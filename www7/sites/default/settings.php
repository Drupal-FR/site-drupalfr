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
 * Shared base domain
 */
$cookie_domain = '.drupal.fr';

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
// failing. Also acts as a performance improvement, avoiding one extra SQL
// query per page.
$conf['blocked_ips'] = array();

// Invoking hooks is unnecessary in our case, no modules implement them.
$conf['page_cache_invoke_hooks'] = FALSE;

$conf['maintenance_theme'] = 'dfrtheme';

/**
 * Local settings (databases password, domain configuration, ...).
 */
require dirname(__FILE__) . '/settings.local.php';

drupal_fast_404();
