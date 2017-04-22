<?php

class MenutrailbypathUrlHelper {
  /**
   * Cached version of the url function
   *
   * @param string $path
   * @return string mixed
   */
  public function getUrl($path) {
    static $urls;
    if (!isset($urls[$path])) {
      $urls[$path] = url($path);
    }
    return $urls[$path];
  }
}
