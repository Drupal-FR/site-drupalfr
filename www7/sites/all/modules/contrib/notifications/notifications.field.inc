<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Base class for Notifications fields
 */
abstract class Notifications_Field {
  // Subscription id
  public $sid;
  // Field type
  public $type;
  // Value
  public $value;
  // Index for subscription
  public $position;
  // Object name
  public $name;
  // Object type, generic one
  public $object_type = 'object';
  // Linked object
  protected $object;
  // Data type
  protected $data_type = 'int';
  // Subscription object this field belongs to
  protected $subscription;

  /**
   * Constructor
   */
  public function __construct($template = array()) {
    foreach ((array)$template as $field => $value) {
      $this->$field = $value;
    }
  }
  /**
   * Quick build
   */
  public static function build($type, $value) {
    return self::build_type($type, array('value' => $value));
  }

  /**
   * Build field instance
   */
  public static function build_type($type, $template = NULL) {
    if ($class = notifications_field_type($type, 'class')) {
      return new $class($template);
    }
    else {
      // Unknown field type, let's build something
      $field = new Notifications_Field_Default($template);
      $field->type = $type;
      return $field;
    }
  }
  /**
   * Build from db object
   */
  public static function build_object($object) {
    return self::build_type($object->type, $object);
  }
  /**
   * Get field value name
   */
  function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif ($this->get_value()) {
      return $this->name = $this->get_object()->get_name();
    }
    else {
      return t('none');
    }
  }
  /**
   * Get field value
   */
  function get_value() {
    return isset($this->value) ? $this->value : NULL;
  }
  /**
   * Get title for field
   */
  public function get_title() {
    return $this->get_property('title', '');
  }
  /**
   * Get description
   */
  public abstract function get_description();


  /**
   * Get link if this field is linked to an object
   */
  function get_link($options = array()) {
    if (($path = $this->get_path()) && ($name = $this->get_name())) {
      return l($name, $path, $options);
    }
    else {
      // Fallback if we have no link, will be plaintext name
      return check_plain($this->get_name());
    }
  }
  /**
   * Get system path
   */
  function get_path() {
    return '';
  }
  /**
   * Get related Notifications object
   */
  function get_object() {
    if (!isset($this->object)) {
      $this->object = notifications_object($this->object_type, $this->value);
    }
    return $this->object;
  }
  /**
   * Get related Drupal object
   */
  function drupal_object() {
    return $this->get_object()->get_object();
  }
  /**
   * Get query condition for current value
   */
  function get_query_condition($alias = 'f') {
    if (isset($this->value)) {
      return $this->get_value_condition($this->value, $alias);
    }
  }
  /**
   * Get value/s from object
   */
  function object_value($object) {
    if ($object->type == $this->object_type) {
      return $object->get_value();
    }
  }
  /**
   * Get query condition for a given value
   *
   * @param $value
   *   Single value or array of values for this field
   * @param $alias
   *   Alias of the notifications_subscription_fields table
   */
  function get_value_condition($value, $alias = 'f') {
    $and = db_and();
    $and->condition($alias . '.type', $this->type);
    if (isset($this->position)) {
      $and->condition($alias . '.position', $this->position);
    }
    if ($this->data_type == 'int') {
      $value = is_array($value) ? array_map('intval', $value) : (int)$value;
      $and->condition($alias . '.intval', $value);
    }
    else {
      $and->condition($alias . '.value', $value);
    }
    return $and;
  }
  /**
   * Format title and value
   */
  function format($format = NOTIFICATIONS_FORMAT_HTML) {
    $items = array(
      $this->get_title(),
      $this->format_value($format),
    );
    return notifications_format_items($items, $format);
  }
  /**
   * Format value
   */
  function format_value($format = NOTIFICATIONS_FORMAT_HTML) {
    if ($format & NOTIFICATIONS_FORMAT_HTML) {
      return $this->get_path() ? $this->get_link() : check_plain($this->get_name());
    }
    else {
      return check_plain($this->get_name());
    }
  }

  /**
   * Check user access
   */
  function user_access($account) {
    return $this->get_object()->user_access($account);
  }

  /**
   * Get unique index for this field
   */
  function index() {
    return $this->object_type . ':' . $this->type . ':' . (isset($this->value) ? $this->value : '');
  }

  /**
   * Set value for this field, update related properties
   */
  function set_value($value, $validate = FALSE) {
    if (!$validate || $this->valid_value($value)) {
      $this->value = $value;
      $this->name = NULL;
      $this->object = NULL;
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  /**
   * Build a field object from submitted values
   */
  public static function build_from_value($data, $type = NULL, $index = NULL) {
    if (is_object($data)) {
      if (!$type || $data->type == $type) {
        $field = $data;
      }
    }
    elseif (is_array($data)) {
      if (isset($data['type']) && (!$type || $type == $data['type'])) {
        $field = self::build_type($data['type']);
        if (isset($data['value'])) {
          $value = $field->parse_value($data['value']);
          $field->set_value($value, TRUE); // Set value with previous validation
        }
      }
    }
    elseif ($type) {
      $field = self::build_type($type);
      $value = $field->parse_value($data);
      $field->set_value($value, TRUE); // Set value with previous validation
    }
    if (!empty($field)) {
      $field->position = $index;
      return $field;
    }
  }
  /**
   * Set subscription
   */
  function set_subscription($subscription) {
    $this->subscription = $subscription;
    $this->sid = !empty($subscription->sid) ? $subscription->sid : NULL;
  }
  /**
   * Check if the field has a valid value or the parameter is a valid value
   */
  function check_value() {
    return isset($this->value) ? $this->valid_value($this->value) : FALSE;
  }
  /**
   * Parse value from form submission
   */
  function parse_value($value) {
    switch ($this->data_type) {
      case 'int':
        return (int)$value;
      case 'float':
        return (float)$value;
      case 'string':
        return (string)$value;
      default:
        return $value;
    }
  }
  /**
   * Check if this is a valid value for this field/**
   */
  function valid_value($value = NULL) {
    return $this->validate_value($value, $this->data_type);
  }

  /**
   * Save to db
   */
  function save() {
    // We may have a new sid for this subscription object
    if (!empty($this->subscription)) {
      $this->sid = $this->subscription->sid;
    }
    // The int value must be set for drupal_write_record
    $this->intval = (int)$this->get_value();
    return drupal_write_record('notifications_subscription_fields', $this);
  }
  /**
   * Load multiple fields
   */
  public static function load_multiple($conditions) {
    $fields = array();
    $query = db_select('notifications_subscription_fields', 'f')->fields('f');
    foreach ($conditions as $key => $value) {
      $query->condition($key, $value);
    }
    foreach ($query->execute()->fetchAll() as $field) {
      $fields[] = self::build_object($field);
    }
    return $fields;
  }
  /**
   * Check if the value is valid for this field has a valid value
   *
   * Was: notifications_field_valid_value($value, $type = NULL)
   */
  static function validate_value($value, $data_type = NULL) {
    // A numeric value of zero is possible too, that's why the is_numeric()
    if (!is_numeric($value) && empty($value)) {
      // The field has no value at all, no go
      return FALSE;
    }
    elseif ($data_type) {
      // We want aditional field type validation
      switch ($data_type) {
        case 'int':
          // @todo Better integer validation, is_int not working for strings
          return is_numeric($value);
        case 'float':
          return is_numeric($value);
        case 'string':
        default:
          return is_string($value);
      }
    }
    else {
      return TRUE;
    }
  }

  /**
   * Build a form element to edit this field
   *
   * Was: notifications_field_form_element($type, $value, $subscription = NULL, $title = FALSE, $required = FALSE, $size = 40)
   */
  function element_edit($element = array()) {
    $element += array(
      '#title' => $this->get_title(),
      '#type' => 'textfield',
      '#default_value' => $this->get_value(),
      '#required' => TRUE,
      '#size' => 40,
    );
    return $element;
  }

  /**
   * Build a form element to display this field
   */
  function element_info($element = array()) {
    $element += array(
      '#type' => 'item',
      '#title' => $this->get_title(),
      '#markup' => $this->get_link(),
    );
    return $element;
  }
  /**
   * Get field type property
   */
  protected function get_property($name, $default = NULL) {
    return $this->type_info($this->type, $name, $default);
  }
  /**
   * Get field type information
   */
  public static function type_info($type = NULL, $property = NULL, $default = NULL) {
    return notifications_info('field types', $type, $property, $default);
  }
  /**
   * PHP Magic. Regurn object properties to be serialized
   */
  public function __sleep() {
    return array('type', 'value', 'position', 'name');
  }
}

/**
 * Default field when we don't have information
 */
class Notifications_Field_Default extends Notifications_Field {
  /**
   * Get title for field
   */
  function get_title() {
    return t('Field');
  }
  /**
   * Get description
   */
  function get_description() {
    return t('Notifications field');
  }
}
/**
 * Field with autocomplete values
 */
abstract class Notifications_Field_Autocomplete extends Notifications_Field {
  /**
   * Format value for autocomplete
   */
  function autocomplete_value() {
    return $this->get_name();
  }
  /**
   * Parse value from autocomplete
   */
  public abstract function autocomplete_parse($string);
  /**
   * Get autocomplete path
   */
  public abstract function autocomplete_path();

  /**
   * Parsing values may be different for these ones
   */
  function parse_value($value) {
    if (is_string($value) && ($parsed = $this->autocomplete_parse($value))) {
      return $parsed;
    }
    else {
      return parent::parse_value($value);
    }
  }
  /**
   * Build a form element to edit this field
   */
  function element_edit($element = array()) {
    $element += parent::element_edit($element);
    $element['#default_value'] = $this->autocomplete_value();
    $element['#autocomplete_path'] = $this->autocomplete_path();
    return $element;
  }

}

/**
 * Field with selectable values
 */
abstract class Notifications_Field_Select extends Notifications_Field {
  public abstract function select_options();
  /**
   * Build a form element to edit this field
   */
  function element_edit($element = array()) {
    $element += array('#required' => TRUE, '#title' => $this->get_title());
    $options = $this->select_options();
    // If not required or not current value, add an empty value at the beginning
    if (empty($element['#required'])) {
      $options = array('' => '') + $options;
    }
    $element += array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->get_value(),
    );
    return $element;
  }
  /**
   * Translate name into options
   */
  function get_name() {
    if (($value = $this->get_value()) && ($options = $this->select_options()) && isset($options[$value])) {
      return $options[$value];
    }
  }
  /**
   * Check valid value
   */
  function valid_value($value = NULL) {
    return ($options = $this->select_options()) && isset($options[$value]);
  }
}

