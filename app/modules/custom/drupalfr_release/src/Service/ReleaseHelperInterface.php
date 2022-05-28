<?php

namespace Drupal\drupalfr_release\Service;

/**
 * Release Helper.
 *
 * @package Drupal\drupalfr_release
 */
interface ReleaseHelperInterface {

  /**
   * Request the XML feed to get the releases information.
   *
   * @return array
   *   A list of releases information.
   */
  public function getFeedReleases();

  /**
   * Import releases during cron.
   *
   * @param array $data
   *   A list of release information to import.
   *
   * @return array
   *   An array of ids of entity updated or created.
   */
  public function importReleaseListData(array $data);

}
