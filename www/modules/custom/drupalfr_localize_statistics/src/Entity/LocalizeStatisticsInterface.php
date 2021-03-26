<?php

namespace Drupal\drupalfr_localize_statistics\Entity;

use Drupal\Core\Entity\ContentEntityInterface;

/**
 * Provides an interface for defining localize statistics entities.
 *
 * @ingroup drupalfr_localize_statistics
 */
interface LocalizeStatisticsInterface extends ContentEntityInterface {

  /**
   * Gets the localize statistics creation timestamp.
   *
   * @return int
   *   Creation timestamp of the localize statistics.
   */
  public function getCreatedTime();

  /**
   * Sets the localize statistics creation timestamp.
   *
   * @param int $timestamp
   *   The localize statistics creation timestamp.
   *
   * @return \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatisticsInterface
   *   The called localize statistics entity.
   */
  public function setCreatedTime($timestamp);

}
