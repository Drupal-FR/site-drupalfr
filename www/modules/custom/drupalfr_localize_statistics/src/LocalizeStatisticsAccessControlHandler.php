<?php

namespace Drupal\drupalfr_localize_statistics;

use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Access\AccessResult;

/**
 * Access controller for the Statistique de la traduction entity.
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
        if (!$entity->isPublished()) {
          return AccessResult::allowedIfHasPermission($account, 'view unpublished statistique de la traduction entities');
        }
        return AccessResult::allowedIfHasPermission($account, 'view published statistique de la traduction entities');

      case 'update':
        return AccessResult::allowedIfHasPermission($account, 'edit statistique de la traduction entities');

      case 'delete':
        return AccessResult::allowedIfHasPermission($account, 'delete statistique de la traduction entities');
    }

    // Unknown operation, no opinion.
    return AccessResult::neutral();
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {
    return AccessResult::allowedIfHasPermission($account, 'add statistique de la traduction entities');
  }

}
