<?php
/**
 * Implements hook_preprocess_html
 */
function drupalfr7_preprocess_html(&$vars) {
  $vars['classes_array'][] = 'no-grid';
}
