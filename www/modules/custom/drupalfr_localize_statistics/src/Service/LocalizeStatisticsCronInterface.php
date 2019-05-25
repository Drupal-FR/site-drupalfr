<?php

namespace Drupal\drupalfr_localize_statistics\Service;

/**
 * Interface LocalizeStatisticsCronInterface.
 *
 * @package Drupal\drupalfr_localize_statistics
 */
interface LocalizeStatisticsCronInterface {

  /**
   * Method executed by hook_cron().
   */
  public function cron();

}
