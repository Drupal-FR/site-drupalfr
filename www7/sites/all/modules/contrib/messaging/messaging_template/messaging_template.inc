<?php
/**
 * @file
 * Drupal Messaging Framework - Text filtering functions
 */

/**
 * Message text class.
 * 
 * A Text is a container for renderable elements. Renderable elements can be:
 * - An array for drupal_render()
 * - A simple string
 * - Other Text objects (Texts can be nested)
 */
class Messaging_Text {
  /**
   * Converts strings to plain utf-8 single line
   */
  static function check_subject($text) {
    $text = check_plain($text);
    // taken from _sanitizeHeaders() in PEAR mail() : http://pear.php.net/package/Mail
    $text = preg_replace('=((0x0A/%0A|0x0D/%0D|\\n|\\r)\S).*=i', NULL, $text);
    return $text;
  }
  /**
   * Clean text of HTML stuff and optionally of line endings
   * 
   * @param $text
   *   Dirty HTML text to be filtered
   * @param $newline
   *   Optional string to be used as line ending
   */
  static function text_clean($text, $newline = NULL) {
    // HTML entities to plain text conversion.
    $text = decode_entities($text);  
    // Filters out all remaining HTML tags
    $text = filter_xss($text, array());
    // Optionally, replace new lines
    if (!is_null($newline)) {
      $text = str_replace("\n", $newline, $text);
    }
    // Trim out remaining beginning/ending spaces
    $text = trim($text);
    return $text;
  }
  
  /**
   * Truncate messages to given length.  Adapted from node_teaser() in node.module
   */
  static function text_truncate($text, $length) {
    // If we have a short message, return the message
    if (drupal_strlen($text) < $length) {
      return $text;
    }  
    // Initial slice.
    $teaser = truncate_utf8($text, $length);
    $position = 0;
    // Cache the reverse of the message.
    $reversed = strrev($teaser); 
    // split at paragraph boundaries.
    $breakpoints = array('</p>' => 0, '<br />' => 6, '<br>' => 4, "\n" => 1);
    // We use strpos on the reversed needle and haystack for speed.
    foreach ($breakpoints as $point => $offset) {
      $length = strpos($reversed, strrev($point));
      if ($length !== FALSE) {
        $position = - $length - $offset;
        return ($position == 0) ? $teaser : substr($teaser, 0, $position);
      }
    } 
    // When even the first paragraph is too long, we try to split at the end of
    // the last full sentence.
    $breakpoints = array('. ' => 1, '! ' => 1, '? ' => 1, ' ' => 0);
    $min_length = strlen($reversed);
    foreach ($breakpoints as $point => $offset) {
      $length = strpos($reversed, strrev($point));
      if ($length !== FALSE) {
        $min_length = min($length, $min_length);
        $position = 0 - $length - $offset;
      }
    }
    return ($position == 0) ? $teaser : substr($teaser, 0, $position);   
  }
}

/**
 * Base template class
 * 
 * A template is a text object that has associated objects and can do token replacement.
 * 
 * These templates have some known parts: subject, header, content, footer
 */
class Messaging_Template {
  // Pre-built elements, needs to be built before render
  public $text = array();
  // Current template elements, renderable array
  public $elements;
  // Default format
  public $format = MESSAGING_FORMAT;
  // Parent element
  protected $parent;
  // Store multiple objects for token replacement
  protected $objects;
  // Options for string building and text replacement
  protected $options = array('replace' => TRUE, 'clear' => FALSE, 'linebreak' => "\n");
  // Tokens to add to all the template elements
  protected $tokens;
  /**
   * Add item of unknown type
   */
  function add_item($name, $value) {
    if (is_string($value)) {
      return $this->add_string($name, $value);
    }
    elseif (is_object($value)) {
      return $this->add_text($name, $value);
    }
    elseif (is_array($value)) {
      return $this->add_element($name, $value);
    }
  }
  /**
   * Add element ready for drupal_render()
   */
  function add_element($name, $element) {
    $this->text[$name] = $element;
    return $this;
  }
  /**
   * Add string
   */
  function add_string($name, $string) {
    $element = array('#markup' => $string);
    return $this->add_element($name, $element);
  }
  /**
   * Add text object
   */
  function add_text($name, $text) {
    $text->set_parent($this);
    return $this->add_element($name, $text);
  }

  /**
   * Set parent text
   */
  function set_parent($template) {
    $this->parent = $template;
    return $this;
  }

