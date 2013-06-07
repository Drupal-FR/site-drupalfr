<?php
/**
 * @file
 * Drupal Messaging Framework - Default class file
 */

/**
 * Message destination class
 * 
 * A destination can be mapped to a sending address (mail, mail@example.com), though optionally it can be linked
 * to an user id. When adding a destination to a message it may have a sending method too. 
 */
class Messaging_Destination extends Messaging_Object {
  // Destination status
  const STATUS_PENDING = 0;
  const STATUS_ACTIVE = 1;

  // Object unique id
  public $mdid = 0;
  // Destination type (mail, xmpp, etc..)
  public $type;
  // User id.
  public $uid;
  // Address for this sending method
  public $address;
  // User name, not saved to db
  public $name;
  // Date sent
  public $sent = 0;
  // Status
  public $status = 0;

  /**
   * Get address type name for display
   */
  function get_title() {
    return $this->address_info('name', t('Address'));
  }
  
  /**
   * Get address name (will be user name if available)
   */
  function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    else {
      return $this->get_user()->name;
    }
  }

  /**
   * Get address for sending
   */
  function get_address() {
    return isset($this->address) ? $this->address : NULL;
  }

  /**
   * Get user account
   */
  function get_user() {
    $user = isset($this->uid) ? user_load($this->uid) : NULL;
    if (!$user) {
      $user = drupal_anonymous_user();
      $user->name = t('User');
    }
    return $user;
  }
  /**
   * Set user for this destination
   */
  function set_user($user) {
    $this->uid = $user->uid;
    $this->name = $user->name;
    if (!isset($this->address)) {
      $this->address = $this->get_address_from_user($user);
    }
    return $this;
  }
  /**
   * Get user from address
   */
  public static function get_user_from_address($address) {
    return NULL;
  }
  /**
   * Get address from user
   */
  public static function get_address_from_user($user) {
    return NULL;
  }

  /**
   * Get from db using conditions
   */
  public static function get($params) {
    $dest = self::load_multiple(array(), $params);
    return reset($dest);
  }
  /**
   * Build destination object
   */
  public static function build($type, $address = NULL, $uid = NULL, $validate = FALSE) {
    if (!$validate || self::validate_address($address, $type)) {
      return self::build_type($type, array('address' => $address, 'uid' => $uid));
    }
  }
  /**
   * Build destination from db object
   */
  public static function build_object($template) {
    return self::build_type($template->type, $template);
  }
  /**
   * Build from address type
   */
  public static function build_type($type, $template = NULL) {
    $class = messaging_address_info($type, 'class', 'Messaging_Destination');
    $destination = new $class($template);
    $destination->type = $type;
    return $destination;
  }
  /**
   * Build from method, address, user
   */
  public static function build_method($method, $address = NULL, $account = NULL) {
    $type = messaging_send_method($method)->address_type();
    $uid = $account ? $account->uid : 0;
    if ($type && $address) {
      return self::build($type, $address, $uid);
    }
    elseif($type && $account) {
      return self::build_user($type, $account);
    }
    elseif ($account) {
      return messaging_send_method($method)->user_destination($account);
    }
  }
  /**
   * Build from address
   */
  public static function build_address($type, $address, $validate = FALSE) {
    return self::build($type, $address, NULL, $validate);
  }
  /**
   * Build from user account
   */
  public static function build_user($type, $user) {
    $destination = self::build_type($type);
    $destination->set_user($user);
    return $destination;  
  }
  /**
   * Load object from database
   */
  public static function load($mdid) {
    $dest = entity_load('messaging_destination', array($mdid));
    return $dest ? $dest[$mdid] : FALSE;
  }
  /**
   * Load multiple events
   */
  public static function load_multiple($mdids = array(), $conditions = array()) {
    return entity_load('messaging_destination', $mdids, $conditions);
  }

  
  /**
   * Validate values to create a destination
   * 
   * @param $method
   *   Send method
   * @param $address
   *   Address value
   * @param $account
   *   Optional user id or account object the destination is intended for
   */
  public static function validate_method($method, $address, $account = NULL) {
    // First validate address and check permission
    $send_method = messaging_send_method($method);
    if (!$send_method || !$send_method->address_validate($address)) {
      return FALSE;
    }
    if (isset($account)) {
      $account = messaging_user_object($account);
      if (!$account || !$send_method->user_access($account)) {
        return FALSE;
      }
    }
    if ($type = messaging_method_info($method, 'address_type')) {
      return self::validate_type($type, $address, $account);
    }
  }
  /**
   * Validate values to create a destination
   * 
   * @param $type
   *   Address type
   * @param $address
   *   Address value
   * @param $account
   *   Optional user id or account object
   */
  public static function validate_type($type, $address, $account = NULL) {
    // First try validate callback for this address type
    if (!self::validate_address($type, $address)) {
      return FALSE;
    }
    elseif (isset($account)) {
      $uid = messaging_user_uid($account);
      if ($existing = self::get_by_address($type, $address)) {
        // There's already a destination with these parameters, check user
        // It will be ok if users match or it was anonymous before
        return !isset($account) || !$existing->uid || $existing->uid == $uid;
      }
      elseif ($address_uid = self::address2uid($type, $address)) {
        // Ok if this address belongs to the same user or to no user 
        return !$address_uid || $address_uid == $uid;
      }
    }
    return TRUE;
  }

  /**
   * Validate address format
   * 
   * @param $address
   *   Destination address
   * @param $type
   *   Optional address type, for static calls
   */
  public static function validate_address($address, $type = NULL) {
    if ($type) {
      return self::static_invoke($type, 'validate_address', $address);
    }
    else {
      return !empty($address);
    }
  }

  /**
   * Create from array data
   */
  public static function create($data) {
    // If no more clues, we create it for anonymous user
    $data += array('uid' => 0, 'method' => NULL, 'type' => NULL, 'address' => NULL);
    if ($data['type'] && $data['address']) {
      return self::create_type($data['type'], $data['address'], $data['uid']);
    }
    else {
      return self::create_method($data['method'], $data['address'], $data['uid']);
    }
  }

  /**
   * Create for sending method
   */
  public static function create_method($send_method, $address, $uid) {
    if ($type = messaging_method_info($send_method, 'address_type')) {
      return self::create_type($type, $address, $uid);
    }
  }
  /**
   * Create with parameters
   */
  public static function create_type($type, $address, $uid) {
    if ($existing = self::get_by_address($type, $address)) {
      if ($existing->uid != $uid) {
        $existing->uid = $uid;    
        $existing->save();
      }
      return $existing;
    }
    // Transitional case, row for user with no address, update it
    elseif ($uid && ($existing = self::get(array('uid' => $uid, 'type' => $type, 'address' => '')))) {
      $existing->address = $address;
      $existing->save();
      return $existing;
    }
    else {
      $destination = self::build(array('type' => $type, 'address' => $address, 'uid' => $uid));
      $destination->save();
      return $destination;
    }
  }
  /**
   * Get destination by method and address. This allows advanced caching.
   */
  public static function get_by_address($type, $address) {
    $cached = self::cache_by_address($type, $address);
    if (isset($cached)) {
      return $cached;
    }
    else {
      return self::get(array('type' => $type, 'address' => $address));
    }
  }

  /**
   * Get unique index for this destination
   */
  function index() {
    return $this->type . ':' . $this->address;
  }

  /**
   * Get address type information
   */
  function address_info($property = NULL, $default = NULL) {
    if (!empty($this->type)) {
      return messaging_address_info($this->type, $property, $default);
    }
  }

  /**
   * Format destination
   */
  function format($type = 'short', $format = MESSAGING_FORMAT_PLAIN) {
    $address = $this->get_address();
    switch ($type) {
      case 'short':
        return $this->format_address($address, $format);
      case 'long':
        return $this->get_name() . ': ' . $this->format_address($address, $format);
    }
  }

  /**
   * Format address.
   * 
   * @param $address
   *   Destination address
   * @param $type
   *   Optional address type, for static calls
   */
  public static function format_address($address, $format = MESSAGING_FORMAT_PLAIN, $type = NULL) {
    if ($type) {
      return self::static_invoke($type, 'format_address', $address, $format);
    }
    else {
      return isset($address) ? check_plain($address) : t('none');
    }
  }
  
  /**
   * Delete messaging destination object/s
   */
  public static function delete_multiple($params) {
    $query = db_delete('messaging_destination');
    foreach ($params as $field => $value) {
      $query->condition($field, $value);
    }
    return $query->execute();
  }

  // Magic function, format as string
  public function __toString() {
    return 'Destination: ' . $this->index();
  }

  /**
   * Save object to cache
   */
  public function cache_save() {
    $this->cache_by_address($this->type, $this->address, $this);
  }
  /**
   * Save object to cache
   */
  public function cache_delete() {
    parent::cache_delete();
    $this->cache_by_address($this->type, $this->address, FALSE);
  }
  // Store into address cache
  public static function cache_set($key, $object) {
    $cache = &drupal_static('messaging_destination_address');
    return $cache[$key] = $object;
  }
  // Get from address cache
  public static function cache_get($key) {
    $cache = &drupal_static('messaging_destination_address');
    return isset($cache[$key]) ? $cache[$key] : NULL;
  }
  /**
   * Cache get/set by method and address
   */
  public static function cache_by_address($type, $address, $object = NULL) {
    if (isset($object)) {
      return self::cache_set("$type:$address", $object);
    }
    else {
      return self::cache_get("$type:$address");
    }
  }

  /**
   * Run module_invoke_all('notifications_subscription') with this object
   */
  protected function invoke_all($op, $param = NULL) {
    return module_invoke_all('messaging_destination', $op, $this, $param);    
  }

  /**
   * Invoke static method on address type.
   * 
   * @param $type
   *   Address type
   * @param $method
   *   Method name
   * @param $arg1, $arg2...
   */
  protected static function static_invoke() {
    $args = func_get_args();
    $type = array_shift($args);
    $function = array_shift($args);
    $class = messaging_address_info($type, 'class', 'Messaging_Destination');
    return call_user_func_array(array($class, $function), $args);
  }
}

/**
 * Destination is a system user
 */
class Messaging_User_Destination extends Messaging_Destination {
  public $type = 'user';
  public $address = '';
  /**
   * Get name for display
   */
  function address_name() { 
    return t('User');
  }
  /**
   * Get address for sending
   */
  function get_address() {
    return $this->get_user();
  }
  /**
   * Set user for this destination
   */
  function set_user($user) {
    $this->uid = $user->uid;
  }
  /**
   * Get unique index for this destination
   */
  function index() {
    return 'user:' . $this->uid;
  }
  /**
   * Get user from address
   */
  public static function get_user_from_address($address) {
    return $address;
  }
  /**
   * Get address from user
   */
  public static function get_address_from_user($user) {
    return $user;
  }
  /**
   * Check address is valid
   */
  public static function validate_address($address, $type = NULL) {
    return is_object($address) ? !empty($address->uid) : FALSE;
  }
  /**
   * Format address
   */
  public static function format_address($user, $format = MESSAGING_FORMAT_PLAIN, $type = NULL) {
    return ($format & MESSAGING_FORMAT_HTML) ? theme('username', array('account' => $user)) : check_plain($user->name);
  }
}
