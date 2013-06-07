<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Notifications Event class
 *
 * This is the object that when triggered will produce notifications to be sent.
 *
 * Each event type can have a linked template that will be the object responsible for building the notification text
 *
 * @see hook_notifications('event types')
 *
 * Hidden variable: 'notifications_event_log', set to number of seconds for keeping all events logged for that time
 *
 */
class Notifications_Event extends Notifications_Entity {
  // Object unique id
  public $eid;
  // Module and type of the event, will define it
  public $module = 'notifications';
  public $type = '';
  public $action = '';

  // User who produced the event
  public $uid = 0;
  // Main object id. I.e. if a node event it will be nid
  public $oid;
  // Time the event was produced
  public $created;
  public $triggered;
  // Notifications in queue linked to this event
  public $counter = 0;
  // Mark event as log
  public $log = 0;
  // Objects for this event
  public $objects = array();
  // Generic content for this event. It can be nodes, comments, etc...
  protected $content;
  // Processing options, event to be queued, sent, discarded
  public $queue;
  public $send;
  public $dispatch;
  // Whether this has already been queued
  public $queued = FALSE;
  // Will be set if any of the objects cannot be loaded
  public $incomplete = FALSE;
  // Event text for composition. This can override the event's default texts.
  public $text;
  // Pre-built template to use for this event
  protected $template;
  // Keep track of notifications sent for this event, indexed by destination
  protected $notifications_sent;
  // Last (biggest) sid, this message has been sent to
  protected $last_sid = 0;
  // Stats about this event
  public $send_start = 0; // Timestamp start sending
  public $send_end = 0; // Timestamp end sending
  public $send_time = 0; // Time lapse, sending time, seconds
  public $notif_count = 0; // Counter for notifications to be sent
  public $notif_errors = 0; // Number of sending errors
  public $notif_sent = 0; // Number of notifications sent
  public $notif_success = 0; // Number of messages successfully sent
  /**
   * Constructor
   */
  function __construct($object = NULL) {
    parent::__construct($object);
    if (!isset($this->created)) {
      $this->created = time();
    }
  }
  /**
   * Build Event from db object
   */
  public static function build_object($object) {
    $class = self::type_info($object->type, 'class', 'Notifications_Event');
    if (!empty($object->data)) {
      drupal_unpack($object);
    }
    return new $class($object);
  }
  /**
   * Build Event from type and action
   */
  public static function build_type($type, $action = NULL) {
    $class = self::type_info($type, 'class', 'Notifications_Event');
    $action = $action ? $action : self::type_info($type, 'action', 'default');
    return new $class(array('type' => $type, 'action' => $action));
  }

  /**
   * Get object title
   */
  public function get_title() {
    return $this->get_type('title', t('Event'));
  }
  /**
   * Get object name
   */
  public function get_name() {
    return $this->get_type('name', t('Event'));
  }
  /**
   * Get generic content
   */
  public function get_content() {
    return isset($this->content) ? $this->content : NULL;
  }
  /**
   * Get subscription type information
   */
  public static function type_info($type_key = NULL, $property = NULL, $default = NULL) {
    return notifications_event_type($type_key, $property, $default);
  }
  /**
   * Load event by id
   */
  public static function load($eid) {
    $event = entity_load('notifications_event', array($eid));
    return $event ? $event[$eid] : FALSE;
  }
  /**
   * Load multiple events
   */
  public static function load_multiple($eids = array(), $conditions = array()) {
    return entity_load('notifications_event', $eids, $conditions);
  }
  /**
   * Get event type information
   */
  function get_type($property = NULL, $default = NULL) {
    return $this->type_info($this->type, $property, $default);
  }
  /**
   * Get simple subject text
   */
  function get_subject() {
    return t('Notifications event');
  }

  /**
   * Get message template to build this event as text
   *
   * The difference with create template is that this one keeps the template with the event so it can be reused
   */
  public function get_template() {
    if (!isset($this->template)) {
      $this->template = $this->create_template();
    }
    return $this->template;
  }

  /**
   * Create message template to build this event as text
   *
   * The value will be taken from this event's defaults, but can be overriden on hook_notifications('event types')
   */
  function create_template() {
    $template_name = $this->get_type('template', 'default');
    return notifications_template($template_name)
      ->set_event($this);
  }

  /**
   * Build template
   */
  function build_template() {
    return $this->get_template()->build();
  }

  /**
   * Get event text if available
   */
  function get_text($key) {
    if (isset($this->text[$key])) {
      return $this->text[$key];
    }
  }

  /**
   * Add Drupal Object, converting it into a Notifications_Object
   *
   * @param $object
   */
  function add_object($type, $value) {
    return $this->set_object(notifications_object($type, $value));
  }
  /**
   * Set Notifications object
   */
  function set_object($object) {
    $this->objects[$object->type] = $object;
    return $this;
  }
  /**
   * Get event objects
   */
  function get_objects() {
    return $this->objects;
  }
  /**
   * Get single object
   */
  function get_object($type) {
    return isset($this->objects[$type]) ? $this->objects[$type] : NULL;
  }
  /**
   * Check that all the objects still exist
   */
  function check_objects() {
    foreach ($this->get_objects() as $object) {
      if (!$object->value) return FALSE;
    }
    return TRUE;
  }

