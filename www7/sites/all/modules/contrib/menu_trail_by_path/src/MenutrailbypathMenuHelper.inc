<?php

class MenutrailbypathMenuHelper {
  protected $language;

  /** @var \MenutrailbypathUrlHelper */
  protected $urlHelper;

  /**
   * MenutrailbypathMenuHelper constructor.
   *
   * @param $language
   */
  public function __construct($language, MenutrailbypathUrlHelper $urlHelper) {
    $this->language  = $language;
    $this->urlHelper = $urlHelper;
  }

  /**
   * Find link items matching the given link_paths in (almost) all menus
   *
   * @param $link_paths
   * @return array
   */
  public function getMenuLinksByPaths(array $link_paths) {
    if (empty($link_paths)) {
      return array();
    }

    $query =  db_select('menu_links', 'ml')
      ->fields('ml')
      ->condition('link_path', $link_paths, 'IN')
      ->condition('hidden', 0) // Only consider normal, visible menu links.
      ->orderBy('depth')
      ->orderBy('weight')
      ->orderBy('mlid');

    if (module_exists('i18n_menu')) {
      $query->condition('language', array(LANGUAGE_NONE, $this->language->language), 'IN');
    }
    $results = $query->execute();
    return ($results instanceof DatabaseStatementInterface) ? $this->translateMenuLinkTitles($results->fetchAll()) : array();
  }

  /**
   * Get menu_links for a given menu_name
   *
   * @param $menu_name
   * @return array
   */
  public function getMenuLinks($menu_name) {
    $cid = 'menu_trail_by_path:menu_links:' . $menu_name . ':' . $this->language->language;
    $cache = cache_get($cid, 'cache_menu');
    if (!empty($cache->data)) {
      $menu_links = $cache->data;
    }

    if (!isset($menu_links)) {
      $query = db_select('menu_links', 'ml')
        ->fields('ml')
        ->condition('menu_name', $menu_name, '=') // Do not touch admin menu.
        ->condition('hidden', 0) // Only consider normal, visible menu links.
        ->orderBy('depth')
        ->orderBy('weight')
        ->orderBy('mlid');

      if (module_exists('i18n_menu')) {
        $query->condition('language', array(LANGUAGE_NONE, $this->language->language), 'IN');
      }
      $results = $query->execute();
      $menu_links = ($results instanceof DatabaseStatementInterface) ? $this->translateMenuLinkTitles($results->fetchAll()) : array();
      foreach ($menu_links as &$menu_link) {
        $menu_link->menu_path_by_trail_url = $this->urlHelper->getUrl($menu_link->link_path);
      }

      cache_set($cid, $menu_links, 'cache_menu');
    }

    return $menu_links;
  }

  /**
   * translateMenuLinkTitles
   *
   * @param array $menu_links
   * @return array
   */
  protected function translateMenuLinkTitles(array $menu_links) {
    foreach ($menu_links as $k => $menu_link) {
      if (module_exists('i18n_menu')) {
        $menu_link->link_title = _i18n_menu_link_title((array) $menu_link, $this->language->language);
      }
    }
    return $menu_links;
  }

  /**
   * Sort menu_links by the menu preference order, see menu_set_active_menu_names()
   * Stable sorting based on https://github.com/vanderlee/PHP-stable-sort-functions
   *
   * @param array $menu_links
   */
  public function sortMenuLinksByMenuPreference(array &$menu_links) {
    $menu_preference     = array_flip(menu_get_active_menu_names());
    $menu_preference_max = count($menu_preference);

    $index = 0;
    foreach ($menu_links as &$item) {
      if (!isset($menu_preference[$item->menu_name])) {
        $menu_preference[$item->menu_name] = $menu_preference_max;
      }
      $item = array($index++, $item);
    }
    $usort_result = usort($menu_links, array(new MenutrailbypathUsortMenulinks($menu_preference), 'compare'));
    foreach ($menu_links as &$item) {
      $item = $item[1];
    }
    return $usort_result;
  }

  /**
   * Make sure all custom menus are present in the active menus variable so that
   * their items may appear in the active trail.
   * @see https://www.drupal.org/node/2830385
   * @see menu_update_7003()
   *
   * @return array
   */
  public function getActiveMenuNames() {
    $active_menus = menu_get_active_menu_names();
    foreach (menu_get_names() as $menu_name) {
      if (!in_array($menu_name, $active_menus) && (strpos($menu_name, 'menu-') === 0)) {
        $active_menus[] = $menu_name;
      }
    }

    return $active_menus;
  }
}
