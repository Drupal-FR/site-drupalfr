<?php

namespace Drupal\drupalfr_theme\Plugin\Preprocess;

use Drupal\bootstrap\Plugin\Preprocess\PreprocessBase;
use Drupal\bootstrap\Utility\Variables;
use Drupal\Component\Render\FormattableMarkup;

/**
 * Implements hook_preprocess_block__search_header_block().
 *
 * @ingroup plugins_preprocess
 *
 * @BootstrapPreprocess("block__search_header_block")
 */
class SearchHeaderBlock extends PreprocessBase {

  /**
   * {@inheritdoc}
   */
  public function preprocessVariables(Variables $variables) {
    // Add icon on search link.
    $render_array = $variables->getArrayCopy();
    if (isset($render_array['content']['search_link'])) {
      $render_array['content']['search_link']['#title'] = new FormattableMarkup(
        '<i class="fa fa-search" aria-hidden="true"></i><span class="sr-only">@original_title</span>',
        [
          '@original_title' => $render_array['content']['search_link']['#title'],
        ]
      );
      $variables->offsetSet('content', $render_array['content']);
    }

    parent::preprocessVariables($variables);
  }

}