  /**
   * Trigger event. Save, run queue query, etc...
   *
   * Replaces notifications_event_trigger($event)
   */
  public function trigger() {
    // If already been triggered, don't do it again
    if (empty($this->triggered)) {
      $this->prepare();
      $this->triggered = REQUEST_TIME;
      // Notify other modules we are about to trigger some subscriptions event
      // Modules can do cleanup operations or modify event properties. To discard, set $event->dispatch = FALSE;
      $this->invoke_all('trigger');
      // Now dispatch de event (send, queue) if not marked for the opposite
      if ($this->dispatch) {
        return $this->dispatch();
      }
    }
    // If already triggered, or not to be dispatched or whatever
    return FALSE;
  }
  /**
   * Prepare event, set default queueing / sending options
   */
  public function prepare() {
    if (!isset($this->dispatch)) {
      $this->dispatch = variable_get('notifications_event_dispatch', 1);
    }
    if (!isset($this->queue)) {
      $this->queue = $this->dispatch && variable_get('notifications_event_queue', 0);
    }
    if (!isset($this->send)) {
      $this->send = $this->dispatch && !$this->queue;
    }
    return $this;
  }
  /**
   * Dispatch event to where it corresponds
   */
  public function dispatch() {
    // Send event to queue for subscriptions, unless marked not to
    if ($this->queue) {
      return $this->queue();
    }
    elseif ($this->send) {
      return $this->send();
    }
    else {
      // Not for queue nor for sending, just do nothing
      return FALSE;
    }
  }

  /**
   * Send operation. Default will be send to all destinations
   */
  public function send() {
    $this->sent = REQUEST_TIME;
    $this->record();
    $this->send_all();
    $this->done();
  }
  /**
   * Create a record for the event and get unique eid
   *
   * @param $update
   *   Whether to update the row if already created.
   */
  function record($update = FALSE) {
    if (!$this->eid || $update) {
      $this->send_time = $this->send_end - $this->send_start;
      $this->updated = time();
      drupal_write_record('notifications_event', $this, $this->eid ? 'eid' : array());
    }
  }
  /**
   * Save full event
   */
  function save() {
    // First of all, make sure we have a unique eid
    $this->record();
    return db_update('notifications_event')
      ->condition('eid', $this->eid)
      ->fields(array('data' => serialize($this)))
      ->execute();
  }

  /**
   * Queue event for later processing
   */
  function queue() {
    // First of all, make sure we have a unique eid
    $this->queued = REQUEST_TIME;
    $this->record(TRUE);
    // If advanced queue enabled, go for it. Otherwise, go for Drupal Queue
    if (function_exists('notifications_queue')) {
      notifications_queue()->queue_event($this);
    }
    else {
      $queue = DrupalQueue::get('notifications_event');
      $queue->createItem($this);
    }

    // Modules can do cleanup operations or modify the queue or the event counter
    $this->invoke_all('queued');
  }

  /**
   * Delete from db
   */
  function delete() {
    if (!empty($this->eid)) {
      // Inform all modules when we still have an eid, in case they have linked data
      $this->invoke_all('delete');
      // Finally, delete traces in our reference table
      return db_delete('notifications_event')->condition('eid', $this->eid)->execute();
      unset($this->eid);
    }
  }

