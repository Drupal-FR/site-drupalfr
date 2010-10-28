<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _drupalfr_user_node_info() {
  $items = array(
    'profile' => array(
      'name' => t('Profil'),
      'module' => 'features',
      'description' => t('A user profile built as content.'),
      'has_title' => '1',
      'title_label' => t('Titre'),
      'has_body' => '1',
      'body_label' => t('Biographie'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
