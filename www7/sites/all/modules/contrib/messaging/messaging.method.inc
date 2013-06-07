<?php
/**
 * @file
 * Drupal Messaging Framework - Send_Method class file
 */

// Outgoing method
define('MESSAGING_TYPE_OUTGOING', 4);
// Incoming method
define('MESSAGING_TYPE_INCOMING', 8);

/**
 * Base class for all Incoming and Sending methods
 */
abstract class Messaging_Method {
  // Method identifier
  public $method;
  // Method type
  public $type;
  // Method group
  public $enabled = TRUE;
  public $anonymous = FALSE;
  // Remaining info array
  public $info = array();
  /**
   * Build send method from info array
   *
   * Some of the array values will be set as properties for the object. Some others won't as they're just
   * for formatting, so they'll be kept only in the $object->info array
   */
  function __construct($method, $info = array()) {
    $this->method = $method;
    foreach ($info as $key => $value) {
      $this->$key = $value;
    }
    $this->info = $info;
  }
  /**
   * Get method title for administrator list
   */
  function get_title() {
    return $this->method_info('title', t('Method'));
  }
  /**
   * Get method name, may be overridden by info properties
   */
  function get_name() {
    return $this->method_info('name', $this->get_title());
  }
  /**
   * Get method description, may be overridden by info properties
   */
  function get_description() {
    return $this->method_info('description', t('Send messages.'));
  }

  /**
   * Get info property
   */
  function method_info($property = NULL, $default = NULL) {
    if ($property) {
      return isset($this->info[$property]) ? $this->info[$property] : $default;
    }
    else {
      return $this->info;
    }
  }
  /**
   * Returns default messaging method
   */
  static function default_method($account = NULL) {
    if ($account && !empty($account->messaging_default) && messaging_method($account->messaging_default)->user_access($account)) {
      return $account->messaging_default;
    }
    elseif ($method = variable_get('messaging_default_method', '')) {
      return $method;
    }
    else {
      return key(messaging_method_info());
    }
  }

  /**
   * Update messaging method.
   * 
   * When a messaging method is disabled, we need to update current settings for this and other modues
   * 
   * @param $method
   *   Method to disable
   * @param $replace
   *   Optional replacement method suggested by the disabled one.
   */
  static function method_disable($method, $replace = NULL) {
    module_load_include('install', 'messaging');
    $replace = isset($replace) ? $replace : messaging_update_method_replace($method, TRUE);
    messaging_update_method_disable($method, $replace);
    if ($replace) {
      drupal_set_message(t('Disabled messaging sending method %method and replaced by %replace', array('%method' => messaging_method_info($method, 'title'), '%replace' => messaging_method_info($replace, 'title'))));
    } else {
      // It seems all methods are disabled, print warning
      drupal_set_message(t('Disabled messaging sending method but cannot find a replacement. Please, enable some other sending method.'), 'error');
    }  
  }

  /**
   * Log message action
   */
  protected function message_log($text, $message) {
    $text = $this->get_name() . ': ' . $text;
    messaging_log($text, array('message' => $message));
  }
}

/**
 * Sending method, implements all specific method functionality
 * 
 * Old callback functions are
 * - send
 * - destination
 */
class Messaging_Send_Method extends Messaging_Method {
  // Default parameters for send method
  public $method = 'send';
  public $type = 'web';
  public $enabled = TRUE;
  // Available for anonymous users
  public $anonymous = FALSE;
  // Qeueue messages instead of sending
  public $queue = FALSE;
  // Whether we want all messages logged for this method
  public $log = FALSE;
  // Default format for message rendering
  public $format = MESSAGING_FORMAT_HTML;  
  /**
  * Get address type
  */
  public static function address_type() {
    return 'none';
  }

  /**
  * Get address info property
  */
  public static function address_info($property = NULL, $default = NULL) {
    return messaging_address_info(self::address_type(), $property, $default);
  }

  /**
   * Get address name
   */
  public static function address_name() {
    return self::address_info('name', t('Address'));
  }

   /**
   * Message processing: Decide on queue, log, cron and send options, prepare parameters
   * 
   * At this stage, the message can be still redirected through other sending method, or marked for discard
   */
  function message_prepare($message) {
    // Set queueing if not message priority
    if ($this->queue && $message->priority <= 0) {
      $message->queue = 1;
    }
    $message->log = $message->log || $this->log;
    $this->message_log('Message prepared', $message);
  }

  /**
   * Prepare message for specific user. Check availability, redirect, etc..
   * 
   * Redirecting is only possible when we are sending to a user account, not for anonymous destinations
   */
  function message_user($message) {
  }

  /**
   * Prepare template for this method.
   * 
   * Here we can clone and alter the template or just return the same
   */  
  function message_render($message) {
    $message->get_template()
      ->set_method($this->method)
      ->set_format($this->format);
  }
  /**
   * Send message to a single or multiple destination
   */
  function message_send($message) {
    $this->message_log('Message send', $message);
    $destinations = $message->get_destinations();
    $results = $this->send_multiple($destinations, $message);
    $message->set_results($results);
    return (boolean)array_filter($results);
  }
  
  /**
   * Send message to destination
   */
  function send_destination($destination, $message) {
    // Set destination to message template
    $message->get_template()->set_destination($destination);
    return $this->send_address($destination->get_address(), $message);
  }
  /**
   * Send message to address, to be implemented by send method
   */
  function send_address($address, $message) {
    return FALSE;
  }
  /**
   * Send message to multiple destinations
   */
  function send_multiple($destinations, $message) {
    $results = array();
    foreach ($destinations as $key => $destination) {
      $results[$key] = $this->send_destination($destination, $message);
    }
    return $results;    
  }

  /**
   * Get message parameters for this method
   */
  function message_params($message) {
    return $message->get_params($this->method) + $message->get_params($this->type) + $this->default_params();    
  }

  /**
   * Queue message for next delivery
   * 
   * By default it is saved to the store, though some sending methods like 'simple' may not consider queueing.
   * 
   * If no queue available we send the message
   */
  function message_queue($message) {
    if ($queue = messaging_store('queue')) {
      $message->queued = time();
      $queue->message_queue($message);
      return TRUE;
    }
    else {
      $message->queued = 0;
      return $this->message_send($message);
    }
  }
  
  /**
   * Get destination for user account
   * 
   * @param $account
   *   User account object or uid
   */
  function user_destination($account) {
    return Messaging_Destination::build_user($this->address_type(), $account);
  }

  /**
   * Check user access to this method
   */
  function user_access($account) {
    if (!$account->uid && !$this->anonymous) {
      return FALSE;
    }
    if ($permission = $this->method_info('access')) {
      return user_access($permission, $account);
    }
    else {
      return TRUE;
    }
  }

  /**
   * Make address into destination
   */
  public function address_destination($address, $validate = TRUE) {
    return Messaging_Destination::build_address($this->address_type(), $address, $validate);
  }
  /**
   * Validate address
   */
  public function address_validate($address) {
    return Messaging_Destination::validate_address($address, $this->address_type());
  }

  /**
   * Format address for display
   */
  public function format_address($address, $format = MESSAGING_FORMAT_PLAIN) {
    return Messaging_Destination::format_address($address, $format, $this->address_type());
  }

  /**
   * Check whether it supports anonyous destination
   */
  function supports_anonymous() {
    return TRUE;
  }
  
  /**
   * Get default parameters for this method
   */
  static function default_params() {
    return array();
  }
}