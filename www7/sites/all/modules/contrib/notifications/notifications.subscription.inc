<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 *
 * Hidden variables
 * - notifications_frontpage - Where to redirect after notifications operations, defaults to site_frontpage
 */

/**
 * Common base for subscription type and subscription instance
 */
class Notifications_Subscription extends Notifications_Entity {
    // Format as plaintext. Note it evaluates to false.
  const FORMAT_PLAIN = 0;
  // Format as html. Note it evaluates to true
  const FORMAT_HTML = 1;
  // Format inline, as a string of csv
  const FORMAT_INLINE = 2;
  // Format as HTML table (4 +1)
  const FORMAT_TABLE = 5;
  // Format as item list (8 + 2(inline) + 1 (html))
  const FORMAT_LIST = 10;

  // Blocked subscriptions, for blocked users
  const STATUS_BLOCKED = 0;
  // Enabled ones, will produce notifications
  const STATUS_ACTIVE = 1;
  // Temporarily disabled ones, maybe user on holidays
  const STATUS_INACTIVE = 2;
  // Disabled because the subscription type is disabled
  const STATUS_DISABLED = -1;
  // Disabled because sending method is disabled, send failed,
  const STATUS_NOSEND = -2;

  // Scheduled notification only, special send interval
  const SEND_SCHEDULED = -1;

  // Unique subscription id
  public $sid = 0;
  // Destination id
  public $mdid;
  // Subscription type
  public $type;
  // User that owns this subscription
  public $uid;
  // Language code
  public $language;
  // Event that triggers this subscription
  public $event_type;
  // Instance fields, depend on subscription type
  public $fields;
  // Number of conditions that must be met
  public $conditions = 0;
  // Values to pass on to the queue for composition
  public $send_interval;
  public $send_method;
  public $cron = 1;
  public $module = 'notifications';
  // Subscription status, defaults to active
  public $status = 1;
  // The address for the destination (Unused, just for logging. To be obsoleted)
  public $destination = '';
  // Timestamps
  public $created;
  public $updated;
  // Name for this subscription that will be quite dependent on context
  protected $name;
  // Mark if incomplete loading of objects
  protected $incomplete;
  // Mark when editable
  protected $editable;
  // Mark when prepared to be instantiated (saved to db)
  protected $prepared;
  // Temporary error message, to display when validation fails
  public $error_message;
  // Subscription type. Array of field types
  public $field_types;
  // Subscription type. Array of field values
  public $field_values;
  // Subscription type. Array of object types
  public $object_types;

  /**
   * Build from db object or template.
   *
   * @param $subscription
   *   Notifications_Subscription object, or generic object
   * @return Notifications_Subscription
   *   Object of the right class of Notifications_Subscription
   */
  public static function build_object($subscription) {
    if (is_object($subscription) && is_a($subscription, 'Notifications_Subscription')) {
      return $subscription;
    }
    else {
      $subscription = (object)$subscription;
      $class = self::type_info($subscription->type, 'class', 'Notifications_Subscription');
      return new $class($subscription);
    }
  }
  /**
   * Build from subscription type
   */
  public static function build_type($type) {
    $class = self::type_info($type, 'class', 'Notifications_Subscription');
    $subscription = new $class();
    $subscription->type = $type;
    return $subscription;
  }
  /**
   * Build subscription instance
   */
  public static function build_instance($type) {
    return self::build_type($type)->instance();
  }
  /**
   * Load from db
   *
   * @param $sid
   *   Subscription id
   * @param $full
   *   Whether to load all fields in the same operation
   */
  public static function load($sid) {
    $sids = !empty($sid) ? array($sid) : array();
    $subscriptions = entity_load('notifications_subscription', $sids);
    return reset($subscriptions);
  }
  /**
   * Load multiple from db
   *
   * @see select_subscriptions
   */
  public static function load_multiple($conditions, $fields = array(), $limit = FALSE) {
    $query = self::select_subscriptions($conditions, $fields, $limit);
    if ($sids = $query->execute()->fetchCol()) {
      return entity_load('notifications_subscription', $sids);
    }
    else {
      return array();
    }
  }
  /**
   * Load single subscription from db
   */
  public static function load_single($conditions, $fields = array(), $limit = FALSE) {
    $query = self::select_subscriptions($conditions, $fields, $limit);
    $query->range(0, 1);
    if ($sid = $query->execute()->fetchField()) {
      return self::load($sid);
    }
    else {
      return array();
    }
  }
  /**
   * Get all existing instances form the db
   */
  public function load_instances($params) {
    $params += array('type' => $this->type);
    // Add all the fields that have a value set (type and instance fields)
    $fields = $this->get_fields(TRUE);
    return $this->load_multiple($params, $fields);
  }
  /**
   * Get single instance form the db
   */
  public function load_single_instance($params) {
    $params += array('type' => $this->type);
    // Add all the fields that have a value set (type and instance fields)
    $fields = $this->get_fields(TRUE);
    return $this->load_single($params, $fields);
  }
  /**
   * Get result from last operation
   */
  public function get_result() {
    return isset($this->result) ? $this->result : TRUE;
  }
  /**
   * Get type fields, the ones that have a value set for this type
   */
  function get_type_fields() {
    return $this->type_fields(TRUE);
  }
  /**
   * Get editable fields, the ones that are not set for type
   */
  function get_editable_fields() {
    return $this->get_instance_fields();
  }
  /**
   * Get unique field per type.
   *
   * A subscription may have more than one field of the same type
   */
  function get_unique_fields() {
    $fields = array();
    foreach ($this->get_fields() as $field) {
      if (!isset($fields[$field->type])) {
        $fields[$field->type] = clone $field;
        unset($fields[$field->type]->position);
      }
    }
    return $fields;
  }
  /**
   * Map field to its right position in this subscription
   */
  function map_field($field) {
    // First we try with instance fields
    foreach ($this->get_instance_fields() as $instance) {
      if ($instance->type == $field->type) {
        return $instance->position;
      }
    }
  }
  /**
   * Get subscription type data
   *
   * @todo replace by get_info
   */
  function get_type($property = NULL, $default = NULL) {
    return empty($this->type) ? NULL : notifications_subscription_type($this->type, $property, $default);
  }