  /**
   * Check user access to event's objects
   *
   * Replaces notifications_event_user_access($event, $account);
   */
  public function user_access($account, $op = 'view') {
    foreach ($this->get_objects() as $object) {
      if (!$object->user_access($account)) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Set parameters
   */
  public function set_params($params = array()) {
    $params += array(
      'uid' => $GLOBALS['user']->uid,
      'language' => $GLOBALS['language']->language,
    );
    foreach ($params as $key => $value) {
      $this->$field = $value;
    }
    return $this;
  }

  /**
   * Clean up queued events
   *
   * Replaces notifications_event_clean()
   *
   * @param $update
   *   Update event counter
   */
  public static function queue_clean($update = FALSE) {
    return notifications_queue()->event_clean($update);
  }

  /**
   * Unserialize after db loading
   */
  public function unserialize() {
    $this->params = $this->params ? unserialize($this->params) : array();
  }
  /**
   * Track notifications queue row processed, decrease counter
   */
  function track_count() {
    return $this->counter ? --$this->counter : 0;
  }
  /**
   * Update event counter
   */
  function update_counter($value = NULL) {
    if (isset($value)) {
      $this->counter = $value;
    }
    db_query('UPDATE {notifications_event} SET counter = :counter WHERE eid = :eid', array(':counter' => $this->counter, ':eid' => $this->eid));
  }

  /**
   * Build query for subscriptions that match this event type
   */
  public function query_subscriptions() {
    // For regular events, time interval must be >= 0
    $query = db_select('notifications_subscription', 's')
      ->condition('s.status', Notifications_Subscription::STATUS_ACTIVE)
      ->condition('s.send_interval', 0, '>=');
    // Maybe we don't need to notify the user who produced this
    if ($this->uid && !variable_get('notifications_sendself', 1)) {
      $query->condition('s.uid', $this->uid, '<>');
    }
    return $query;
  }

  /**
   * Get subscriptions conditions
   */
  protected function subscriptions_conditions() {
    $add = db_or();
    foreach ($this->subscription_types() as $type) {
      $condition = notifications_subscription($type)->event_conditions($this);
      if ($condition && $condition->count()) {
        $add->condition($condition);
      }
    }
    return $add && $add->count() ? $add : NULL;
  }

  /**
   * Get subscription types triggered by this event
   */
  function subscription_types() {
    // Get types from event definition, add types for main object and run hook_notifications_event()
    $types = $this->get_type('subscriptions', array());
    if (($object_type = $this->get_type('object')) && $object = $this->get_object($object_type)) {
      $types = array_merge($types, $object->subscription_types());
    }
    $types = array_merge($types, $this->invoke_all('subscription types'));
    return array_filter(array_unique($types), 'notifications_subscription_type_enabled');
  }

  /**
   * Invoke all hook_notifications_event()
   */
  function invoke_all($op) {
    return module_invoke_all('notifications_event', $op, $this);
  }

  /**
   * Send message to all subscriptions
   */
  public function send_all() {
    $limit = variable_get('notifications_batch_size', 100);
    $total_count = 0;
    while ($sids = $this->get_subscriptions($limit)) {
      $count = count($sids);
      $results = $this->send_list($sids);
      $sent = count($results['sent']);
      $skip = count($results['skip']);
      $success = count($results['success']);
      $errors = $sent - $success;
      watchdog('notifications', 'Sent event @event to @count subscriptions: @success success, @errors errors, @skip skipped.', array('@event' => $this->get_title(), '@count' => $count, '@success' => $success, '@errors' => $errors, '@skip' => $skip));
      $total_count += $count;
    }
    return $total_count;
  }

  /**
   * Processing and sending is done, log or dispose
   */
  public function done() {
    // If logging enabled record data, if not just delete
    if (variable_get('notifications_event_log', NOTIFICATIONS_EVENT_LOG)) {
      $this->log = 1;
      $this->record(TRUE);
    }
    else {
      $this->delete();
    }
  }

  /**
   * Get subscriptions
   *
   * @param $limit
   *   Whether to limit the number of subscriptions. If so we'll use last_sid and 'notifications_batch_size'
   * @return array
   *   Array of subscription ids
   */
  public function get_subscriptions($limit = 0) {
    if ($condition = $this->subscriptions_conditions()) {
      $query = $this->query_subscriptions()
        ->fields('s', array('sid', 'conditions'));
      $query->leftJoin('notifications_subscription_fields', 'f', 's.sid = f.sid');
      $query->condition($condition);
      $query->groupBy('s.sid');
      $query->having('COUNT(f.sid) = s.conditions');
      if ($limit) {
        $query
          ->condition('s.sid', $this->last_sid, '>')
          ->orderBy('s.sid')
          ->range(0, $limit);
      }
      return $query
        ->execute()
        ->fetchCol();
    }
    else {
      return array();
    }
  }
  /**
   * Send to subscriptors
   *
   * @param $subscriptions
   *   List of subscriptions to send to
   */
  public function send_list($sids) {
    if (!$this->send_start) {
      $this->send_start = time();
    }
    $this->notif_count += count($sids);
    $subscriptions = entity_load('notifications_subscription', $sids);
    // Template to be rendered for each user
    $template = $this->get_template();
    $message = $template->build_message();
    $message->add_event($this, array_keys($subscriptions));
    // Keep track of users sent so we don't repeat
    $results = array('skip' => array(), 'sent' => array(), 'success' => array(), 'error' => array());
    foreach ($subscriptions as $subscription) {
      $this->last_sid = $subscription->sid;
      if ($destination = $subscription->get_destination()) {
        // If already sent for this destination (user, method, address), move on
        if (isset($this->notifications_sent[$destination->index()])) {
          $results['skip'][] = $subscription->sid;
          continue;
        }
        // Mark the event as already sent for this destination
        $this->notifications_sent[$destination->index()] = TRUE;
      }
      else {
        // Missing or wrong destination
        watchdog('notifications', 'Cannot find destination or missing user for subscription @sid', array('@sid' => $subscription->sid), WATCHDOG_WARNING);
      }
      // Check access to all objects related with this event
      if ($destination && ($user = $subscription->get_user()) && $this->user_access($user)) {
        $this->notif_sent++;
        $message->add_destination($destination, $subscription->send_method);
        $results['sent'][] = $subscription->sid;
      }
      else {
        $this->notif_error++;
        $results['error'][] = $subscription->sid;
      }
    }
    // This will send out all messages. It will be a bulk sending or not depending on send method
    if (!empty($results['sent'])) {
      $message->send_all();
      $results['results'] = $message->get_results();
      $results['success'] = count(array_filter($results['results']));
      $this->notif_success += $results['success'];
    }
    $this->send_end = time();
    return $results;
  }
}
