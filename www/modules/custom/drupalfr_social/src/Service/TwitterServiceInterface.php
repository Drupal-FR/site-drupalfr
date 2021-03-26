<?php

namespace Drupal\drupalfr_social\Service;

/**
 * Twitter service interface.
 *
 * @package Drupal\drupalfr_social
 */
interface TwitterServiceInterface {

  /**
   * Retrieve statuses from Twitter.
   *
   * @param string $path
   *   The path to get the statuses.
   * @param array $options
   *   Options to retrieve statuses.
   *
   * @return array
   *   An array of statuses object.
   *
   * @see: https://twitteroauth.com/
   * @see: https://dev.twitter.com/rest/public
   */
  public function getStatuses($path, array $options);

}