  /**
   * Reset built elements
   * 
   * @param $part1, $part2...
   *   Optional parts to reset
   */
  public function reset() {
    $parts = func_get_args();
    if ($parts) {
      foreach ($parts as $key) {
        if (isset($this->elements[$key])) {
          unset($this->elements[$key]);
        }
      }
    }
    else {
      unset($this->elements);
    }
  }
  /**
   * Build all elements, return array
   * 
   * @param $part1, $part2...
   *   Optional parts to render
   */
  public function build() {    
    $parts = func_get_args();
    // If we don't have a list of parts we take known text parts
    $parts = $parts ? $parts : $this->get_parts();
    // Build an array with each of the parts
    return $this->build_parts($parts);
  }
  /**
   * Render elements, return string
   * 
   * @param $part1, $part2...
   *   Optional parts to render
   */
  public function render() {
    $parts = func_get_args();
    // If we don't have a list of parts we take known text parts
    $parts = $parts ? $parts : $this->get_parts();
    $build = $this->build_parts($parts);
    return drupal_render($build);
  }
  /**
   * Build template parts
   */
  public function build_parts($parts) {
    $build = array();
    foreach ($parts as $key) {
      if (isset($this->elements[$key])) {
        // This one was already built
        $build[$key] = $this->elements[$key];
      }
      else {
        $build[$key] = $this->build_element($key);
      }
    }
    return $build;    
  }
  /**
   * Build a named element
   */
  public function build_element($name, $options = array()) {
    $text = $this->get_text($name);
    $element = $text ? $this->build_text($text) : array();
    $element += $this->element_defaults($name);
    
    if (!empty($element['#parts'])) {
      // If the element has subparts, build them before the element
      $element += $this->build_parts($element['#parts']);
    }
    return $this->element_build($element);
  }
  /**
   * Build a message text element
   */
  protected function build_text($element, $options = array()) {
    if (is_object($element)) {
      return $element->build();
    }
    elseif (is_string($element)) {
      return array('#markup' => $element);
    }
    elseif (is_array($element)) {
      return $element;
    }
    else {
      return array();
    }
  }

  /**
   * Build a message element with optional text replacement
   */
  protected function element_build($element, $options = array()) {
    foreach (element_children($element) as $key) {
      $element[$key] = $this->build_text($element[$key], $options);
    }
    /*
    if (!empty($element['#tokens']) && (!isset($options['replace']) || $options['replace'])) {
      $element = $this->element_replace($element, $options);
    }
    */
    return $element;
  }
  /**
   * Perform token replace within an element
   */
  protected function element_replace($element, $options = array()) {
    foreach (array('#markup', '#title', '#children', '#plaintext') as $key) {
      if (!empty($element[$key])) {
        $element[$key] = $this->token_replace($element[$key]);
      }
    }
    foreach (element_children($element) as $key) {
      $element[$key] = $this->element_replace($element[$key], $options);
    }
    return $element;
  }
  
  /*
   * Get defaults for elements
  */
  protected function element_defaults($name) {
    return array('#format' => $this->format, '#method' => $this->method, '#options' => $this->get_options(), '#template' => $this);
  }
  /**
   * Get known template parts
   */
  protected function get_parts() {
    return array_keys($this->text);
  }
  /**
   * Add object to the list
   */
  function add_object($type, $object) {
    $this->objects[$type] = $object;
    return $this;
  }
  /**
   * Get objects from this template (include parent's ones)
   */
  function get_objects() {
    $objects = isset($this->objects) ? $this->objects : array();
    if (isset($this->parent)) {
      $objects += $this->parent->get_objects();
    }
    return $objects;
  }  
  
  /**
   * Get element from elements or default texts
   */
  function get_element($type, $options = array()) {
    if (isset($this->elements[$type])) {
      return $this->elements[$type];
    }
    else {
      return $this->get_text($type, $options);
    }
  }
  /**
   * Get text element from this template
   */
  public function get_text($type, $options = array()) {
    if (isset($this->text[$type])) {
      return $this->text[$type];
    }
    else {
      $options += $this->get_options();
      return $this->default_text($type, $options);
    }
  }
  /**
   * Get options for texts, translations, etc
   */
  function get_options() {
    if (!isset($this->options)) {
      $this->set_language(language_default());
    }
    return $this->options;
  }
  /**
   * Get tokens for templates
   */
  function get_tokens() {
    if (!isset($this->tokens)) {
      $this->tokens = array();
      // Use template options but don't clear tokens
      $options = $this->get_options();
      $objects = $this->get_objects();
      // Build token groups to optimize module calls
      $token_groups = array();
      foreach ($this->token_list() as $token) {
        list($type, $name) = explode(':', $token);
        // Example $tokens['site']['name'] = 'site:name'
        $token_groups[$type][$name] = $token;
        // The token defaults to itself if it can't be replaced yet
        $this->tokens[$token] = '[' . $token . ']';
      }
      foreach ($token_groups as $type => $tokens) {
        $type_tokens = token_generate($type, $tokens, $objects, $options);
        $this->tokens = $type_tokens + $this->tokens;
      }
    }
    return $this->tokens;
  }
  /**
   * Set language
   */
  function set_language($language) {
    $this->set_option('language', $language);
    $this->set_option('langcode', $language->language);
    $this->reset();
    return $this;
  }
  /**
   * Set options
   */
  function set_option($name, $value = TRUE) {
    $this->options[$name] = $value;
    return $this;
  }
  /**
   * Set array of options
   */
  function set_options($options = array()) {
    $this->options = array_merge($this->options, $options);
    return $this;
  }

