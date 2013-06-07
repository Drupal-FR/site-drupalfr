<?php
/**
 * @file
 * Drupal Messaging Framework - Send_Method class file
 */

/**
 * Base class for mail sending methods
 */
class Messaging_Mail_Method extends Messaging_Send_Method {
  // Default group and address type
  public $method = 'mail';
  public $type = 'mail';
  public $anonymous = TRUE;
  public $format = MESSAGING_FORMAT_PLAIN;

  /**
   * Get address type
   */
  public static function address_type() {
    return 'mail';
  }

  /**
   * Add specific mail parameters to message
   */
  function message_prepare($message) {
    // First, decide on queue, log, cron and send options
    parent::message_prepare($message);
    // Build specific mail parameters and store them into the message
    $params = $this->message_params($message);
    $message->params[$this->method] = $this->mail_params($message, $params);
  }

  /**
   * Render message body. This will produce an array of lines to be formatted later
   */
  function render_body($text) {
    $body = array();
    foreach (element_children($text) as $key) {
      $body[$key] = drupal_render($text[$key]);
    }
    return $body;
  }

  /**
   * Send message to address
   */
  function send_address($address, $message) {
    // Build the message in drupal mail format
    $mail = $this->mail_build($address, $message);
    return $this->mail_send($mail, $message);
  }
  
  /**
   * Actually send mail through Drupal system
   * 
   * @param $mail
   *   Built Drupal mail array, not rendered
   * @param $message
   *   Original message object
   */
  protected static function mail_send($mail, $message) {
    // Retrieve the responsible implementation for this message.
    $system = drupal_mail_system($mail['module'], $mail['key']);
    // Format the message body, last chance for formatting
    $mail = $system->format($mail);
    return $system->mail($mail);    
  }
  /**
   * Get message parameters for this method.
   */
  function message_params($message) {
    return $message->get_params($message->method) + $this->default_params();
  }
  /**
   * Get default method parameters, not depending on message
   */
  static function default_params() {
    $default_mail = variable_get('site_mail', ini_get('sendmail_from'));
    return array(
      'default_from' => variable_get('messaging_mail_default_from', $default_mail),
      'returnpath' => variable_get('messaging_mail_returnpath', $default_mail),
    );
  }

  /**
   * Rebuild message in Drupal mail format, right before sending
   * 
   * This mimics drupal_mail for finest access to properties
   * @param $address
   *   Email address to send to
   * @param $message
   *   Message object
   */
  protected function mail_build($address, $message) {
    $params = $this->message_params($message);
    // Bundle up the variables into a structured array for altering.
    $mail = array(
      'headers' => $this->mail_headers($message, $params),
      'id'       => $message->module . '_' . $message->key,
      'module'   => $message->module,
      'key'      => $message->key,
      'to'       => $address,
      'from'     => isset($params['from']) ? $params['from'] : $params['default_from'],
      'language' => $message->get_language(),
      'params'   => $params,
      'subject'  => $message->get_subject(),
      'body'     => $this->render_body($message->get_template()->build('body')),
      'attachments' => $message->get_files(),
    );
  
    // Build the e-mail (get subject and body, allow additional headers) by
    // invoking hook_mail() on this module. We cannot use module_invoke() as
    // we need to have $message by reference in hook_mail().
    if (function_exists($function = $message->module . '_mail')) {
      $function($message->key, $mail, $params);
    }
  
    // Invoke hook_mail_alter() to allow all modules to alter the resulting e-mail.
    drupal_alter('mail', $mail);
    
    // Invoke drupal_mail without sending, then override headers
    return $mail;
  }
  
  /**
   * Format from name and address
   */
  static function format_from($name, $mail) {
    $tokens = array(
      '[name]' => $name,
      '[mail]' => $mail,
    );
    return strtr(variable_get('messaging_mail_sender_format', '[name] <[mail]>'), $tokens);
  }

  /**
   * Prepare from address and mail headers
   */
  protected function mail_params($message, $params = array()) {
    // The message 'from' will depend on message sender if present, otherwise default to site mail    
    if (empty($params['from'])) {
      $sender_name = $message->get_sender_name();
      $sender_account = $message->get_sender_account();
      if ($sender_name && $sender_account && !empty($sender_account->mail)) {
        $from = $this->format_from($sender_name, $sender_account->mail);
      } elseif ($sender_name) {
        $from = $this->format_from($sender_name, $params['default_from']);
      } else {
        $from = $params['default_from'];
      }
      $params['from'] = $from;
    } else {
      $from = $params['from'];
    }
    $params += array(
      'from'     => $from,
      'headers'  => array(),
    );
    // Set headers, or add to existing ones. Pre-existing ones should not be overridden.
    $headers = $this->mail_headers($message, $params);
    $params['headers'] += $headers;  
  
    return $params;
  }
  
  /**
   * Get mail headers. Helper function for mail methods
   * 
   * @param $params
   *   Array of parameters with the following elements
   *   - 'from', Mail from address
   *   - 'default_from', Default from address
   *   - 'headers', Predefined headers to be added to this one
   */
  static function mail_headers($message, $params = array()) {
    $headers = !empty($params['headers']) ? $params['headers'] : array();
    // Add some default headers
    $headers += array(
      'MIME-Version'              => '1.0',
      'Content-Type'              => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer'                  => 'Drupal'
    );
    $default_from = $params['default_from'];
    $from = !empty($params['from']) ? $params['from'] : $default_from;
    // Set default headers depending on data
    $headers += array(
      'From' => $from,
      'Reply-To' => $from,
    );
    if ($params['returnpath']) {
      // To prevent e-mail from looking like spam, the addresses in the Sender and
      // Return-Path headers should have a domain authorized to use the originating
      // SMTP server. Errors-To is redundant, but shouldn't hurt.
      $more_headers['Sender'] = $more_headers['Return-Path'] = $more_headers['Errors-To'] = $params['returnpath'];
      $headers += $more_headers;
    }
    return $headers;
  }
}

/**
 * Mail addresses
 */
class Messaging_Mail_Address extends Messaging_Destination {
  public $type = 'mail';
  /**
   * Get name for display
   */
  function address_name() {
    return t('E-mail address');
  }
  /**
   * Get user from address
   */
  public static function get_user_from_address($mail) {
    return user_load_by_mail($mail);
  }
  /**
   * Get address from user
   */
  public static function get_address_from_user($user) {
    if ($user = messaging_user_object($user)) {
      return !empty($user->mail) ? $user->mail : NULL;
    }
  }

  public static function validate_address($address, $type = NULL) {
    return valid_email_address($address);
  }
}
