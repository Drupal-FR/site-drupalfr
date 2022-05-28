<?php

/**
 * @file
 * Contains localize_statistics.page.inc.
 *
 * Page callback for localize statistics entities.
 */

use Drupal\Core\Render\Element;

/**
 * Prepares variables for localize statistics templates.
 *
 * Default template: localize_statistics.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - elements: An associative array containing the user information and any
 *   - attributes: HTML attributes for the containing element.
 */
function template_preprocess_localize_statistics(array &$variables) {
  // Helpful $content variable for templates.
  foreach (Element::children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }
}