  /**
   * Get default link options
   */
  protected function get_link_options($operation, $options = array()) {
    $options += array('query' => array(), 'destination' => TRUE);
    switch ($operation) {
      case 'subscribe':
        $options += array(
          'base_path' => 'notifications/subscribe',
          'skip_confirmation' => variable_get('notifications_option_subscribe_links', 0)
        );
        // Add instance fields to the query string indexed by position
        foreach ($this->get_fields(TRUE) as $field) {
          $options['query'][$field->position] = $field->value;
        }
        break;
      case 'unsubscribe':
        $options += array(
          'base_path' => 'notifications/unsubscribe',
          'skip_confirmation' => variable_get('notifications_option_unsubscribe_links', 0)
        );
        break;
      default:
        $options += array(
          'base_path' => 'notifications/subscription/' . ($this->is_stored() ? $this->sid . '/' : '') . $operation
        );
        break;
    }
    // Translate destination option into actual destination
    if (!empty($options['destination'])) {
      $destination = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : (isset($_GET['q']) ? $_GET['q'] : '');
      if ($destination) {
        $options['query'] += array('destination' => $destination);
      }
    }
    return $options;
  }
  /**
   * Get default link type (susbscribe || unsubscribe)
   *
   * 'subscription' type will translate on subscribe/unsubscribe depending on current status
   *
   * @param $type
   *   Link type.
   */
  protected function get_link_type($type = NULL) {
    if (!$type || $type == 'subscription') {
      return $this->is_stored() ? 'unsubscribe' : 'subscribe';
    }
    else {
      return $type;
    }
  }
  /**
   * Subscribe links are ok for instances and types
   */
  function get_link($type = NULL, $options = array()) {
    $link = $this->element_link($type, $options);
    return drupal_render($link);
  }

  /**
   * Get URL for messages (absolute and signed)
   */
  public function get_url($operation, $options = array()) {
    $options += array('absolute' => TRUE, 'signed' => TRUE);
    return $this->get_path($operation, $options);
  }

  /**
   * Get link element
   */
  public function element_link($operation = NULL, $options = array()) {
    $type = $this->get_link_type($operation);
    $options += $this->get_link_options($type, $options);
    $path = $this->get_path($type, $options);
    return array(
        '#type' => 'link',
        '#title' => $this->get_link_text($type),
        '#href'  => $path,
        '#options' => notifications_url_options($path, $options) + array('html' => TRUE),
        '#description' => $this->get_description(),
      );
  }

  /**
   * Get subscription type information
   */
  public static function type_info($type = NULL, $property = NULL, $default = NULL) {
    return notifications_subscription_type($type, $property, $default);
  }

  /**
   * Filter out fields that have or don't have a value set
   *
   * @param $fields
   *   Array of field objects to be filtered
   * @param $filter
   *   If TRUE will return fields with value set. If FALSE, fields with value not set
   */
  public static function filter_fields($fields, $filter = NULL) {
    if (isset($filter)) {
      foreach ($fields as $key => $field) {
        if ($filter && !isset($field->value) || !$filter && isset($field->value)) {
          unset($fields[$key]);
        }
      }
    }
    return $fields;
  }

  /**
   * Save fields from form submission
   */
  public function set_properties($values, $fields) {
    foreach ($fields as $key) {
      if (isset($values[$key])) {
        switch ($key) {
          case 'fields':
            if (!empty($values['parsed_fields'])) {
              $this->set_fields($values['parsed_fields']);
            }
            else {
              $this->set_fields($values['fields']);
            }
            break;
          case 'destination':
            $this->set_destination($values[$key]);
            break;
          default:
            $this->$key = $values[$key];
            break;
        }
      }
    }
  }

  /**
   * Validate form
   */
  public function form_validate($form, &$form_state) {
    $this->set_properties_from_submission($form, $form_state);
    return $this->validate_submission($form, $form_state);
  }
  /**
   * Process form submission
   */
  public function form_submit($form, &$form_state) {
    $triggering_element = isset($form_state['triggering_element']['#name']) ? $form_state['triggering_element']['#name'] : '';
    switch ($triggering_element) {
      case 'cancel':
      case 'delete':
        $operation = $triggering_element;
        break;
      default:
        $operation = $form_state['values']['operation'];
        $this->set_properties_from_submission($form, $form_state);
        break;
    }
    return $this->form_operation($operation, $form, $form_state);
  }

