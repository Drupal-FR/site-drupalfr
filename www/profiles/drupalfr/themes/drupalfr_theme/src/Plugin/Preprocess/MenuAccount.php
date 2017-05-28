<?php

namespace Drupal\drupalfr_theme\Plugin\Preprocess;

use Drupal\bootstrap\Plugin\Preprocess\PreprocessBase;
use Drupal\bootstrap\Utility\Variables;
use Drupal\Component\Render\FormattableMarkup;

/**
 * Implements hook_preprocess_menu__account().
 *
 * @ingroup plugins_preprocess
 *
 * @BootstrapPreprocess("menu__account")
 */
class MenuAccount extends PreprocessBase {

  /**
   * {@inheritdoc}
   */
  public function preprocessVariables(Variables $variables) {
    // Add icon on user link.
    if (isset($variables['items']['drupalfr_user.account.compte'])) {
      $variables['items']['drupalfr_user.account.compte']['title'] = new FormattableMarkup(
        '<i class="fa fa-user-circle-o" aria-hidden="true"></i><span class="sr-only">@original_title</span>',
        [
          '@original_title' => $variables['items']['drupalfr_user.account.compte']['title'],
        ]
      );
    }

    parent::preprocessVariables($variables);
  }

}
