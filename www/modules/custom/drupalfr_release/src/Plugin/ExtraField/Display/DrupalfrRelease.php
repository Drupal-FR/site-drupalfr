<?php

namespace Drupal\drupalfr_release\Plugin\ExtraField\Display;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\extra_field\Plugin\ExtraFieldDisplayBase;

/**
 * Release Extra field Display.
 *
 * @ExtraFieldDisplay(
 *   id = "release",
 *   label = @Translation("Release"),
 *   bundles = {
 *     "node.release",
 *   }
 * )
 */
class DrupalfrRelease extends ExtraFieldDisplayBase {

  /**
   * {@inheritdoc}
   */
  public function view(ContentEntityInterface $entity) {
    $date_formatter = \Drupal::service('date.formatter');

    $elements['creation_date'] = t('Created the @date', [
      '@date' => $date_formatter->format($entity->getCreatedTime(), 'jour_mois_annee_heure'),
    ])->render();
    return $elements;
  }

}