  /**
   * Run form operation
   */
  protected function form_operation($operation, $form, &$form_state) {
    switch ($operation) {
      case 'cancel':
        drupal_set_message(t('Operation cancelled.'), 'warning');
        $this->result = FALSE;
        break;

      case 'subscribe':
      case 'add':
      case 'edit':
        $this->result = $this->save();
        switch ($this->result) {
          case SAVED_NEW:
            drupal_set_message(t('The subscription has been created.'));
            break;
          case SAVED_UPDATED;
            drupal_set_message(t('The subscription has been updated.'));
            break;
          default:
            drupal_set_message(t('Error saving the subscription.'), 'error');
        }
        if (!empty($this->sid)) {
          $form_state['redirect'] = 'notifications/subscription/' . $this->sid;
        }
        break;

      case 'unsubscribe':
      case 'delete':
        $this->result = $this->delete();
        if ($this->result) {
          drupal_set_message(t('The subscription has been removed.'));
        }
        else {
          drupal_set_message(t('Error deleting the subscription.'), 'error');
        }
        $form_state['redirect'] = variable_get('notifications_frontpage', '<front>');
        break;

      default:
        drupal_set_message(t('Operation not implemented yet.'), 'warning');
        $this->result = FALSE;
        break;
    }
  }
  /**
   * Get form for this subscription
   *
   * @return $form
   */
  public function get_form($operation, $form, &$form_state) {
    $form['subscription'] = array('#type' => 'value', '#value' => $this);
    $form['operation'] = array('#type' => 'value', '#value' => $operation);
    switch ($operation) {
      case 'subscribe':
        // This case will be editable if not all fields are predefined
        $editable = count($this->get_fields()) > count($this->get_fields(TRUE));
        break;
      case 'edit':
        $editable = TRUE;
        break;
      case 'unsubscribe':
      case 'delete':
      default:
        $editable = FALSE;
        break;
    }
    if ($editable) {
      $form = $this->form_edit($operation, $form, $form_state);
    }
    else {
      $form = $this->form_view($operation, $form, $form_state);
    }
    $form['controls'] = $this->element_operation($operation);
    return $form;
  }

  /**
   * Get edit form for this subscription
   */
  public function form_edit($operation, $form, &$form_state) {
    $form['subscription_info'] = $this->element_info();
    $form['send_method'] = $this->element_send_method();
    $form['send_interval'] = $this->element_send_interval();
    if ($fields = $this->get_editable_fields()) {
      $form['subscription_fields'] = $this->element_fields_edit($fields);
    }
    return $form;
  }
  /**
   * Get view only form for this subscription
   */
  public function form_view($operation, $form, &$form_state) {
    $form['subscription_info'] = $this->element_info();
    $form['send_method'] = $this->element_send_method();
    $form['send_interval'] = $this->element_send_interval();
    if ($fields = $this->get_fields(TRUE)) {
      $form['subscription_fields'] = $this->element_fields_info($fields);
    }
    return $form;
  }
  /**
   * Get subscription form info element
   */
  public function element_info() {
    $elements = array(
      '#type' => 'fieldset',
      '#title' => $this->get_name(),
    );
    $elements['description']['#markup'] = $this->get_description();
    // We may have fixed fields or not
    if ($fields = $this->get_type_fields()) {
      $elements['fields'] = $this->element_fields_info($fields);
    }
    return $elements;
  }
  /**
   * Send method: form element
   */
  public function element_send_method($element = array()) {
    $send_methods = notifications_send_methods($this->get_user());
    if (count($send_methods) > 1) {
      $element += array(
        '#title' => t('Send method'),
        '#type' => 'select',
        '#options' => $send_methods,
        '#default_value' => $this->send_method,
      );
    }
    return $element;
  }
  /**
   * Form element: send interval
   */
  public function element_send_interval($element = array()) {
    // For scheduled notifications we don't give any option
    if (isset($this->send_interval) && $this->send_interval == self::SEND_SCHEDULED) {
      return $element;
    }
    $send_intervals = notifications_send_intervals($this->get_user());
    if (count($send_intervals) > 1) {
      $element += array(
        '#title' => t('Send interval'),
        '#type' => 'select',
        '#options' => $send_intervals,
        '#default_value' => $this->send_interval,
      );
    }
    return $element;
  }
  /**
   * Get all fields (editable and not editable)
   */
  public function element_fields() {
    $element = array();
    if ($type_fields = $this->get_type_fields()) {
      $elements['info'] = $this->element_fields_info($fields);
    }
    if ($edit_fields = $this->get_editable_fields()) {
      $elements['edit'] = $this->element_fields_info($fields);
    }
    return $element;
  }
  /**
   * Get elements for information fields (non editable)
   */
  public function element_fields_info($fields) {
    $elements = array();
    foreach ($fields as $field) {
      $elements[$field->position] = $field->element_info();
    }
    return $elements;
  }
  /**
   * Get form elements for editable fields
   */
  public function element_fields_edit($fields) {
    $elements = array('#tree' => TRUE);
    foreach ($fields as $field) {
      $elements[$field->position] = $field->element_edit();
    }
    return $elements;
  }
  /**
   * Get operation buttons for form
   */
  public function element_operation($operation) {
    $elements = array('#type' => 'fieldset', '#weight' => 100);
    switch ($operation) {
      case 'view';
        return; // no buttons.
      case 'subscribe':
      case 'add':
        $elements['subscribe'] = array(
          '#type' => 'submit',
          '#name' => 'subscribe',
          '#value' => t('Create subscription'),
        );
        break;
      case 'edit':
        $elements['save'] = array(
          '#type' => 'submit',
          '#name' => 'save',
          '#value' => t('Save subscription'),
        );
        // no break
      case 'delete':
      case 'unsubscribe':
        $elements['delete'] = array(
          '#type' => 'submit',
          '#name' => 'delete',
          '#value' => t('Delete subscription'),
        );
        break;
    }
    $elements['cancel'] = array(
      '#type' => 'submit',
      '#name' => 'cancel',
      '#value' => t('Cancel'),
    );
    return $elements;
  }

