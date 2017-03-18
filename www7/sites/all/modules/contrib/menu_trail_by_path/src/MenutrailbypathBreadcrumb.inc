<?php

class MenutrailbypathBreadcrumb {
  protected $pathHelper;
  protected $menuHelper;

  /**
   * MenutrailbypathBreadcrumb constructor.
   *
   * @param \MenutrailbypathPathHelper $pathHelper
   * @param \MenutrailbypathMenuHelper $menuHelper
   */
  public function __construct(MenutrailbypathPathHelper $pathHelper, MenutrailbypathMenuHelper $menuHelper) {
    $this->pathHelper = $pathHelper;
    $this->menuHelper = $menuHelper;
  }

  /**
   * Sets the breadcrumb by path
   *
   * @see menu_get_active_breadcrumb()
   */
  public function setBreadcrumb() {
    $breadcrumb = array();
    $this->setActiveTrail();

    // No breadcrumb for the front page.
    if (drupal_is_front_page()) {
      return;
    }

    $active_trail = menu_get_active_trail();
    $item = end($active_trail);
    if (!empty($item['href'])) {
      // Allow modules to alter the breadcrumb, if possible, as that is much
      // faster than rebuilding an entirely new active trail.
      drupal_alter('menu_breadcrumb', $active_trail, $item);

      // Don't show a link to the current page in the breadcrumb trail.
      $end = end($active_trail);
      if ($item['href'] == $end['href']) {
        $current_page_link = array_pop($active_trail);
      }

      // Remove the tab root (parent) if the current path links to its parent.
      // Normally, the tab root link is included in the breadcrumb, as soon as we
      // are on a local task or any other child link. However, if we are on a
      // default local task (e.g., node/%/view), then we do not want the tab root
      // link (e.g., node/%) to appear, as it would be identical to the current
      // page. Since this behavior also needs to work recursively (i.e., on
      // default local tasks of default local tasks), and since the last non-task
      // link in the trail is used as page title (see menu_get_active_title()),
      // this condition cannot be cleanly integrated into menu_get_active_trail().
      // menu_get_active_trail() already skips all links that link to their parent
      // (commonly MENU_DEFAULT_LOCAL_TASK). In order to also hide the parent link
      // itself, we always remove the last link in the trail, if the current
      // router item links to its parent.
      if (($item['type'] & MENU_LINKS_TO_PARENT) == MENU_LINKS_TO_PARENT) {
        array_pop($active_trail);
      }

      foreach ($active_trail as $parent) {
        $breadcrumb[] = l($parent['title'], $parent['href'], $parent['localized_options']);
      }

      if (!empty($current_page_link)) {
        $drupal_page_title = drupal_get_title();
        if (!empty($drupal_page_title) && $current_page_link['href'] == current_path()) {
          $current_page_link['title'] = $drupal_page_title;
        }
        if (MenutrailbypathConfig::get('breadcrumb_display_current_page') == 'yes_link') {
          $link_options = $current_page_link['localized_options'];
          $link_options['attributes']['class'][] = 'breadcrumb__item--current-page';
          $link_options['attributes']['class'][] = 'active';
          $breadcrumb[] = l($current_page_link['title'], $current_page_link['href'], $link_options);
        } elseif(MenutrailbypathConfig::get('breadcrumb_display_current_page') == 'yes_span') {
          $breadcrumb[] = '<span class="breadcrumb__item--current-page">' . $current_page_link['title'] . '</span>';
        }
      }
    }

    drupal_set_breadcrumb($breadcrumb);
  }

  /**
   * Sets the active_trail by path
   */
  protected function setActiveTrail() {
    $active_trail          = array();
    $trail_paths           = $this->pathHelper->getPaths();
    $trail_menu_links      = $this->menuHelper->getMenuLinksByPaths($trail_paths);
    $breadcrumb_menu_links = $this->groupMenuLinksByPath($trail_menu_links);

    // First breadcrumb is always Home.
    $active_trail[] = array(
      'title'             => t('Home'),
      'href'              => '<front>',
      'localized_options' => array(),
      'type'              => 0,
    );
    // Add links for the trail-paths
    foreach ($trail_paths as $trail_path) {
      $menu_link   = NULL;
      $router_item = menu_get_item($trail_path);

      if (!empty($breadcrumb_menu_links[$trail_path]->link_title)) {
        $menu_link             = (array) $breadcrumb_menu_links[$trail_path];
        $menu_link['external'] = 1;
        $menu_link['type']     = $router_item['type'];
        _menu_link_translate($menu_link);
      }
      else {
        $menu_link = array(
          'title'             => (isset($router_item['title'])) ? $router_item['title'] : '',
          'href'              => $trail_path,
          'localized_options' => array(),
          'type'              => MENU_NORMAL_ITEM,
        );
      }

      if (!empty($menu_link)) {
        $active_trail[] = $menu_link;
      }
    }

    menu_set_active_trail($active_trail);
  }

  /**
   * Group MenuLinks by path, preferring menu_links by menu preference order, menu_link depth, menu_link weight
   *
   * @param array $menu_links
   * @return array
   */
  protected function groupMenuLinksByPath(array $menu_links) {
    $menu_links = array_reverse($menu_links);
    $this->menuHelper->sortMenuLinksByMenuPreference($menu_links);

    $breadcrumb_menu_links = array();
    foreach ($menu_links as $menu_link) {
      if (!isset($breadcrumb_menu_links[$menu_link->link_path]) && !empty($menu_link->link_title)) {
        $breadcrumb_menu_links[$menu_link->link_path] = $menu_link;
      }
    }
    return $breadcrumb_menu_links;
  }
}
