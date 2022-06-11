<?php

namespace Drupal\drupalfr_localize_statistics;

use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\Core\Link;

/**
 * Defines a class to build a listing of localize statistics entities.
 *
 * @ingroup drupalfr_localize_statistics
 */
class LocalizeStatisticsListBuilder extends EntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header['id'] = $this->t('localize statistics ID');
    $header['name'] = $this->t('Name');
    $header['value'] = $this->t('Value');
    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    /** @var \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatistics $entity */
    $row['id'] = $entity->id();
    $row['name'] = Link::createFromRoute(
      $entity->label(),
      'entity.localize_statistics.edit_form',
      ['localize_statistics' => $entity->id()]
    );
    $row['value'] = $entity->get('value')->value;
    return $row + parent::buildRow($entity);
  }

}