  /**
   * Build subscription instance from form submission
   */
  public static function build_from_submission($form, &$form_state) {
    // The subscription object should be here, it may be a subscription type
    $subscription = $form_state['values']['subscription'];
    return $subscription->instance();
  }
  /**
   * Set instance properties from form submission
   */
  protected function set_properties_from_submission($form, &$form_state) {
    // Process regular values and validate
    $this->set_properties_from_values($form_state['values'], TRUE);
    // There may be optional fields to add
    if ($fields = $this->build_fields_from_submission($form, $form_state)) {
      $this->set_fields($fields);
    }
    return $this;
  }
  /**
   * Set instance properties from url parameters
   */
  public function set_properties_from_url($query = NULL) {
    $params = isset($query) ? $query : $_GET;
    return $this
      ->set_properties_from_values($params, TRUE)
      ->set_fields_from_values($params);
  }
  /**
   * Set instance properties from array of values
   */
  public function set_properties_from_values($values, $validate = TRUE) {
      // Process some other fields that may be there
    foreach (array('send_method', 'send_interval') as $field) {
      if (isset($values[$field])) {
        $this->set_property($field, $values[$field]);
      }
    }
    return $this;
  }
  /**
   * Build instance fields form URL parameters
   *
   * @param $values
   *   Array of field values indexed by field position
   */
  protected function set_fields_from_values($values = array()) {
    $this->result = TRUE;
    // Set field values, they come in the query string indexed by field position
    foreach ($this->get_editable_fields() as $field) {
      if (isset($values[$field->position])) {
        // Validate (TRUE) and set value
        $this->result = $this->result && $field->set_value($values[$field->position], TRUE);
      }
    }
    return $this;
  }
  /**
   * Set property, possibly from form submission
   */
  protected function set_property($name, $value, $validate = TRUE) {
    $this->$name = $value;
  }
  /**
   * Build submitted fields (match them with this subscription type fields)
   */
  protected function build_fields_from_submission($form, &$form_state) {
    $fields = array();
    if (!empty($form_state['values']['subscription_fields'])) {
      $field_values = $form_state['values']['subscription_fields'];
      // In this case we have known fields that are always indexed by position
      foreach ($this->get_editable_fields() as $field) {
        if (isset($field_values[$field->position])) {
          $build = Notifications_Field::build_from_value($field_values[$field->position], $field->type, $field->position);
          if ($build) {
            $fields[$field->position] = $build;
          }
        }
      }
    }
    return $fields;
  }
  /**
  * Validate form submission
  */
  public static function validate_submission($form, &$form_state) {
    // @todo
  }

  /**
   * Build subscriptions for array of objects and user account.
   *
   * @return Notifications_Subscription_List
   */
  static function object_subscriptions($objects, $account) {
    $subscriptions = new Notifications_Subscription_List();
    foreach ($objects as $object) {
      if ($more = $object->subscribe_options($account)) {
        $subscriptions->add($more);
      }
    }
    return $subscriptions;
  }

  /**
   * Get event conditions OR'd
   *
   * These will look like
   *
   * s.type = 'node' AND (field_condition1 OR field_condition2)
   * s.type = 'term' AND (field_condition1)
   */
  function event_conditions($event) {
    $condition = $this->object_conditions($event->get_objects());
    if ($condition && $condition->count()) {
      // Finally, add subscription type
      return db_and()
        ->condition('s.type', $this->type)
        ->condition($condition);
    }
  }

  /**
   * Get OR'd field conditions for this subscription type and object
   *
   * Conditions will look like
   *   f.type = 'nid' AND f.value = 100
   *   f.type = 'term' AND f.value IN (1, 2, 3, 4)
   **/
  function object_conditions($objects) {
    $add = db_or();
    foreach ($objects as $object) {
      foreach ($this->get_unique_fields() as $field) {
        if ($value = $field->object_value($object)) {
          $condition = $field->get_value_condition($value);
          $add->condition($condition);
        }
      }
    }
    return $add;
  }

  /**
   * Build a form element for a field
   *
   * A subscription type can override its form elements
   */
  function field_form_element($field, $element = array()) {
    return $field->form_element($title, $element);
  }

  /**
   * Run module_invoke_all('notifications_subscription') with this object
   */
  public function invoke_all($op, $param = NULL) {
    return module_invoke_all('notifications_subscription', $op, $this, $param);
  }
  /**
   * Status list
   */
  public static function status_list() {
    return array(
      self::STATUS_ACTIVE => t('active'),
      self::STATUS_BLOCKED => t('blocked'),
      self::STATUS_INACTIVE => t('inactive'),
      self::STATUS_DISABLED => t('disabled'),
   );
  }

  /**
   * Delete multiple subscriptions and clean up related data (pending notifications, fields).
   *
   * Was notifications_delete_subscriptions ($params, $field_conditions = array(), $limit = FALSE)
   *
   * Warning: If !$limit, it will delete also subscriptions with more conditions than the fields passed.
   *
   * @param array $params
   *   Array of multiple conditions in the notifications table to delete subscriptions
   * @param array $field_conditions
   *   Array of multiple conditions in the notifications_subscription_fields table to delete subscriptions
   * @param $limit
   *   Whether to limit the result to subscriptions with exactly that condition fields
   *
   * @return int
   *   Number of deleted subscriptions
   */
  public static function delete_multiple($conditions, $field_conditions = array(), $limit = FALSE) {
    $query = self::select_subscriptions($conditions, $field_conditions, $limit);
    if ($sids = $query->execute()->fetchCol()) {
      return self::delete_subscription($sids);
    }
  }

