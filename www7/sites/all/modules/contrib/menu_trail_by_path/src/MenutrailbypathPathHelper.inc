<?php

class MenutrailbypathPathHelper {
  /**
   * Get current trail paths, formally known as "parent candidates"
   *
   * e.g. given the request_path() 'foo/bar/zee', this returns an array of
   * internal Drupal paths for 'foo', 'foo/bar', 'foo/bar/zee'.
   *
   * @return array
   *   An array of internal Drupal paths.
   */
  public function getPaths() {
    static $paths;

    if (!isset($paths)) {
      if (drupal_is_front_page()) {
        $paths = array('<front>');
        return $paths;
      }

      include_once DRUPAL_ROOT . '/includes/language.inc';
      list($request_language, $request_path) = language_url_split_prefix(request_path(), language_list());

      $paths         = array();
      $path_elements = explode('/', $request_path);
      while (count($path_elements)) {
        $path = drupal_get_normal_path(implode('/', $path_elements));
        if ($router_item = menu_get_item($path)) {
          $paths[] = $path;
        }
        array_pop($path_elements);
      }
      $paths = array_reverse($paths);

      // Allow other modules to alter the paths (formerly known as "parent candidates").
      drupal_alter('menu_trail_by_path_parent_candidates', $request_path, $paths);
    }

    return $paths;
  }
}