  /**
   * Do token replacement with this template's objects
   */
  public function token_replace($text, $options = array()) {
    return token_replace($text, $this->get_objects(), $options + $this->get_options());
  }
  /**
   * Get default elements
   */
  protected function default_elements() {
    return array();
  }
  /**
   * Default texts for this template, may need token replacement
   */
  protected function default_text($type, $options) {
    // Text not found, something went wrong with our template processing
    return t('Template text not found: @type.', array('@type' => $type), $options);
  }
  /**
   * Tokens for this template. Will be stored
   */
  protected function token_list() {
    return array(
      'site:name',
      'site:url',
    );
  }
}

/**
 * Template for a full message (subject, body, etc..)
 */
class Messaging_Message_Template extends Messaging_Template implements Messaging_Message_Render {
  public $method = 'default';

  /**
   * Set message elements
   */
  protected function get_parts() {
    return array('subject', 'body');
  }
  /**
   * Get Message_Object with this template linked
   */
  public function build_message($options = array()) {
    $message = new Messaging_Message($options);
    $message->set_template($this);
    return $message;
  }
  /**
   * Set destination (and reset built elements)
   */
  function set_destination($destination) {
    $this->add_object('destination', $destination);
    $this->add_object('user', $destination->get_user());
    $this->reset();
    return $this;
  }
  /**
   * Set text format
   */
  public function set_format($format) {
    $this->format = $format;
    $this->reset();
    return $this;
  }
  /**
   * Set sending method
   */
  public function set_method($method) {
    $this->method = $method;
    $this->reset();
    return $this;
  }
  /*
   * Get defaults for elements
  */
  protected function element_defaults($name) {
    switch ($name) {
      case 'subject':
        $defaults = array('#type' => 'messaging_template_subject');
        break;
      case 'body':
        $defaults = array('#type' => 'messaging_template_body', '#parts' => array('header', 'content', 'footer'));
        break;
      case 'header':
      case 'content':
      case 'footer':
        $defaults = array('#type' => 'messaging_template_text');
        break;
      default:
        $defaults = array();
    }
    return $defaults + parent::element_defaults($name);
  }
  /**
   * Default texts for this template, may need token replacement
   */
  protected function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return array('#tokens' => TRUE, '#markup' => t('Message from [site:name]', array(), $options));
      case 'footer':
        return array(
          '#type' => 'messaging_link', '#tokens' => TRUE,
          '#text' => t('Message from [site:name]', array(), $options),
          '#url' => '[site:url]',
        );
      default:
        return parent::default_text($type, $options);
    }
  }
}

/**
 * Theme messaging text
 */
function theme_messaging_template_text($variables) {
  $element = $variables['element'];
  $text = array();
  if (!empty($element['#title'])) {
    $text[] = $element['#title'];
  }
  if (!empty($element['#markup'])) {
    $text[] = $element['#markup'];
  }
  
  foreach (element_children($element) as $key) {
    $text[] = is_array($element[$key]) ? drupal_render($element[$key]) : $element[$key];
  }
  return implode($element['#linebreak'], $text);
}

/**
 * Theme message subject. Does nothing but can be overridden
 */
function theme_messaging_template_subject($variables) {
  $element = $variables['element'];
  return $element['#children'];
}

/**
 * Theme message body
 * 
 * This is just a wrapper to decide which is the right template to use
 */
function theme_messaging_template_body($variables) {
  $element = $variables['element'];
  // First look for an alternate for sending method
  $themes[] = 'messaging_template_body_' . $element['#method'];
  switch ($element['#format']) {
    case MESSAGING_FORMAT_HTML:
      $themes[] = 'messaging_template_body_html';
      break;
    case MESSAGING_FORMAT_PLAIN:
    default:
      $themes[] = 'messaging_template_body_plain';
  }
  return theme($themes, $variables);
}
