<?php
/**
 * @file
 * Drupal Messaging Framework - Message class file
 * 
 * The steps the message follows are
 * - build
 *   Parameters are checked, goes through alter hooks, etc...
 *   The sending method may change at this stage
 * - prepare
 *   Parameters are prepared using the sending method
 *   The sending method may still change at this stage 
 * - presend, send, aftersend
 *   If the message is to be sent right away
 * - queue, afterqueue
 *   If the message is to be queued
 * 
 * The message will be rendered right before sending or before being stored
 */

/**
 * Message class
 * 
 * This is a message with all the information, ready for sending
 */
class Messaging_Message {
  // Messages status, nothing yet
  const STATUS_NONE = 0;
  // Message status, building
  const STATUS_BUILD = 1;
  // Message status, preparing
  const STATUS_PREPARE = 2;
  // Message status, processing
  const STATUS_PROCESS = 3;
  // Message status, sending
  const STATUS_DISPATCH = 4;
  // Message status, sending
  const STATUS_SEND = 5;
  // Message status: Already sent
  const STATUS_SENT = 10;
  // Message status: Queued
  const STATUS_QUEUE = 20;
  // Message status: Error
  const STATUS_ERROR = 30;
  // Errror code: Unespecified
  const ERROR = 1;
  // Error code: Destination error
  const ERROR_DESTINATION = 2;
  
  // Unique message id
  public $msid = 0;
  // Queue id
  public $mqid = 0;
  // Status code
  public $status = 0;
  // Sending method key: 'mail', 'sms', etc...
  public $method;
  // Destinations, indexed by send method and destination key
  public $destination = array();
  // User destination, if the message is intended for a single user
  public $uid;
  // Language code
  public $language;
  // Sender parameters
  public $sender = array();
  // Module and key (type by module), will have defaults
  public $module = 'messaging';
  public $key = 'message';
  // Results for each destination
  public $results = array();
  // Rendered message parts
  protected $text;
  // Template for this message text
  protected $template;
  // Files linked to this message, object array indexed by 'fid'
  public $files;
  // Processing flags. They are 1 if the message is:
  public $queue = 0; // To be queued
  public $send = 0;  // To be sent
  public $log = 0; // To be logged
  // Message priority. If > 0 it won't be queued.
  public $priority = 0;
  // Multiple timestamps for different events
  public $created = 0;
  public $sent = 0;
  public $queued = 0;
  public $updated = 0;
  // Error code, error text
  public $error = 0;
  // Temporary processing variables. These are not stored
  public $result = TRUE; // Result of last operation
  public $discard = FALSE;  // Marked to be discarded
  public $redirect = FALSE; // Redirect to a different sending method
  public $retry = FALSE;    // Retry sending
  public $deleted = FALSE;  // The message has been deleted, though it's still around for whatever use
  // Keep to-do status and result of every step for every method
  protected $process = array();
  /**
   * Constructor, with predefined array of data
   */ 
  function __construct($data = array()) {
    foreach ($data as $key => $value) {
      $this->$key = $value;
    }
  }
  /**
   * Add destination object
   * 
   * @param $destination
   *   Destination object
   * @param $method
   *   Optional send method, otherwise it will be current method
   */
  function add_destination($destination, $method = NULL) {
    $method = $method ? $method : $this->method;
    $this->destination[$method][$destination->index()] = $destination;
    return $this;
  }
  /**
   * Add destination list, taking care of duplicated destinations
   * 
   * @param $destinations
   *   Array of destinations
   */
  function add_destination_list($destinations, $method = NULL) {
    foreach ($destinations as $index => $dest) {
      $this->add_destination($dest, $method);
    }
    return $this;
  }
  /**
   * Add user as a destination
   * 
   * @param $user
   *   User object
   * @return Messaging_Destination
   *   Returns the destination if successfully added
   */
  function add_user($user) {
    if ($destination = $this->send_method()->user_destination($user)) {
      $this->add_destination($destination);
    }
    return $this;
  }
  /**
   * Add list of users as destinations
   */
  function add_user_list($users) {
    foreach ($users as $index => $user) {
      $this->add_user($user);
    }
    return $this;
  }
  /**
   * Add address or array of addresses as destination
   */
  function add_address($address, $validate = TRUE) {
    if ($destination = $this->send_method()->address_destination($address, $validate)) {
      $this->add_destination($destination);
    }
    return $this;
  }
  /**
   * Add list of addresses as destinations
   */
  function add_address_list($addresses) {
    foreach ($addresses as $key => $address) {
      $addresses[$key] = $this->add_address($address);
    }
    return $this;
  }
  /**
   * Get destinations for sending with current method
   * 
   * @todo Return only the ones not sent
   */
  function get_destinations($method = NULL) {
    $method = $method ? $method : $this->method;
    return $this->destination[$method];
  }
  /**
   * Get sending results
   */
  function get_results() {
    return $this->results;
  }
  