  /**
   * Query for selecting multiple subscriptions
   *
   * @param array $conditions
   *   Array of multiple conditions in the notifications table to delete subscriptions
   * @param array $field_conditions
   *   Array of field objects or multiple (type => value) conditions in the notifications_subscription_fields table to delete subscriptions
   * @param $limit
   *   Whether to limit the result to subscriptions with exactly that condition fields
   */
  public static function select_subscriptions($conditions, $field_conditions = array(), $limit = FALSE) {
    $query = db_select('notifications_subscription', 's')->fields('s', array('sid'));
    foreach ($conditions as $field => $value) {
      if (is_object($value)) {
        $query->condition($value);
      }
      else {
        // This is a field => value pair
        $query->condition('s.' . $field, $value);
      }
    }
    // For exact condition fields we add one more main condition.
    if ($limit) {
      $query->condition('conditions', count($field_conditions));
    }
    if ($field_conditions) {
      $count = 0;
      foreach ($field_conditions as $type => $field) {
        $alias = 'f' . $count++;
        $query->join('notifications_subscription_fields', $alias, "s.sid = $alias.sid");
        if (is_object($field)) {
          $query->condition($field->get_query_condition($alias));
        }
        else {
          $query->condition($alias . '.type', $type);
          $query->condition($alias . '.value', $field);
        }
      }
    }
    return $query;
  }
  /**
   * Delete subscription and clean up related data, included the static cache
   * It also removes pending notifications related to that subscription
   *
   * @param $sid
   *   Subscription object or sid or array of sids of subscription/s to delete
   */
  public static function delete_subscription($sid) {
    $result = db_delete('notifications_subscription')->condition('sid', $sid)->execute();
    db_delete('notifications_subscription_fields')->condition('sid', $sid)->execute();
    return $result;
  }

  /**
   * Check whether this is a subscription instance.
   *
   * Regular subscription types will be the same as the instance. For more ellaborated subscription
   * types, we can get multiple different instances depending on field values.
   */
  public function is_instance() {
    return $this->is_stored() || !empty($this->prepared);
  }

  /**
   * Check whether this is a stored instance
   */
  public function is_stored() {
    return !empty($this->sid);
  }

  /**
   * Get subscription instance.
   *
   * This will be the same object for regular subscriptions. More complex subscriptins can be able
   * to produce multiple types depending on parameters.
   */
  public function instance() {
    if (!$this->is_instance()) {
      $this->invoke_all('prepare');
      $this->prepared = TRUE;
    }
    return $this;
  }

  /**
   * Check user access to this subscription
   */
  public function user_access($account, $op = 'view') {
    // If this has a user, only same user or administrators have access
    if (!empty($this->uid) && $account->uid != $this->uid && !user_access('administer notifications', $account) && !user_access('administer subscriptions', $account)) {
      return FALSE;
    }
    elseif ($op == 'view') {
      return TRUE;
    }
    elseif ($op == 'subscribe') {
      // To create a new subscription user needs to have access to all the objects
      return $this->type_access($account) && $this->object_access($account);
    }
  }
  /**
   * Check access to all the subscription objects
   */
  public function object_access($account) {
    foreach ($this->get_objects(TRUE) as $object) {
      if (!$object->user_access($account)) {
        return FALSE;
      }
    }
    return TRUE;
  }
  /**
   * User access function
   */
  public function type_access($account) {
    $access = $this->get_info('access');
    // Access may be true/false or an array of permissions
    if ($access && is_array($access)) {
      foreach ($access as $perm) {
        if (!user_access($perm, $account)) {
          return FALSE;
        }
      }
      return TRUE;
    }
    else {
      return (boolean)$access;
    }
  }

  /**
   * Get subscribe / unsubscribe text for this
   */
  protected function get_link_text($type, $options = array()) {
    switch ($type) {
      case 'subscribe':
        return t('Subscribe to: @name', array('@name' => $this->get_name()));
      case 'unsubscribe':
        return t('Unsubscribe from: @name', array('@name' => $this->get_name()));
      default:
        return parent::get_link_text($type);
    }
  }

  /**
   * Get path for links
   */
  protected function get_path($operation, $options = array()) {
    $options = $this->get_link_options($operation, $options);
    switch ($operation) {
      case 'subscribe':
      case 'add':
        return $options['base_path'] . '/' . $this->type;
      case 'unsubscribe':
        return $options['base_path'] . '/' . $this->sid;
      default:
        return parent::get_path($operation, $options);
    }
  }

  /**
   * Set name for this specific subscription
   */
  function set_name($name) {
    $this->name = $name;
    return $this;
  }
  /**
   * Set send interval
   */
  function set_interval($interval = 0) {
    $this->send_interval = $interval;
    return $this;
  }
  /**
   * Set send method
   */
  function set_method($method) {
    $this->send_method = $method;
    return $this;
  }
  /**
   * Set user account as the owner of this subscription and take care of defaults for this account.
   *
   * @param $account
   *   User account or user uid
   */
  function set_user($account) {
    $account = messaging_user_object($account);
    $this->uid = $account->uid;
    if (!isset($this->send_interval)) {
      $this->send_interval = notifications_user_setting('send_interval', $account, 0);
    }
    if (!isset($this->send_method)) {
      $this->send_method = notifications_user_setting('send_method', $account);
    }
    if (empty($this->language)) {
      $this->language = user_preferred_language($account)->language;
    }
    return $this;
  }

