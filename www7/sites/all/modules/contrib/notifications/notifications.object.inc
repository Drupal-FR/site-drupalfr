<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Base class for all Notifications entities
 */
abstract class Notifications_Entity {
  protected $type;
  /**
   * Constructor
   */
  public function __construct($template = NULL) {
    if ($template) {
      $properties = (array)$template;
      foreach ($properties as $key => $value) {
        $this->$key = $value;
      }
    }
  }
  /**
   * Get object title
   */
  public abstract function get_title();
  /**
   * Get object name
   */
  public abstract function get_name();
  /**
   * Build from db object or template
   */
  public static function build_object($object) {
    return new Notifications_Entity($object);
  }
  /**
   * User access function
   */
  public abstract function user_access($account, $op = 'view');
  /**
   * Get information from notifications hooks
   */
  public static function type_info($type = NULL, $property = NULL, $default = NULL) {
    // Creating static abstract functions produces warnings.
  }
  /**
   * Get information for specific instance.
   *
   * This is useful for properties that can be overridden with alter hooks
   */
  public function get_info($property = NULL, $default = NULL) {
    return $this->type_info($this->type, $property, $default);
  }
}

/**
 * Interface for Notification objects.
 */
interface Notifications_ObjectInterface {
  /**
   * Load related object or data
   */
  public static function object_load($value);
  /**
   * Get name for object
   */
  public static function object_name($object);
  /**
   * Map object to value (key)
   */
  public static function object_value($object);
}

/**
 * Wrapper for Drupal objects
 *
 * This will be a wrapper for Drupal objects passed around and used as parameters, with some advantages:
 * - All the objects have a 'type' property
 * - When serialized, the object itself won't be serialized
 */
abstract class Notifications_Object implements Notifications_ObjectInterface {
  // Object type
  public $type;
  public $value;
  public $name;
  protected $object;
  protected $fields;

  /**
   * Get object type title
   */
  public function get_title() {
    return $this->get_property('title', '');
  }

  /**
   * Get object name (user name, node title, etc)
   */
  public function get_name() {
    if (!empty($this->name)) {
      return $this->name;
    }
    elseif ($this->get_object()) {
      return $this->object_name($this->get_object());
    }
    else {
      return '';
    }
  }

  /**
   * Get object token type
   */
  public function get_token_type() {
    return $this->type;
  }

  /**
   * Build from type, value (can be a plain value or a Drupal object)
   */
  public static function build($type, $value = NULL) {
    $class = self::type_info($type, 'class', 'Notifications_Drupal_Object');
    $object = new $class(array('type' => $type));
    if (isset($value)) {
      $object->set_value($value);
    }
    return $object;
  }
  /**
   * Set value (reset object)
   */
  public function set_value($value) {
    if (is_object($value)) {
      $this->set_object($value);
    }
    else {
      $this->value = $value;
      $this->object = NULL;
    }
    return $this;
  }
  /**
   * Get object value
   */
  public function get_value() {
    return isset($this->value) ? $this->value : NULL;
  }
  /**
   * Set object
   */
  public function set_object($object) {
    $this->object = $object;
    $this->value = $this->object_value($object);
    return $this;
  }
  /**
   * Get object unique key as string. The default will be 'type:value'
   */
  public function index() {
    return $this->type . ':' . (isset($this->value) ? $this->value : 'empty');
  }
  /**
   * Get object value
   */
  public function get_object() {
    if (!isset($this->object)) {
      $object = isset($this->value) ? $this->object_load($this->value) : NULL;
      $this->object = $object ? $object : FALSE;
    }
    return $this->object;
  }

  /**
   * Check user access. By default it will be true if object can be loaded
   */
  function user_access($account) {
    return (bool)$this->get_object();
  }
  /**
   * Get fields for this object type
   */
  function get_fields() {
    if (!isset($this->fields)) {
      $this->fields = array();
      if ($object = $this->get_object()) {
        // As this does an array_merge_recursive() we get grouped field => array(value1, value2..)
        $fields = module_invoke_all('notifications_object_' . $this->type, 'fields', $object);
        // Now we just need to filter out duplicate values
        foreach ($fields as $field) {
          $this->fields[$field->index()] = $field;
        }
      }
    }
    return $this->fields;
  }

