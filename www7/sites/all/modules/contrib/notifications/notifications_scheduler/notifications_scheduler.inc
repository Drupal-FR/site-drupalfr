<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */


/**
 * Notifications Schedule Event class
 *
 * Unlike regular events, these are full auto-contained events that know which message to produce
 * and how to send it. Thus, these events will be queued in regular Drupal queue.
 *
 * These events should allow batched execution too. TBD.
 */
abstract class Notifications_Scheduler_Event extends Notifications_Event {
  // Default system queue name and item if came from queue
  public $queue = 'notifications_event';
  protected $queue_item;
  // Subscriptions counter, this starts with 1 (si the event record is not deleted)
  // And with every process is set to the max sid processed
  public $counter = 1;
  // Subscriptions to be processed on every batch, indexed by sid
  protected $subscriptions;
  // Store action parameters
  protected $action_object;
  protected $accion_context;

  /**
   * Prepare this event to be triggered
   */
  function prepare() {
    parent::prepare();
    // Load the content now, later it may be different
    $this->content = $this->load_content();
    // Build message template so it can be reused later
    if ($this->content && $this->get_template()) {
      $this->dispatch = TRUE;
    }
    else {
      // If something failed we don't even store this event
      $this->dispatch = FALSE;
    }
    return $this;
  }
  /**
   * Prepare event objects from context parameters
   */
  protected abstract function prepare_context();
  /**
   * Load content, to be implemented by subclasses
   */
  protected abstract function load_content();

  /**
   * Set action parameters
   */
  public function set_action($object, $context) {
    $this->action_object = $object;
    $this->action_context = $context;
    $this->prepare_context();
    return $this;
  }
  /**
   * Get property from action context
   */
  public function get_action_context($name = NULL, $default = NULL) {
    if ($name) {
      return isset($this->action_context[$name]) ? $this->action_context[$name] : $default;
    }
    else {
      return $this->action_context;
    }
  }

  /**
   * Create message template to build this event as text
   *
   * The template can be configured per action
   */
  function create_template() {
    $template_name = $this->get_action_context('template');
    // If no template set, we go for the default for this event type
    $template_name = $template_name ? $template_name : $this->get_type('template', 'default');
    return notifications_template($template_name)
      ->set_event($this);
  }
  /**
   * Process event, send pending notifications. Subscriptions start on $counter (min sid)
   *
   * @param $limit
   *   Maximum number of subscriptions to process
   */
  function process($limit = 10) {
    while ($limit && ($subscriptions = $this->get_subscriptions($limit))) {
      $limit = $limit - count($subscriptions);
      // New value for the counter if this goes right
      $counter = max(array_keys($subscriptions));
      // Turn subscriptions into batches, groups, etc...
      $groups = $this->prepare_subscriptions($subscriptions);
      // Now process groups one at a time
      foreach ($groups as $group) {
        $results = $this->process_group($groups);
      }
      // Update counter on the event record
      $this->update_counter($counter);
    }
    if (!empty($counter)) {
      // We will do more processing later
      $this->release();
    }
    else {
      // Nothing to process, delete all this
      $this->delete();
    }
  }

  /**
   * Groups subscriptions. This one will just create a group for all
   */
  protected function prepare_subscriptions($subscriptions) {
    return array($subscriptions);
  }

  /**
   * Process group, add all to a message and send out
   */
  protected function process_group($group) {
    $result = array('messages' => 0, 'items' => 0, 'errors' => 0);
    $message = $this->build_message();
    foreach ($group as $item) {
      if ($destination = $item->get_destination()) {
        $message->add_destination($destination);
      }
      else {
        $result['errors']++;
      }
      $result['items']++;
    }
    if ($message->send()) {
      $result['messages']++;
    }
    return $result;
  }
    /**
   * Build query for subscriptions that match this event type
   */
  function query_subscriptions() {
    // This is the condition for scheduled notifications
    return db_select('notifications_subscription', 's')
      ->condition('s.status', Notifications_Subscription::STATUS_ACTIVE)
      ->condition('s.send_interval', -1);
  }
  /**
   * Check user access to event's objects. Default for mass mailing events is TRUE
   */
  public function user_access($account, $op = 'view') {
    return TRUE;
  }
  /**
   * When done these events cannot be just deleted, we need to keep track of last time it was executed
   */
  public function done() {
    $this->record(TRUE);
    // Now take care of previous events of this type / action
    if (variable_get('notifications_event_log', NOTIFICATIONS_EVENT_LOG)) {
      // If logging enabled, make logs of previous events
      $query = db_update('notifications_event')
        ->fields(array('log' => 1));
    }
    else {
      // If not logging enabled, delete all previous events
      $query = db_delete('notifications_event');
    }
    $query
      ->condition('type', $this->type)
      ->condition('action', $this->action)
      ->condition('eid', $this->eid, '<')
      ->execute();
  }
}