  /**
   * Load condition fields from db
   */
  function load_fields() {
    // Make sure fields are set
    $this->set_fields();
    if (!empty($this->sid)) {
      foreach (Notifications_Field::load_multiple(array('sid' => $this->sid)) as $field) {
        $this->set_field($field);
      }
    }
  }
  /**
   * Save to db
   */
  function insert() {
    $result = parent::insert();
    $this->save_fields(FALSE);
    return $result;
  }
  /**
   * Update db
   */
  function update() {
    $this->save_fields(TRUE);
    return parent::update();
  }
  /**
   * Delete from db
   */
  function delete() {
    if ($this->is_stored()) {
      $this->invoke_all('delete');
      return $this->delete_subscription($this->sid);
    }
    else {
      return FALSE;
    }
  }
  /**
   * Check all subscriptions values and set error message if any invalid one
   *
   * @return boolean
   *   TRUE if everything is ok
   */
  function check_all() {
    if (!$this->check_account()) {
      $this->error_message = t('Invalid user account for this subscription.');
      return FALSE;
    }
    elseif (!$this->check_fields()) {
      $this->error_message = t('Invalid field values for this subscription.');
      return FALSE;
    }
    elseif (!$this->check_destination()) {
      $this->error_message = t('The destination method or address for the subscription is not valid.');
      return FALSE;
    }
    else {
      return TRUE;
    }
  }
  /**
   * Check subscription user account and related parameters
   */
  function check_account() {
    if (!isset($this->uid)) {
      return FALSE;
    }
    elseif (!isset($this->send_method) || !isset($this->send_interval)) {
      return $this->set_account($this->get_user());
    }
    else {
      return TRUE;
    }
  }
  /**
   * Check destination (user, method, address, etc...)
   */
  function check_destination() {
    if ($this->get_user()) {
      return !empty($this->send_method);
    }
    else {
      return !empty($this->send_method) && !empty($this->mdid);
    }
  }
  /**
   * Check permission for user account
   */
  function check_access($account = NULL) {
    $account = $account ? $account : $this->get_account();
    // @todo check access to all subscription fields
    return TRUE;
  }
  /**
   * Check all fields are there and they have a value
   */
  function check_fields() {
    return count($this->type_fields()) === count($this->get_fields(TRUE));
  }
  /**
   * Create destination for this subscription
   */
  function create_destination($method = NULL, $address = NULL) {
    $account = $this->get_user();
    if (Messaging_Destination::validate_method($method, $address, $account)) {
      $destination = Messaging_Destination::build_method($method, $address, $account);
      $this->set_destination($destination);
    }
  }


  /**
   * Save to db
   */
  function save() {
    $update = $this->is_stored();
    $this->updated = REQUEST_TIME;
    if (!$update) {
      $this->created = REQUEST_TIME;
    }
    $result = drupal_write_record('notifications_subscription', $this, $update ? 'sid' : array());
    $this->save_fields($update);
    return $result;
  }

  /**
   * Save condition fields to db
   *
   * @param $update
   *   Whether this is an old subscription being created
   */
  function save_fields($update = FALSE) {
    $result = TRUE;
    if (isset($this->fields)) {
      if ($update) {
        db_query('DELETE FROM {notifications_subscription_fields} WHERE sid = :sid', array(':sid' => $this->sid));
      }
      foreach ($this->get_fields() as $field) {
        $field->set_subscription($this);
        $result = $result && $field->save();
      }
    }
    return $result;
  }

  /**
   * Get fields as array of field => value pairs
   *
   * Duplicate fields are returned as field => array(value1, value2...)
   *
   * @param $type
   *   Optional to just return the values for some field type
   */
  function get_conditions($type = NULL) {
    $list = array();
    foreach ($this->get_fields() as $field) {
      // We cannot simply use isset() because the value may be NULL
      if (!array_key_exists($field->field, $list)) {
        $list[$field->field] = $field->value;
      }
      elseif (is_array($list[$field->field])) {
        $list[$field->field][] = $field->value;
      }
      else {
        $list[$field->field] = array($list[$field->field], $field->value);
      }
    }
    if (isset($type)) {
      return isset($list[$type]) ? $list[$type] : NULL;
    }
    else {
      return $list;
    }
  }
  /**
   * Check whether we have a given condition
   */
  function has_condition($type, $value) {
    $conds = $this->get_conditions($type);
    return isset($conds) && (is_array($conds) && in_array($value, $conds) || !is_array($conds) && $conds === $value);
  }

  /**
   * Order and serialize fields so we can get a unique signature for this subscription fields
   */
  function serialize_fields() {
    return notifications_array_serialize($this->get_conditions());
  }
  /**
   * Serialize type and conditions
   */
  function serialize_type() {
    return implode('/', array($this->type, $this->serialize_fields()));
  }

  /**
   * Add field arguments from url
   *
   * @param $fields
   *   String of field names separated by commas
   * @param $values
   *   String of field values separated by commas
   */
  function add_field_args($fields, $values) {
    $names = explode(',', $fields);
    $params = explode(',', $values);
    foreach ($names as $index => $type) {
      $this->add_field($type, isset($params[$index]) ? $params[$index] : NULL);
    }
  }
  /**
   * Add field from type, value
   */
  function add_field($type, $value) {
    $field = Notifications_Field::build($type, $value);
    return $this->set_field($field);
  }
  /**
   * Set a field, we may need to find out the position
   */
  function set_field($field) {
    $field->set_subscription($this);
    if (!isset($field->position)) {
      $field->position = $this->map_field($field);
    }
    $this->fields[$field->position] = $field;
    $this->conditions = count($this->fields);
    return $this;
  }
  /**
   * Get a field for a given position
   */
  function get_field($position) {
    $fields = $this->get_fields();
    return isset($fields[$position]) ? $fields[$position] : NULL;
  }
  /**
   * Get fields as array of field objects
   */
  function get_fields($filter = NULL) {
    if (!isset($this->fields)) {
      if ($this->is_stored()) {
        $this->load_fields();
      }
      else {
        $this->set_fields();
      }
    }
    return $this->filter_fields($this->fields, $filter);
  }

