<?php

/**
 * @file
 * Contains localize_statistics.page.inc.
 *
 * Page callback for Statistique de la traduction entities.
 */

use Drupal\Core\Render\Element;

/**
 * Prepares variables for Statistique de la traduction templates.
 *
 * Default template: localize_statistics.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - elements: An associative array containing the user information and any
 *   - attributes: HTML attributes for the containing element.
 */
function template_preprocess_localize_statistics(array &$variables) {
  // Fetch LocalizeStatistics Entity Object.
  $localize_statistics = $variables['elements']['#localize_statistics'];

  // Helpful $content variable for templates.
  foreach (Element::children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }
}
