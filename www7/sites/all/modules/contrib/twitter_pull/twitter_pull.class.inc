<?php

/**
 * @file
 * twitter pull class implementation
 */

class twitter_puller {

  var $twitkey;
  var $num_items;
  var $tweets;

  /**
  * @param $twitkey
  *     Twitter key, which can be a username (prepended with @) or hashtag (prepended with #)
  * @param $num_items
  *     maximum number of tweets to pull from Twitter.
  */
  function __construct($twitkey, $num_items) {

    $this->twitkey = $twitkey;
    $this->num_items = $num_items;

    $this->check_arguments();

  }

  function check_arguments() {

    if (empty($this->twitkey) || drupal_strlen($this->twitkey) < 2) {
      throw new Exception(t('Twitter key may not be empty.'));
    }

    $num = intval($this->num_items);
    if ($num <= 0 || $num > 200) {
      throw new Exception(t('Number of Twitter items to pull must be a positive integer less than or equal to 200.'));
    }

  }

  function get_items() {

    $prefix = drupal_substr($this->twitkey, 0, 1);
    $slash = strpos($this->twitkey, '/', 1);
    $num = intval($this->num_items);

    // lists have the format @username/listname
    if ($prefix == '@' && $slash !== FALSE) {
      $username = drupal_substr($this->twitkey, 1, $slash - 1);
      $listname = drupal_substr($this->twitkey, $slash + 1);
      $url = 'https://api.twitter.com/1.1/lists/statuses.json';
      $query = '?slug=' . urlencode($listname)
        . '&owner_screen_name=' . urlencode($username);

    }
    // if the first character is @, then consider the key a username
    elseif ($prefix == "@") {
      $key = drupal_substr($this->twitkey, 1);
      $url = 'https://api.twitter.com/1.1/statuses/user_timeline.json';
      $query = "?screen_name=${key}&count=${num}&include_rts=1";
    }
    // if the first character is ~, then consider the key a favorites feed
    elseif ($prefix == "~") {
      $key = drupal_substr($this->twitkey, 1);
      $url = 'https://api.twitter.com/1.1/favorites/list.json';
      $query = "?screen_name=${key}&count=${num}";
    }
    // otherwise, use the key as a search term
    else {
      if ($num > 100) {
        $num = 100;
      }
      $url = 'https://api.twitter.com/1.1/search/tweets.json';
      $query = '?q=' . rawurlencode($this->twitkey) ."&include_entities=true&count=${num}";
    }

    // Look for the twitter-api-lib, using libraries module if possible
    // http://github.com/j7mbo/twitter-api-php
    if (module_exists('libraries')) {
      $lib = libraries_get_path('twitter-api-php');
    }
    //If libraries module is not available, try a common default location:
    else {
      $path_default = DRUPAL_ROOT . '/sites/all/libraries/twitter-api-php';
      $lib = is_dir($path_default) ? $path_default : FALSE;
    }

    if (!$lib) {
      drupal_set_message(t('Please install TwitterAPIExchange as a library.  http://github.com/j7mbo/twitter-api-php'), 'error');
      return;
    }
    require_once($lib . '/TwitterAPIExchange.php');
    
    $settings = array();
    foreach (array('consumer_key', 'consumer_secret', 'oauth_access_token', 'oauth_access_token_secret') as $var) {
      $settings[$var] = variable_get('twitter_pull_'.$var, FALSE);
    }
    
    if (in_array(FALSE, $settings, TRUE)) {
      drupal_set_message('Twitter has not been configured yet.  Please contact your admin.', 'error');
      return;
    }

    $url = '' . $url;
    
    $twitter = new TwitterAPIExchange($settings);
    $req = $twitter->setGetfield($query)
      ->buildOauth($url, 'GET')
      ->performRequest();
    $items = json_decode($req);

    $this->parse_items($items);

  }

  function parse_items($items) {
    $tweets = array();

    //-- If search response then items are one level lower.
    if (isset($items->statuses) && is_array($items->statuses)) {
      $items = $items->statuses;
    }

    if (is_array($items)) {
      $items = array_slice($items, 0, $this->num_items);
      foreach ($items as $item) {
        $obj = new stdClass();

        if (isset($item->retweeted_status)) {
          $obj->id = check_plain($item->retweeted_status->id_str);
          $obj->username = (isset($item->retweeted_status->user) && !empty($item->retweeted_status->user->screen_name)) ? $item->retweeted_status->user->screen_name : $item->retweeted_status->from_user;
          $obj->username = check_plain($obj->username);
          //get the user photo for the retweet
          $obj->userphoto = (isset($item->retweeted_status->user) && !empty($item->retweeted_status->user->profile_image_url)) ? $item->retweeted_status->user->profile_image_url : $item->retweeted_status->profile_image_url;
          $obj->userphoto = check_plain($obj->userphoto);
          $obj->userphoto_https = (isset($item->retweeted_status->user) && !empty($item->retweeted_status->user->profile_image_url_https)) ? $item->retweeted_status->user->profile_image_url_https : $item->retweeted_status->profile_image_url_https;
          $obj->userphoto_https = check_plain($obj->userphoto_https);

          $obj->text = filter_xss($item->retweeted_status->text);
          //-- Convert date to unix timestamp so themer can easily work with it.
          $obj->timestamp = strtotime($item->retweeted_status->created_at);
        }
        else {
          $obj->id = check_plain($item->id_str);
          $obj->username = (isset($item->user) && !empty($item->user->screen_name)) ? $item->user->screen_name : $item->from_user;
          $obj->username = check_plain($obj->username);
          //retrieve the user photo
          $obj->userphoto = (isset($item->user) && !empty($item->user->profile_image_url)) ? $item->user->profile_image_url : $item->profile_image_url;
          $obj->userphoto = check_plain($obj->userphoto);
          $obj->userphoto_https = (isset($item->user) && !empty($item->user->profile_image_url_https)) ? $item->user->profile_image_url_https : $item->profile_image_url_https;
          $obj->userphoto_https = check_plain($obj->userphoto_https);

          $obj->text = filter_xss($item->text);
          //-- Convert date to unix timestamp so themer can easily work with it.
          $obj->timestamp = strtotime($item->created_at);
        }
        $tweets[] = $obj;
      }
    }

    $this->tweets = $tweets;
  }

}
