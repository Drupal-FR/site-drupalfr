<?php

namespace Drupal\drupalfr_localize_statistics\Service;

/**
 * Localize Statistics cron.
 *
 * @package Drupal\drupalfr_localize_statistics
 */
interface LocalizeStatisticsCronInterface
{

  /**
   * Method executed by hook_cron().
   */
    public function cron();
}
