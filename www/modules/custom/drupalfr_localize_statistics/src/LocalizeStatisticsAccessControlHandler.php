<?php

namespace Drupal\drupalfr_localize_statistics;

use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Access\AccessResult;

/**
 * Access controller for the localize statistics entity.
 *
 * @see \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatistics.
 */
class LocalizeStatisticsAccessControlHandler extends EntityAccessControlHandler {

  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, AccountInterface $account) {
    /** @var \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatisticsInterface $entity */
    switch ($operation) {
      case 'view':
        return AccessResult::allowedIfHasPermission($account, 'view localize statistics entities');

      case 'update':
        return AccessResult::allowedIfHasPermission($account, 'edit localize statistics entities');

      case 'delete':
        return AccessResult::allowedIfHasPermission($account, 'delete localize statistics entities');
    }

    // Unknown operation, no opinion.
    return AccessResult::neutral();
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {
    return AccessResult::allowedIfHasPermission($account, 'add localize statistics entities');
  }

}