  /**
   * Get instance fields that don't have a value set for this type
   */
  function get_instance_fields() {
    $this->get_fields();
    $editable = array();
    foreach ($this->type_fields(FALSE) as $type_field) {
      // Get current instance field if available, new one (from type) if not
       if ($field = $this->get_field($type_field->position)) {
         $editable[$field->position] = $field;
       }
       else {
         $this->set_field($type_field);
         $editable[$type_field->position] = $type_field;
       }
    }
    return $editable;
  }

  /**
   * Get user account
   */
  function get_user() {
    return isset($this->uid) ? user_load($this->uid) : NULL;
  }

  /**
   * Set field values, all at a time from the ones coming from the subscription type.
   */
  function set_fields($fields = NULL) {
    if (isset($fields)) {
      $this->fields = array();
      foreach ($fields as $field) {
        $this->set_field($field);
      }
    }
    elseif (!isset($this->fields)) {
      $this->set_fields($this->type_fields());
    }
  }

  /**
   * Get destination object.
   *
   * Though a destination is just an address, in this case it will have a sending method too,
   * so it can be passed on to the messaging layer with the full information.
   */
  function get_destination() {
    if (!empty($this->mdid)) {
      // This is a stored destination, load and return
      return Messaging_Destination::load($this->mdid);
    }
    elseif ($this->uid && ($user = $this->get_user())) {
      // We try to build a destination knowing the method and the user
      return Messaging_Destination::build_method($this->send_method, NULL, $user);
    }
    else {
      return NULL;
    }
  }

  /**
   * Get language object
   */
  function get_language() {
    if (!empty($this->language) && ($languages = language_list()) && isset($languages[$this->language])) {
      return $languages[$this->language];
    }
    else {
      return user_preferred_language($this->get_account());
    }
  }

  /**
   * Set destination object
   */
  function set_destination($destination) {
    if (empty($destination)) {
      $this->mdid = 0;
      $this->destination = '';
    }
    elseif (is_object($destination)) {
      $this->uid = $destination->uid;
      $this->mdid = $destination->mdid;
      $this->destination = $destination->address;
    }
    elseif (is_numeric($destination)) {
      $this->mdid = $destination;
    }
    return $this;
  }

  /**
   * Whether this subscription's fields are editable or not
   *
   * Unless preset the 'editable' property, this is how it works:
   * - Once we have an instance we don't allow changing the fields, which may cause some consistency problems
   * - Also if the subscription type has no fields, this is not editable
   * - When it has fields and they've been all preset, not editable either
   */
  function is_editable() {
    if (!isset($this->editable)) {
      if (!$this->is_instance() && ($type_fields = $this->get_type_fields())) {
        // It is editable if not all fields are set
        $this->editable = count($type_fields) > count($this->get_instance_fields());
      }
      else {
        // It is instance or the type has no fields
        $this->editable = FALSE;
      }
    }
    return $this->editable;
  }

  /**
   * Get instance of this one for certain conditions
   *
   * If we got multiple ones, return just the first one
   *
   * @param $params
   *   Parameters to add to the subscription type to get an instance of itself
   */
  function get_instance($params = array(), $return_self = FALSE) {
    $params += array('type' => $this->type);
    if ($user = $this->get_user()) {
      $params+= array('uid' => $user->uid);
    }
    if ($instance = $this->load_single_instance($params)) {
      return $instance;
    }
    else {
      return $return_self ? $this : FALSE;
    }
  }

  /**
   * Get objects associated to this subscription's fields
   */
  function get_objects($filter = NULL) {
    $objects = array();
    foreach ($this->get_fields($filter) as $index => $field) {
      $objects[$index] = $field->get_object();
    }
    return $objects;
  }

  /**
   * Format as short text
   */
  function format_short($format = self::FORMAT_HTML) {
    return t('@type: !values', array('@type' => $this->get_type('title'), '!values' => $this->format_name($format | self::FORMAT_INLINE)));
  }
  /**
   * Format as long text
   */
  function format_long($format = self::FORMAT_HTML) {
    return t('Subscription %id of type %type to: !values', array('%id' => $this->sid, '%type' => $this->get_type('title'), '!values' => $this->format_name($format | self::FORMAT_INLINE)));
  }