  /**
   * Get list of possible and existing subscriptions for user/object
   *
   * @return Notifications_Subscription_List
   */
  function user_subscriptions($account = NULL) {
    $account = $account ? $account : $GLOBALS['user'];
    $subscriptions = $this->subscribe_options($account);
    $subscriptions->build_instances(array('uid' => $account->uid));
    $subscriptions->set_user($account);
    return $subscriptions;
  }

  /**
   * Get subscription options for object, account. Only enabled subscription types
   *
   * We pass on the user account so we cah check permissions on the fly and save lots of objects
   *
   * @return Notifications_Subscription_List
   */
  function subscribe_options($account) {
    $subscriptions = new Notifications_Subscription_List();
    if ($options = $this->invoke_all('subscriptions', $account)) {
      $subscriptions->add($options);
    }
    return $subscriptions;
  }
  /**
   * Get subscription types for this object
   */
  public function subscription_types() {
    return $this->invoke_all('subscription types');
  }

  /**
   * Get object type property
   */
  protected function get_property($name, $default = NULL) {
    return $this->type_info($this->type, $name, $default);
  }
  /**
   * Get object type information
   */
  public static function type_info($type = NULL, $property = NULL, $default = NULL) {
    return notifications_info('object types', $type, $property, $default);
  }
  /**
   * Run module_invoke_all('notifications_object_[type]') with this (Drupal) object
   */
  protected function invoke_all($op, $param = NULL) {
    return module_invoke_all('notifications_object_' . $this->type, $op, $this->get_object(), $param);
  }

  /**
   * PHP Magic. Regurn object properties to be serialized
   */
  public function __sleep() {
    return array('type', 'value', 'name');
  }

}

/**
 * Wrapper for generic Drupal objects
 *
 * This basically implements abstract methods with some defaults
 *
 * @todo Get some information from entities
 */
class Notifications_Drupal_Object extends Notifications_Object {
  public function get_title() {
    return t('Object');
  }
  /**
   * Load related object or data
   */
  public static function object_load($value) {
    return NULL;
  }

  /**
   * Get name for object
   */
  public static function object_name($object) {
    return t('unknown');
  }

  /**
   * Map object to value (key)
   */
  public static function object_value($object) {
    return NULL;
  }
}

/**
 * Node objects
 */
class Notifications_Node extends Notifications_Drupal_Object {
  public $type = 'node';
  /**
   * Load related object or data
   */
  public static function object_load($value) {
    return node_load($value);
  }
  /**
   * Get object name, unfiltered string
   */
  public static function object_name($node) {
    return $node->title;
  }
  /**
   * Get object key
   */
  public static function object_value($node) {
    return $node->nid;
  }
  /**
   * Check user access
   */
  function user_access($account) {
    if ($node = $this->get_object()) {
      return node_access('view', $node, $account);
    }
  }
}

/**
 * User objects
 */
class Notifications_User extends Notifications_Drupal_Object {
  public $type = 'user';
  /**
   * Load related object or data
   */
  public static function object_load($value) {
    return user_load($value);
  }
  /**
   * Get object name, unfiltered string
   */
  public static function object_name($object) {
    return $object->name;
  }
  /**
   * Get object key
   */
  public static function object_value($user) {
    return $user->uid;
  }
  /**
   * Check user access
   */
  function user_access($account) {
    $user = $this->get_object();
    return $user && $user->uid &&
    (
      // Always let users view their own profile.
      ($user->uid == $account->uid) ||
      // Administrators can view all accounts.
      user_access('administer users', $account) ||
      // The user is not blocked and logged in at least once.
      ($user->access && $user->status && user_access('access user profiles', $account))
    );
  }
}
