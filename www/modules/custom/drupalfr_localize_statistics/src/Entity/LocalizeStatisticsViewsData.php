<?php

namespace Drupal\drupalfr_localize_statistics\Entity;

use Drupal\views\EntityViewsData;

/**
 * Provides Views data for Statistique de la traduction entities.
 */
class LocalizeStatisticsViewsData extends EntityViewsData {

  /**
   * {@inheritdoc}
   */
  public function getViewsData() {
    $data = parent::getViewsData();

    // Additional information for Views integration, such as table joins, can be
    // put here.

    return $data;
  }

}