  /**
   * Get subscription short name.
   */
  function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    else {
      return t('!type subscription', array('!type' => $this->get_title()));
    }
  }
  /**
   * Get object title
   */
  public function get_title() {
    return $this->get_type('title', t('Subscription'));
  }
  /**
   * Get long description
   */
  public function get_description($format = self::FORMAT_HTML) {
    return $this->get_type('description');
  }
  /**
   * If the subscription type has a name, like custom subscriptions have, that will be the name
   * Otherwise we build the name using fields and values
   */
  function format_name($format = self::FORMAT_PLAIN) {
    if ($name = $this->get_name()) {
      return $name;
    }
    else {
      return $this->format_fields($format);
    }
  }

  /**
   * Format all fields
   *
   * @return array();
   *   Array of arrrays with (name, value)
   */
  function format_fields($format = self::FORMAT_HTML) {
    if (!isset($this->format[$format]['fields'])) {
      // Get field names and values formatting each field
      $items = array();
      foreach ($this->get_fields() as $field) {
        $items[] = $this->format_field($field, $format);
      }
      $this->format[$format]['fields'] = $this->format_items($items, $format);
    }
    return $this->format[$format]['fields'];
  }

  /**
   * Format items
   *
   * @param $items
   *   Array of arrays with 'name' and 'value' elements
   */
  function format_items($items, $format = self::FORMAT_INLINE) {
    // If no items the output will be always an empty string
    if (!$items)  {
      return '';
    }
    // Some formats need each item to be a string first
    if ($format & self::FORMAT_INLINE) {
      foreach ($items as $key => $value) {
        if (is_array($value)) {
          $items[$key] = implode(': ', $value);
        }
      }
    }
    switch (TRUE) {
      case $format & self::FORMAT_INLINE:
        return implode(',', $items);
      case $format & self::FORMAT_LIST:
        return theme('item_list', array('items' => $items));
      case $format & self::FORMAT_TABLE:
        return theme('table', array('rows' => $items));
      default:
        // Items not formatted, return as array
        return $items;
    }
  }
  /**
   * Format subscriptions field for display and get some more information
   *
   * @return array()
   *   Array with 'name' and 'value' elements
   */
   function format_field($field, $format = self::FORMAT_HTML) {
    return $field->format($format);
   }

  /**
   * Subscription information field for several forms
   *
   * @return Forms API field structure
   */
  function form_info() {
    $info = $this->get_type();
    // Get fields formatted as array of items
    $fields = $this->get_fields();
    if (!empty($info['name'])) {
      // This subscription type already has a name
      $value = $info['name'];
    }
    elseif (empty($fields)) {
      // No name, maybe no fields it should be enough with the title
      $value = '';
    }
    elseif (count($fields) == 1) {
      // If the field is unique, we don't need a table nor a name for it
      $value = $this->format_fields(self::FORMAT_HTML | self::FORMAT_INLINE);
    }
    else {
      // Multiple fields, format as a table
      $value = $this->format_fields(self::FORMAT_TABLE);
    }
    // Build a form field with all these values
    $field = array(
      '#type' => 'item',
      '#title' => t('@type subscription', array('@type' => $this->get_type('title'))),
      '#value' => $value,
    );
    if (!empty($info['description'])) {
      $field['#description'] = $info['description'];
    }
    return $field;
  }

  /**
   * Get a subform to edit field elements.
   *
   * Defaults to the subscription type method with this instance fields.
   */
  function fields_edit_fieldset($fields = NULL) {
    $fields = isset($fields) ? $fields : $this->get_fields();
    $elements['fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Subscription fields'),
    ) + $this->fields_edit_elements($fields);
    return $elements;
  }

  /**
   * Get elements to edit fields. Subscription can override the fieldset
   */
  function fields_edit_elements($fields = NULL) {
    $fields = isset($fields) ? $fields : $this->get_fields();
    $elements = array();
    foreach ($fields as $index => $field) {
      $elements[$index] = $this->field_edit_element($field);
    }
  }
  /**
   * Get element to edit field. Subscription can override the field element.
   */
  function field_edit_element($field, $element = array()) {
    return $field->form_edit_element($field);
  }

  /**
   * Display a form field for a notifications_field
   */
  public function field_element($field, $element = array()) {
    return $this->field_edit_element($field, $element);
  }

  /**
   * Get field types for this subscription type. The order is important as it will determine the field index
   */
  function field_types() {
    return isset($this->field_types) ? $this->field_types : $this->get_info('field_types', array());
  }
  /**
   * Get field values  for this subscription type. The order is important as it will determine the field index
   *
   * For empty values, well fill with NULL until the number of fields
   */
  function field_values() {
    $values = isset($this->field_values) ? $this->field_values : $this->get_info('field_values', array());
    $values = array_pad($values, count($this->field_types()), NULL);
    return $values;
  }
  /**
   * Get object types
   */
  function object_types() {
    return isset($this->object_types) ? $this->object_types : $this->get_info('object_types', array());
  }
  /**
   * Get subscription type fields as array of field objects
   */
  function type_fields($filter = NULL) {
    $fields = array();
    $values = $this->field_values();
    foreach ($this->field_types() as $index => $type) {
      $fields[$index] = Notifications_Field::build_type($type, array('value' => $values[$index], 'position' => $index));
    }
    return $this->filter_fields($fields, $filter);
  }
}

/**
 * Simple subscription.
 *
 * This is the base class for subscriptions that:
 * - Have a fixed number of fields defined by the subscription type
 * - Have only a field of each type
 */
class Notifications_Subscription_Simple extends Notifications_Subscription {
  /**
   * In this case we can get fields by type or by position
   */
  function get_field($index) {
    if (is_numeric($index)) {
      return parent::get_field($index);
    }
    else {
      $mapping = array_flip($this->field_types());
      return isset($mapping[$index]) ? parent::get_field($mapping[$index]) : NULL;
    }
  }

}

/**
 * Multiple subscription. It has an undetermined number of fields
 *
 * This is the base class for subscriptions that:
 * - Have a fixed number of fields defined by the subscription type
 * - Have only a field of each type
 */
class Notifications_Subscription_Multiple extends Notifications_Subscription {

  /**
   * Map field to its right position in this subscription. As fields have no specific position it will be the next one
   */
  function map_field($field) {
    return isset($this->fields) ? count($this->fields) : 0;
  }
  /**
   * Check all fields are there and they have a value. In this case we need at least the same fields that the type fields has
   */
  function check_fields() {
    return count($this->type_fields()) <= count($this->get_fields(TRUE));
  }
}

/**
 * Subscription without fields
 */
class Notifications_Subscription_NoFields extends Notifications_Subscription {
   /**
   * Return a simple subscription type condition
   */
  function event_conditions($event) {
    return db_and()->condition('s.type', $this->type);
  }
}