/**
 * Test this schedule class, send latest created nodes
 */
class Notifications_Scheduler_Latest_Posts extends Notifications_Scheduler_Event {
  /**
   * Set action parameters, get field mapping from context
   */
  protected function prepare_context() {
    if ($type = $this->get_action_context('node_type')) {
      $this->add_object('node_type', $type);
    }
    // Some hack, try a field with array value ??
    if ($tid = $this->get_action_context('taxonomy_term')) {
      $this->add_object('taxonomy_term', $tid);
    }
  }
  /**
   * Load nodes created and published or updated since latest time. It doesn't check node access
   */
  protected function load_content() {
    $query = db_select('node', 'n')
      ->fields('n', array('nid', 'created'))
      ->orderBy('n.created', 'DESC')
      ->condition('n.status', 1)
      ->extend('PagerDefault')
      ->limit($this->get_action_context('node_number', variable_get('default_nodes_main', 10)));
    if ($type = $this->get_action_context('node_type')) {
      $query->condition('type', $type);
    }
    if ($tid = $this->get_action_context('taxonomy_term')) {
      $query->join('taxonomy_index', 't', 'n.nid = t.nid');
      $query->condition('t.tid', $tid);
    }
    if ($nids = $query->execute()->fetchCol()) {
      return node_load_multiple($nids);
    }
  }
  /**
   * Check user access to event's objects.
   */
  public function user_access($account, $op = 'view') {
    return user_access('access content', $account);
  }
  /**
   * Get subscription types triggered by this event
   */
  function subscription_types() {
    $types = parent::subscription_types();
    $type = $this->get_action_context('node_type');
    $term = $this->get_action_context('taxonomy_term');
    if ($type) {
      $types[] = 'content_type';
    }
    if ($term) {
      $types[] = 'taxonomy_term';
    }
    if ($type && $term) {
      $types[] = 'content_type_term';
    }
    return $types;
  }
}

/**
 * Send new content posted since last notification.
 */
class Notifications_Scheduler_New_Posts extends Notifications_Scheduler_Latest_Posts {
  /**
   * Set action parameters, get field mapping from context
   *
   * We need to find out when this was last executed for which we build a unique key based on parameters
   * and also we must log it somewhere
   */
  protected function prepare_context() {
    parent::prepare_context();
    $this->action = 'new_posts';
    foreach ($this->get_objects() as $object) {
      $this->action .= ':' . $object->index();
    }
    // Our action could look like new_posts:type:story:term:25
  }

  /**
   * Load nodes created and published since last time this was executed. It doesn't check node access
   */
  protected function load_content() {
    // Only nodes updated since last time
    $query = db_select('node', 'n')
      ->fields('n', array('nid', 'created'))
      ->orderBy('n.created', 'DESC')
      ->condition('n.status', 1)
      ->condition('n.changed', $this->get_last_time(), '>=')
      ->extend('PagerDefault')
      ->limit($this->get_action_context('node_number', 50));
    if ($type = $this->get_action_context('node_type')) {
      $query->condition('type', $type);
    }
    if ($tid = $this->get_action_context('taxonomy_term')) {
      $query->join('taxonomy_index', 't', 'n.nid = t.nid');
      $query->condition('t.tid', $tid);
    }
    if ($nids = $query->execute()->fetchCol()) {
      return node_load_multiple($nids);
    }
  }

  /**
   * Get last time (timestamp) when this event/action was executed
   *
   * @return int (timestamp)
   */
  protected function get_last_time() {
    $last = (int)db_query('SELECT MAX(created) FROM {notifications_event} WHERE type = :type AND action = :action', array(
      ':type' => $this->type,
      ':action' => $this->action,
    ))->fetchField();
    return $last ? $last : variable_get('notifications_scheduler_last', 0);
  }

  /**
   * Check user access to event's objects.
   */
  public function user_access($account, $op = 'view') {
    return user_access('access content', $account);
  }
}
