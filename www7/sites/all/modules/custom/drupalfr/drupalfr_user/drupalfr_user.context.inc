<?php

/**
 * @file
 * drupalfr_user.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function drupalfr_user_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dfr_user_profile';
  $context->description = '';
  $context->tag = 'Drupal fr';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'current',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-77b1fcd4b10d9f4b7eaf8706e844970b' => array(
          'module' => 'views',
          'delta' => '77b1fcd4b10d9f4b7eaf8706e844970b',
          'region' => 'content',
          'weight' => '7',
        ),
        'commerce_cart-cart' => array(
          'module' => 'commerce_cart',
          'delta' => 'cart',
          'region' => 'content',
          'weight' => '9',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Drupal fr');
  $export['dfr_user_profile'] = $context;

  return $export;
}
