<?php
/**
 * @file
 * Drupal Notifications Framework - Templates
 */

/**
 * Message template. This should be able to produce a full message by itself
 */
class Notifications_Message_Template extends Messaging_Message_Template {
  // Subscriptions that triggered this notification, indexed by sid
  protected $subscriptions = array();
  // Events that triggered this notifications, indexed by eid
  protected $events = array();
  // Event content to be rendered on the notification
  protected $content;
  // Template information
  protected $info;
  /**
   * Construct from template info
   */
  function __construct($info = NULL) {
    $this->info = $info;
  }

  /**
   * Get Message_Object with this template linked
   */
  public function build_message($options = array()) {
    $message = new Notifications_Message($options);
    $message->set_template($this);
    return $message;
  }

  /**
   * Set notifications event
   */
  public function set_event($event) {
    $this->content = $event->get_content();
    $this->objects = $event->get_objects();
    $this->objects['notifications_event'] = $event;
    return $this;
  }
  /**
   * Add event object and its corresponding template
   */
  function add_event($event) {
    $this->events[$event->eid] = $event;
    $this->add_template('event:' . $event->eid, $event->get_template());
  }
  /**
   * Get objects as Drupal objects (Removing Notifications object wrapper)
   */
  function get_objects() {
    $objects = array();
    foreach (parent::get_objects() as $type => $object) {
      $token_type = is_a($object, 'Notifications_Object') ? $object->get_token_type() : $type;
      $object = is_a($object, 'Notifications_Object') ? $object->get_object() : $object;
      $objects[$token_type] = $object;
    }
    return $objects;
  }

  /**
   * Get defaults for elements. Add some elements to message body to make them available for templates
   */
  /*
  protected function element_defaults($name) {
    $defaults = parent::element_defaults($name);
    switch ($name) {
      case 'body':
        $defaults['#parts'][] = 'user_unsubscribe_url';
        break;
    }
    return $defaults;
  }
  */

  /**
   * Default texts for this template, may need token replacement
   */
  protected function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return $this->text_subject($options);
      case 'header':
        return $this->text_header($options);
      case 'content':
        return $this->text_content($options);
      case 'footer':
        return $this->text_footer($options);
      case 'break':
        return array('#type' => 'messaging_break');
      default:
        return parent::default_text($type, $options);
    }
  }
  /**
   * Subject text
   */
  protected function text_subject($options) {
    return t('Notification for your subscriptions', array(), $options);
  }
  /**
   * Header text
   */
  protected function text_header($options) {
    return array(
      '#tokens' => TRUE,
      '#markup' => t("Greetings [user:name],", array(), $options),
    );
  }
  /**
   * Content text
   */
  protected function text_content($options) {
    return t("A item to which you are subscribed has been updated", array(), $options);
  }
  /**
   * Footer text
   */
  protected function text_footer($options) {
    return array(
      '#tokens' => TRUE,
      //'from' => t('This is an automatic message from !site_name', array('!site_name' => variable_get('site_name', 'Drupal')), $options),
      // For links add markup and plaintext alternatives
      'from' => array(
        '#type' => 'messaging_link',
        '#text' => t('This is an automatic message from [site:name],', array(), $options),
        '#url' => '[site:url]',
      ),
      'unsubscribe' => array(
        '#type' => 'messaging_link',
        '#text' => t('You can unsubscribe at', array(), $options),
        '#url' => '[user:unsubscribe-url]',
      ),
    );
  }
  /**
   * Declare all tokens used for this template
   */
  public function token_list() {
    // Don't forget parent tokens
    $tokens = parent::token_list();
    $tokens[] = 'user:unsubscribe-url';
    $tokens[] = 'user:name';
    return $tokens;
  }
}
