<?php

namespace Drupal\drupalfr_localize_statistics\Entity;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityChangedInterface;
use Drupal\user\EntityOwnerInterface;

/**
 * Provides an interface for defining Statistique de la traduction entities.
 *
 * @ingroup drupalfr_localize_statistics
 */
interface LocalizeStatisticsInterface extends ContentEntityInterface, EntityChangedInterface, EntityOwnerInterface {

  // Add get/set methods for your configuration properties here.

  /**
   * Gets the Statistique de la traduction name.
   *
   * @return string
   *   Name of the Statistique de la traduction.
   */
  public function getName();

  /**
   * Sets the Statistique de la traduction name.
   *
   * @param string $name
   *   The Statistique de la traduction name.
   *
   * @return \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatisticsInterface
   *   The called Statistique de la traduction entity.
   */
  public function setName($name);

  /**
   * Gets the Statistique de la traduction creation timestamp.
   *
   * @return int
   *   Creation timestamp of the Statistique de la traduction.
   */
  public function getCreatedTime();

  /**
   * Sets the Statistique de la traduction creation timestamp.
   *
   * @param int $timestamp
   *   The Statistique de la traduction creation timestamp.
   *
   * @return \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatisticsInterface
   *   The called Statistique de la traduction entity.
   */
  public function setCreatedTime($timestamp);

  /**
   * Returns the Statistique de la traduction published status indicator.
   *
   * Unpublished Statistique de la traduction are only visible to restricted users.
   *
   * @return bool
   *   TRUE if the Statistique de la traduction is published.
   */
  public function isPublished();

  /**
   * Sets the published status of a Statistique de la traduction.
   *
   * @param bool $published
   *   TRUE to set this Statistique de la traduction to published, FALSE to set it to unpublished.
   *
   * @return \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatisticsInterface
   *   The called Statistique de la traduction entity.
   */
  public function setPublished($published);

}
