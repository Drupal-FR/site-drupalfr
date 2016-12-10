<?php

class MenutrailbypathUsortMenulinks {
  private $menu_preference;

  public function __construct($menu_preference) {
    $this->menu_preference = $menu_preference;
  }

  public function compare($a, $b) {
    $result = ($this->menu_preference[$a[1]->menu_name] == $this->menu_preference[$b[1]->menu_name]) ? 0 : (($this->menu_preference[$a[1]->menu_name] < $this->menu_preference[$b[1]->menu_name]) ? -1 : 1);
    return ($result == 0) ? ($a[0] - $b[0]) : $result;
  }
}
