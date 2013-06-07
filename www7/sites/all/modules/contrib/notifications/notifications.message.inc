<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Wrapper for Notifications messages
 *
 * Store some more temporary properties on the message that will be dropped if the message is stored
 */
class Notifications_Message extends Messaging_Message {
  // Message type, set property
  public $type = 'notifications';
  // Send interval used for grouping
  public $send_interval = 0;
  // Notifications events
  public $events = array();
  // Map events to subscriptions
  public $events_subscriptions = array();
  // Subscriptions that triggered this message, indexed by sid
  public $subscriptions = array();
  // Text parts used to build this, will be useful for debugging
  public $text_parts = array();
  // Digest flag, will be true if this is a digest of multiple events
  public $digest = FALSE;
  // Build method used for this
  public $build_method;

  /**
   * Check parameters and go for alter hook
   */
  protected function do_build() {
    parent::do_build();
    drupal_alter('notifications_message', $this);
    // Clean up Notifications objects so we don't move too much data around (Sometimes the full object gets serialized)
    unset($this->events);
    unset($this->events_subscriptions);
    unset($this->subscriptions);
    unset($this->text_parts);
    // The message must be built, without errors and not for discarding
    return $this->check_status();
  }

  /**
   * Add event to the message. One message can be produced by one or more events.
   */
  public function add_event($event, $subscriptions = array()) {
    $this->events[$event->eid] = $event;
    $this->events_subscriptions[$event->eid] = $subscriptions;
    $this->add_subscriptions($subscriptions);
  }

  /**
   * Add subscriptions
   */
  public function add_subscriptions($subscriptions) {
    foreach ($subscriptions as $sid) {
      if (!isset($this->subscriptions[$sid])) {
        $this->subscriptions[$sid] = $sid;
      }
    }
  }
  /**
   * Set message sender. Depending on options this will set sender account too.
   *
   * Optional sender, if chosen will be the user account who produced the event
   * It will be up to the sending method modules what to do with this information.
   */
  public function set_sender($account) {
    $sender_option = variable_get('notifications_sender', 0);
    if ($sender_option) {
      $sender = is_numeric($account) ? notifications_load_user($account) : $account;
      if ($sender_option == 2) {
        parent::set_sender($sender);
      }
      else {
        $this->sender = 0;
        $this->sender_name = $sender->name;
      }
    }
  }

  /**
   * Digest multiple events in a single message, short format.
   *
   * Was: notifications_process_build_simple()
   *
   * @return array with messages ready to be sent
   */
  public static function build_simple($template, $events, $subscriptions, $module = 'notifications') {
    $messages = array();
    $sender_option = variable_get('notifications_sender', 0);
    foreach ($events as $event) {
      $event_subscriptions = isset($subscriptions[$event->eid]) ? array_filter($subscriptions[$event->eid]) : NULL;
      $message = self::build_simple_message($template, $event, $event_subscriptions, $module);
      $message->set_sender($event->uid);
      $messages[] = $message;
    }
    return $messages;
  }

  /**
   * Creates a single message for a single event
   *
   * @param $account
   *   Destination user account
   * @param $event
   *   Event object which caused this notification
   * @param $subscriptions
   *   Array of subscription ids
   * @param $debug
   *   Return template parts information with the message
   *
   * @return
   *   Message object
   */
  public static function build_simple_message($template, $event, $subscriptions, $module = 'notifications') {
    $send_method = $template->method;
    $destination = $template->get_destination();
    $account = $template->get_user();
    $language = $template->get_language();

    // Start the message by cloning the template
    $message = clone $template;
    $message->add_event($event, $subscriptions);
    // Create message. Do all this in one replacemente
    $parts = array(
      'subject' => $event->message_part('subject', $send_method, $language, $module),
      'header' => $event->message_part('header', $send_method, $language, $module),
      'event'  => $event->message_part('main', $send_method, $language, $module),
      'footer' => $event->message_part('footer', $send_method, $language, $module),
    );
    // We pass only the first subscription, which is at least something
    // @ TODO Handle nicely the case where there are more than one subscription
    $subscription = $subscriptions ? notifications_subscription_load(current($subscriptions)) : NULL;
    $objects = array('destination' => $destination, 'user' => $account, 'event' => $event, 'subscription' => $subscription);
    $objects = array_merge($objects, $event->get_objects());
    $text = messaging_template_text_replace($parts, $objects, $language);

    // Get subject out of text and build the message array
    $message->subject = $text['subject'];
    unset($text['subject']);
    $message->body = $text;
    $message->text_parts = $parts;

    return $message;
  }

}