  /**
   * Set message template
   */
  function set_template($template) {
    $this->template['default'] = $template;
  }
  /**
   * Get message template for current method
   */
  function get_template() {
    if (empty($this->method)) {
      return $this->default_template();
    }
    elseif (!isset($this->template[$this->method])) {
      $this->render();
    }
    return $this->template[$this->method];
  }
  /**
   * Get default message template
   */
  protected function default_template() {
    if (!isset($this->template['default'])) {
      // We don't have a template yet, create one
      $this->template['default'] = new Messaging_Message_Text();
    }
    return $this->template['default'];
  }
  /**
   * Set user, if the message is intended for a single user try to find suitable method
   */
  function set_user($account) {
    $this->uid = $account->uid;
    if (empty($this->method)) {
      $this->method = Messaging_Method::default_method($account);
    }
    return $this->add_user($account);
  }
  /**
   * Set send method
   * 
   * @todo Should we reset some parts when changing method?
   */
  function set_method($method) {
    $this->method = $method;
    return $this;
  }
  /**
   * Get user, if the message is intended for a single user
   */
  function get_user() {
    return !empty($this->uid) ? user_load($this->uid) : NULL; 
  }
  /**
   * Get sender parameters or single property
   */
  function get_sender($property = NULL) {
    if (!$property) {
      return $this->sender;
    }
    else {
      return isset($this->sender[$property]) ? $this->sender[$property] : NULL;
    }
  }
  /**
   * Get sender name
   */
  function get_sender_name() {
    if ($name = $this->get_sender('name')) {
      return $name;
    }
    elseif ($account = $this->get_sender_account()) {
      return $this->sender['name'] = check_plain($account->name);
    }
    else {
      return $this->sender['name'] = variable_get('site_name', 'Drupal');
    }
  }
  /**
   * Get sender account if uid present
   */
  function get_sender_account() {
    if (isset($this->sender['uid'])) {
      return user_load($this->sender['uid']);
    }
  }
  /**
   * Check message status, will return FALSE if we should stop processing
   */
  function check_status() {
    return !empty($this->method) && !$this->discard && !$this->error && !$this->deleted;
  }

  /**
   * Set success status and return status check
   */
  function set_status($status) {
    $this->status = $status;
    $this->result = TRUE;
    return $this->check_status();
  }

