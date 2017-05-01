<?php

namespace Drupal\drupalfr_theme\Plugin\Alter;

use Drupal\bootstrap\Utility\Variables;
use Drupal\bootstrap\Plugin\Alter\ThemeSuggestions as BoostrapThemeSuggestions;

/**
 * Implements hook_theme_suggestions_alter().
 *
 * @ingroup plugins_alter
 *
 * @BootstrapAlter("theme_suggestions")
 */
class ThemeSuggestions extends BoostrapThemeSuggestions {

  /**
   * {@inheritdoc}
   */
  public function alter(&$suggestions, &$context1 = NULL, &$hook = NULL) {
    parent::alter($suggestions, $context1, $hook);

    $variables = Variables::create($context1);

    switch ($hook) {
      case 'field':
        $suggestions[] = 'field__' . $variables->element->getProperty('entity_type') . '__' . $variables->element->getProperty('field_name') . '__' . $variables->element->getProperty('bundle') . '__' . $variables->element->getProperty('view_mode');
        break;

    }
  }

}
