<?php

/**
 * @file twitter_pull.admin.inc
 * 
 * Configuration settings page for twitter_pull module.
 */

function twitter_pull_config_page() {
  
  // check for twitter-api-php library
  $lib = libraries_get_path('twitter-api-php');
  if (!$lib) {
    drupal_set_message(t('Please install TwitterAPIExchange as a library.  http://github.com/j7mbo/twitter-api-php'), 'error');
  }
  
  $desc =<<<EOF
Using twitter_pull with v1.1 of the Twitter api requires you register your site as a twitter app.  To do this, please
visit https://dev.twitter.com/apps/new and create an application.  Click "Create my access token" at the bottom of the
page.  Then copy the Consumer key, Consumer secret, Access token, and Access token secret into the fields below.
EOF;
  
  $form = array(
    'twitter_api_key' => array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#description' => t($desc),
      '#title' => t('Twitter v1.1 API keys'),

      'twitter_pull_consumer_key' => array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => variable_get('twitter_pull_consumer_key', ''),
        '#title' => t('Consumer Key'),     
      ),

      'twitter_pull_consumer_secret' => array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => variable_get('twitter_pull_consumer_secret', ''),
        '#title' => t('Consumer Secret'),
      ),
        
      'twitter_pull_oauth_access_token' => array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => variable_get('twitter_pull_oauth_access_token', ''),
        '#title' => t('Access Token'),
      ),
        
      'twitter_pull_oauth_access_token_secret' => array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => variable_get('twitter_pull_oauth_access_token_secret', ''),
        '#title' => t('Access Token Secret'),
      ),   
    ) 
  );
  
  
  return system_settings_form($form);
}