  /**
   * Get a list of destinations for current method
   */
  function check_destination() {
    if (empty($this->method) || empty($this->destination[$this->method])) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

  /**
   * Get list of destinations of this type
   */
  function destination_type($type) {
    $result = array();
    foreach ($this->destination as $key => $destination) {
      if ($destination->type == $type) {
        $result[$key] = $destination;
      }
    }
    return $result;
  }
  /**
   * Set full array of destinations
   */
  /**
   * Mark as sent for this destination key or for all  
   * 
   * @param $results
   *   Array of results indexed per destination key
   */
  function set_results($results) {
    $this->results = array_merge($this->results, $results);
  }
  
  /**
   * Get rendered subject
   */
  function get_subject() {
    return $this->get_template()->render('subject');
  }
  /**
   * Get rendered body
   */
  function get_body() {
    return $this->get_template()->render('body');
  }
  /**
   * Get message content rendered
   */
  function get_content() {
    return $this->get_template()->render('content');
  }
  /**
   * Get language object
   */
  function get_language() {
    if (!empty($this->language) && ($list = language_list()) && isset($list[$this->language])) {
      return $list[$this->language];
    }
    else {
     return language_default();
    }
  }
  /**
   * Get template text, prepared for this method
   */
  function get_text() {
    return $this->get_template();
  }
  /**
   * Get message files
   */
  function get_files() {
    return !empty($this->files) ? $this->files : array();
  }

  /**
   * Check parameters and go for alter hook
   */
  protected function do_build() {
    $this->template[$this->method] = clone $this->default_template();
    // Save the method in case it changes
    $this->set_status(self::STATUS_BUILD);
    // Provides a hook for other modules to modify the message before sending
    $this->invoke_hook('build', $this);
    // The message must be built, without errors and not for discarding
    return $this->check_status();
  }
  /**
   * Build text parts for drupal_render()
   */
  protected static function prepare_element($text) {
    if (is_array($text)) {
      foreach (element_children($text) as $key) {
        $text[$key] = self::prepare_element($text[$key]);
      }
      return $text;
    }
    else {
      // This should be a string
      return array('#markup' => $text);
    }
  }

  /**
   * Prepare for sending through given method
   * 
   * At this stage, message can be redirected to a different method, marked for queueing, etc..
   */
  protected function do_prepare() {
    $this->set_status(self::STATUS_PREPARE);
    // Prepare invoking one or more sending methods
    $this->send_method()->message_prepare($this);
    // At this stage the message must be ready and have a valid destination
    return $this->check_destination();
  }
  
  /**
   * Render for current method
   */
  public function render() {
    return $this->process('build', 'prepare', 'render');
  }
  /**
   * Render through sending method
   */
  protected function do_render() {
    $this->send_method()->message_render($this);
    return TRUE;
  }

  /**
   * Send message through sending method.
   * 
   * The message may need to be processed and it can be queued or discarded yet.
   */
  public function send() {
    $this->send = 1;
    return $this->process('build', 'prepare', 'dispatch');
  }
  
  /**
   * Send message through all sending methods
   */
  public function send_all() {
    $result = array();
    foreach ($this->destination as $method => $destinations) {
      $this->set_method($method);
      $result[$method] = $this->send();
    }
    return $result;
  }

  /**
   * Send message through sending method
   */
  protected function do_send() {
    $this->set_status(self::STATUS_SEND);
    // The message has been processed and it is for sending
    $result = $this->send_method()->message_send($this);
    if ($result) {
      $this->sent = time();
      $this->invoke_hook('sent');
    }
    return $result;
  }
  /**
   * Queue message using the messaging store.
   * 
   * This should happen only once per message, we check it hasn't been queued before.
   */
  public function queue() {
    $this->queue = 1;
    return $this->process('build', 'prepare', 'dispatch');
  }
  
  /**
   * Queue message using sending method
   */
  protected function do_queue() {
    $this->set_status(self::STATUS_QUEUE);
    $result = $this->send_method()->message_queue($this);
    if ($result) {
      $this->queued = time();
      $this->invoke_hook('queued');
    }
    return $result;
  }
  
  /**
   * Check whether the message is to be sent / queued
   * 
   * @return boolean
   *   Final success status
   */
  protected function do_dispatch() {
    // Mark as processed, so it is actually queued/sent
    $this->set_status(self::STATUS_DISPATCH);
    // Now, depending on message status, make a final decission.
    if (!empty($this->test)) {
      $result = TRUE;
      $this->invoke_hook('test');
    } 
    elseif (!empty($this->queue)) {
      $result = $this->process('queue');
    }
    else {
      $result = $this->process('send');
    }
    // Message done, maybe log and return
    if ($result) {
      return $this->dispatch_success();
    }
    else {
      return $this->dispatch_failed();
    }
  }
  /**
   * Run operations on message for current method if not done before
   * 
   * @param $step1, $step2....
   *   Name of operation, the method invoked will be do_$step
   */
  protected function process() {
    $operations = func_get_args();
    $result = TRUE;
    while ($result && $step = array_shift($operations)) {
      $result = $this->process_step($step);
    }
    // Success if completed operations and result is still ok
    return $result && empty($operations);
  }

  /**
   * Run operations on message for current method if not done before
   * 
   * @param $step
   *   Name of operation, the method invoked will be do_$step
   */
  protected function process_step($step) {
    while ($this->process_todo($step) && $this->check_status()) {
      $method = $this->method;
      $function = 'do_' . $step;
      $success = $this->$function();
      $this->process_done($step, $success, $method);
    }
    return $this->process_result($step);    
  }
  /**
   * Mark operation as done for the current method
   */
  protected function process_done($step, $success = TRUE, $method = NULL) {
    $method = $method ? $method : $this->method;
    $success = $success && $this->result && $this->check_status();
    $this->process[$method][$step] = $success;
    $this->message_log('Process done', array('method' => $method, 'step' => $step, 'result' => $success));
    return $success;
  }

  /**
   * Check whether an operation is still to do for the current method
   */
  protected function process_todo($step) {
    return !isset($this->process[$this->method][$step]);
  }
  
  /**
   * Return process result for method and operation
   */
  protected function process_result($step) {
    return !empty($this->process[$this->method][$step]);
  }

  /**
   * Get generic parameters
   */
  function get_params($key = NULL) {
    if ($key) {
      return isset($this->params[$key]) ? $this->params[$key] : array();
    }
    else {
      return isset($this->params) ? $this->params : array();
    }
  }

  /**
   * Get send method object
   */
  public function send_method() {
    return !empty($this->method) ? messaging_send_method($this->method) : NULL;
  }
  /**
   * Set error condition and stop processing
   * 
   * @param $text
   *   Error message to be stored
   */
  function set_error($code = 1, $text = NULL) {
    // This will stop processing if we are in the middle of anything
    $this->set_status(self::STATUS_ERROR);
    $text = $text ? $text : t('Error code !code', array('!code' => $code));
    $this->set_text('error', $text);
    $this->result = FALSE;
    $this->error = $code;
    $this->message_log('Error processing message.');
    // By default, messages are set to be logged when errors happen
    if ($this->log_error) {
      $this->log = 1;
    }
    elseif (!$this->log) {
      // Not for logging, discard
      $this->discard = TRUE;
    }
  }
  /**
   * Discard message
   */
  function discard($reason = 'No reason.') {
    $this->result = FALSE;
    $this->discard = TRUE;
    $this->debug('Message discarded during process', array('reason' => $reason));
    $this->delete();
  }

  /**
   * After the message has been processed successfully
   */
  function dispatch_success() {
    if ($this->discard) {
      $this->delete();
    }
    elseif ($this->log) {
      $this->log();
    }
    return TRUE;
  }
  /**
   * After the message has been processed with errors
   */
  function dispatch_failed() {
    if ($this->discard) {
      $this->delete();
    }
    elseif ($this->error || $this->log) {
      $this->log();
    }
    return FALSE;
  }
  /**
   * Delete message from logs and store
   */
  public function delete() {
    if ($this->msid) {
      db_delete('messaging_message')
        ->condition('msid', $this->msid)
        ->execute();
      if (!empty($this->store) && ($store = messaging_store($this->store))) {
        $store->message_delete($this);
      }
    }
    $this->deleted = TRUE;
  }
  /**
   * Create or update message record
   */
  public function record($update = FALSE) {
    if (empty($this->msid) || $update) {
      $this->updated = time();
      if (!isset($this->created)) {
        $this->created = $this->updated;
      }
      return drupal_write_record('messaging_message', $this, $this->msid ? 'msid' : array());
    }
  }
  /**
   * Save message to store $name
   */
  public function save($name = 'store') {
    $this->record();
    if ($store = messaging_store($name)) {
      return $store->save_message($this);
    }
  }
  /**
   * Log message
   */
  public function log() {
    $this->record();
    return $message->save('logs');
  }
  /**
   * Load message by id
   */
  public static function load($msid) {
    $message = entity_load('messaging_message', array($eid));
    return $message ? $message[$msid] : FALSE;
  }

  /**
   * Load multiple events
   */
  public static function load_multiple($msids = array(), $conditions = array()) {
    return entity_load('messaging_message', $msids, $conditions);
  }
  /**
   * Build from db object
   */
  public static function build_object($template) {
    return new Messaging_Message($template);
  }
  /**
   * Build from template object
   */
  public static function build_template($template) {
    return self::build_object($template);
  }
  /**
   * Invoke hook_messaging_message() on all modules
   */
  protected function invoke_hook($op) {
    return module_invoke_all('messaging_message', $op, $this);
  }
  /**
   * Log facility
   */
  protected function message_log($text, $variables = array()) {
    $variables += array('message' => (string)$this);
    messaging_debug($text, $variables);
  }
  /**
   * Debug facility
   */
  protected function message_debug($text, $variables = array()) {
    $variables += array('message' => $this);
    messaging_debug($text, $variables);
  }
  /**
   * Magic method, properly set some variables
   */
  public function __set($name, $value) {
    switch ($name) {
      case 'subject':
      case 'body':
      case 'content';
      case 'header':
      case 'footer':
        // Add text elements the right way
        $this->get_template()->add_element($name, $value);
    }
  }
  // Magic function, format as string
  public function __toString() {
    $msid = isset($this->msid);
    $method = !empty($this->method) ? $this->method : '<none>';
    $destination = !empty($this->destination) ? implode('; ', array_keys($this->destination)) : '<none>';
    $text[] = "method= $method";
    $text[] = "destination= $destination";
    if (!empty($this->error)) {
      $text[] = " error=$this->error";
    }
    return 'Message: ' . implode(' ', $text);
  }
}

