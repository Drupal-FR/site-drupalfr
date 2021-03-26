<?php

namespace Drupal\drupalfr_search\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Url;

/**
 * Provides a 'SearchHeaderBlock' block.
 *
 * @Block(
 *  id = "search_header_block",
 *  admin_label = @Translation("Search header block"),
 * )
 */
class SearchHeaderBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [
      'search_link' => [
        '#type' => 'link',
        '#url' => Url::fromUserInput('/recherche'),
        '#title' => $this->t('Search'),
      ],
    ];

    return $build;
  }

}
