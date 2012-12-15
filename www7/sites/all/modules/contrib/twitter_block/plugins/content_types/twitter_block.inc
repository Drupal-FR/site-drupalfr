<?php

/**
 * @file
 * Ctools content type plugin that shows a Twitter Block.
 */

/**
 * Plugin definition.
 */
$plugin = array(
  'title' => t('Twitter Block'),
  'category' => t('Miscellaneous'),
  'render callback' => 'twitter_block_content_type_render',
  'defaults' => array(),
  'edit form' => 'twitter_block_content_type_edit_form',
);

/**
 * Render function for the block content.
 */
function twitter_block_content_type_render($subtype, $conf, $args) {
  $block = new stdClass();
  $block->content = twitter_block_load_tweets($conf);
  return $block;
}

/**
 * Block configuration form.
 */
function twitter_block_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['search_string'] = array(
    '#type' => 'textfield',
    '#title' => 'The string or username to search.',
    '#size' => 32,
    '#default_value' => $conf['search_string'],
  );

  $form['search_type'] = array(
    '#type' => 'select',
    '#title' => t('Choose the type of Tweets to return.'),
    '#options' => array(
      'searchHashtag' => t('Tweets mentioning #hashtag or search string.'),
      'getTweetsFrom' => t('Tweets sent from the Twitter user.'),
      'getReplies' => t('Replies to the Twitter user.'),
      'getMentions' => t('Tweets mentioning the Twitter user.'),
    ),
    '#default_value' => $conf['search_type'],
  );

  $form['lang'] = array(
    '#type'  => 'textfield',
    '#title' => 'Language',
    '#description'  => t('If you would like to limit the search by a language, '.
            'you can set it here. This should be the specific string expected by '.
            'Twitter for your language. See the Twitter advanced search page for '.
            'more information. (Hint: English is "en"). Leave blank if you don\'t '.
            'want to filter by language at all.'),
    '#size'  => 12,
    '#default_value' => $conf['lang'],
  );

  $form['results_per_page'] = array(
    '#type' => 'textfield',
    '#title' => 'Number of tweets to return',
    '#description' => t('Please enter the number of tweets to return per page up to a maximum of 100.'),
    '#size' => 12,
    '#default_value' => $conf['results_per_page'],
  );

  return $form;
}

/**
 * Block config submit handler.
 */
function twitter_block_content_type_edit_form_submit($form, &$form_state) {
  foreach (array('search_string', 'search_type', 'lang', 'results_per_page') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Panels admin info function. Returns the info that you see in panels admin interface
 * once you have placed the block in a pane.
 */
function twitter_block_content_type_admin_info($subtype, $conf) {
  $strings = array(
    'searchHashtag' => t('Tweets mentioning #hashtag or search string'),
    'getTweetsFrom' => t('Tweets sent from the Twitter user'),
    'getReplies' => t('Replies to the Twitter user'),
    'getMentions' => t('Tweets mentioning the Twitter user'),
  );

  $block = twitter_block_content_type_render($subtype, $conf, array());
  $block->title = $strings[$conf['search_type']] .' <strong>('. $conf['search_string'] .')</strong>';

  return $block;
}
