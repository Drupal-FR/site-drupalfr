<?php

class MenutrailbypathActivetrail {
  protected $pathHelper;
  protected $menuHelper;
  protected $urlHelper;

  /**
   * MenutrailbypathActivetrail constructor.
   *
   * @param \MenutrailbypathPathHelper $pathHelper
   * @param \MenutrailbypathMenuHelper $menuHelper
   */
  public function __construct(MenutrailbypathPathHelper $pathHelper, MenutrailbypathMenuHelper $menuHelper, MenutrailbypathUrlHelper $urlHelper) {
    $this->pathHelper = $pathHelper;
    $this->menuHelper = $menuHelper;
    $this->urlHelper  = $urlHelper;
  }

  /**
   * Set the active trail for all menu's
   */
  public function setActivetrails() {
    $menu_names = $this->menuHelper->getActiveMenuNames();
    foreach ($menu_names as $menu_name) {
      $this->setActivetrail($menu_name);
    }
  }

  /**
   * Set the active trail for the specified menu
   *
   * @param $menu_name
   */
  public function setActivetrail($menu_name) {
    // The only way to override the default preferred menu link for a path is to
    // inject it into the static cache of the function menu_link_get_preferred().
    $preferred_links = &drupal_static('menu_link_get_preferred');
    $path            = $_GET['q'];

    // If the regular menu_link_get_preferred isn't called yet, we need to call it get a
    // clean menu_link_get_preferred static cache (and thus avoiding any unpredictable behaviours)
    if (!isset($preferred_links[$path][MENU_PREFERRED_LINK])) {
      menu_link_get_preferred();
    }

    if ($menu_link = $this->getActiveTrailLink($menu_name)) {
      $query = db_select('menu_links', 'ml', array('fetch' => PDO::FETCH_ASSOC));
      $query->leftJoin('menu_router', 'm', 'm.path = ml.router_path');
      $query->fields('ml');
      // Weight must be taken from {menu_links}, not {menu_router}.
      $query->addField('ml', 'weight', 'link_weight');
      $query->fields('m');
      $query->condition('ml.mlid', $menu_link->mlid, '=');

      $candidate_item = $query->execute()->fetchAssoc();
      $candidate_item['weight'] = $candidate_item['link_weight'];

      // TODO: The menu_links doesn't always have a 1:1 relation to the a menu_router, is it ok to skip the _menu_translate?
      if (!empty($candidate_item['path'])) {
        $map = explode('/', $path);
        _menu_translate($candidate_item, $map);
      }

      $preferred_links[$path][$menu_name] = $candidate_item;
    }
  }

  /**
   * Fetches the deepest, heaviest menu link which matches the deepest trail path url.
   *
   * @param string $menu_name
   *   The menu within which to find the active trail link.
   *
   * @return stdClass|NULL
   *   The menu link for the given menu, or NULL if there is no matching menu link.
   */
  protected function getActiveTrailLink($menu_name) {
    $menu_links  = array_reverse($this->menuHelper->getMenuLinks($menu_name));
    $trail_paths = array_reverse($this->pathHelper->getPaths());

    foreach ($trail_paths as $trail_path) {
      $trail_path_url = $this->urlHelper->getUrl($trail_path);
      foreach ($menu_links as $menu_link) {
        if ($menu_link->menu_path_by_trail_url == $trail_path_url) {
          return $menu_link;
        }
      }
    }

    return NULL;
  }
}