/**
 * Node fields
 */
class Notifications_Node_Field extends Notifications_Field_Autocomplete {
  public $type = 'node:nid';
  public $field = 'nid';
  public $object_type = 'node';

  /**
   * Get field name
   */
  function get_title() {
    return t('Node');
  }
  /**
   * Get description
   */
  function get_description() {
    return t('Node');
  }

  /**
   * Get system path
   */
  function get_path() {
    return isset($this->value) ? 'node/' . $this->value : '';
  }

  /**
   * Get autocomplete path
   */
  function autocomplete_path() {
    return 'notifications/autocomplete/node/title';
  }

  /**
   * Parse value from autocomplete
   */
  function autocomplete_parse($string, $field = NULL) {
    module_load_include('inc', 'notifications', 'includes/node');
    return notifications_node_title2nid($string, $field);
  }
  /**
   * Format value for autocomplete
   */
  function autocomplete_value() {
    $node = $this->drupal_object();
    return $node ? check_plain($node->title) . ' [nid:' . $node->nid . ']' : '';
  }
  /**
   * Get query condition for nodes
   */
  function get_object_condition($object) {
    if ($object->type == 'node' && isset($object->value)) {
      return $this->get_value_condition($object->value);
    }
  }
}

/**
 * Drupal user
 */
class Notifications_User_Field extends Notifications_Field_Autocomplete {
  public $type = 'user:uid';
  public $object_type = 'user';
  /**
   * Get title for field
   */
  function get_title() {
    return t('User');
  }
  /**
   * Get description
   */
  function get_description() {
    return t('User name');
  }
   /**
   * Get autocomplete path
   */
  function autocomplete_path() {
    return 'user/autocomplete';
  }

  /**
   * Parse value from autocomplete
   */
  function autocomplete_parse($string) {
    if ($user = user_load_by_name($string)) {
      return $user->uid;
    }
  }

  function format_name($format) {
    return messaging_user_format_name($this->value);
  }
}
