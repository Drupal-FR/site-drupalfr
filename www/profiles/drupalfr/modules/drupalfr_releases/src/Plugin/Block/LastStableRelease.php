<?php

namespace Drupal\drupalfr_release\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\drupalfr_release\Release;

/**
 * Provides a 'LastStableRelease' Block.
 *
 * @Block(
 *   id = "last_stable_release",
 *   admin_label = @Translation("Last stable release  block"),
 * )
 */
class LastStableRelease extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    return [
      '#theme' => 'drupalfr_release_last_stable',
    ];
  }

}