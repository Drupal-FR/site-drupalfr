<?php

namespace Drupal\drupalfr_localize_statistics;

use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\Core\Link;

/**
 * Defines a class to build a listing of Statistique de la traduction entities.
 *
 * @ingroup drupalfr_localize_statistics
 */
class LocalizeStatisticsListBuilder extends EntityListBuilder {


  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header['id'] = $this->t('Statistique de la traduction ID');
    $header['name'] = $this->t('Name');
    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    /* @var $entity \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatistics */
    $row['id'] = $entity->id();
    $row['name'] = Link::createFromRoute(
      $entity->label(),
      'entity.localize_statistics.edit_form',
      ['localize_statistics' => $entity->id()]
    );
    return $row + parent::buildRow($entity);
  }

}
