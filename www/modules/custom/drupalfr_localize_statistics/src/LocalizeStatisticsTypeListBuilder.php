<?php

namespace Drupal\drupalfr_localize_statistics;

use Drupal\Core\Config\Entity\ConfigEntityListBuilder;
use Drupal\Core\Entity\EntityInterface;

/**
 * Provides a listing of localize statistics type entities.
 */
class LocalizeStatisticsTypeListBuilder extends ConfigEntityListBuilder
{

  /**
   * {@inheritdoc}
   */
    public function buildHeader()
    {
        $header['label'] = $this->t('localize statistics type');
        $header['id'] = $this->t('Machine name');
        return $header + parent::buildHeader();
    }

  /**
   * {@inheritdoc}
   */
    public function buildRow(EntityInterface $entity)
    {
        $row['label'] = $entity->label();
        $row['id'] = $entity->id();
        return $row + parent::buildRow($entity);
    }
}
