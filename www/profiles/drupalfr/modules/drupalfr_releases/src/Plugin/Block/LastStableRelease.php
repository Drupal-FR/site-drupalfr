<?php

namespace Drupal\drupalfr_releases\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Provides a 'LastStableRelease' Block.
 *
 * @Block(
 *   id = "last_stable_release",
 *   admin_label = @Translation("Last stable release block"),
 * )
 */
class LastStableRelease extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    return [
      '#theme' => 'drupalfr_releases_last_stable',
    ];
  }

}
