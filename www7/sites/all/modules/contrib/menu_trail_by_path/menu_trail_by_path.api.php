<?php
/**
 * @file
 * Hooks provided by the Menu Trail By Path module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the parent candidates before breadcrumb is set.
 *
 * @param string $path
 *   Path string. e.g. 'foo/bar/zee'.
 * @param array $parent_candidates
 *   Array of breadcrumb parent candidates.
 */
function hook_menu_trail_by_path_parent_candidates_alter($path, &$parent_candidates) {
  // Alter parent candidates here.
}

/**
 * @} End of "addtogroup hooks".
 */
