<?php
/**
 * @file
 * Basic message templates and theming
 */
/**
 * Interface for all message templates
 */
interface Messaging_Message_Render {
  /**
   * Add item of unknown type
   */
  public function add_element($name, $element);
  
  /**
   * Build message parts as renderable array
   */
  public function build();
  /**
   * Render message parts
   */
  public function render();
  /**
   * Set text format
   */
  public function set_format($format);
  /**
   * Set single option
   */
  public function set_option($name, $value = TRUE);
  /**
   * Set message destination
   */
  public function set_destination($destination);
  /**
   * Set sending method
   */
  public function set_method($method);
}

/**
 * Very simple template with plain subject, header, content, footer texts
 */
class Messaging_Message_Text implements Messaging_Message_Render {
  public $elements = array();
  public $options = array();
  // Default format and send method
  public $format = MESSAGING_FORMAT;
  public $method = 'default';

  /**
   * Add item of unknown type (string, renderable array)
   */
  public function add_element($name, $element) {
    $this->elements[$name] = is_array($element) ? $element : array('#markup' => $element);
  }
  /**
   * Build message parts as renderable array
   */
  public function build() {
    $args = func_get_args();
    $args = $args ? $args : array('subject', 'body');
    return $this->build_parts($args) ;
  }
  /**
   * Build array of message parts
   */
  protected function build_parts($parts) {
    $build = array();
    foreach ($parts as $name) {
      if ($name == 'body') {
        $build['body'] = $this->build('header', 'content', 'footer');
      }
      else {
        $build[$name] = isset($this->elements[$name]) ? $this->elements[$name] : array();
      }
    }
    return $build + array('#type' => 'messaging_text', '#options' => $this->options, '#format' => $this->format, '#method' => $this->method);    
  }
  /**
   * Render message parts
   */
  public function render() {
    $args = func_get_args();
    $args = $args ? $args : array('subject', 'body');
    $build = $this->build_parts($args);
    return drupal_render($build);
  } 

  /**
   * Set text format, just change separator
   */
  public function set_format($format) {
    $this->format = $format;
    switch ($this->format) {
      case MESSAGING_FORMAT_PLAIN:
        $this->set_option('linebreak', "\n");
        break;
      case MESSAGING_FORMAT_HTML:
        $this->set_option('linebreak', '<br />');
        break;
    }
    return $this;
  }
  /**
   * Set single option
   */
  public function set_option($name, $value = TRUE) {
    $this->options[$name] = $value;
    return $this;
  }
  /**
   * Set message destination (and reset built elements)
   */
  public function set_destination($destination) {
    // Do nothing
    return $this;
  }
  /**
   * Set sending method
   */
  public function set_method($method) {
    $this->method = $method;
    return $this;
  }
}

/**
 * Theme messaging text
 */
function theme_messaging_text($variables) {
  $element = $variables['element'];
  $options = $element['#options'];
  $options += array('linebreak' => "\n");
  $text = array();
  foreach (element_children($element) as $key) {
    $text[] = is_array($element[$key]) ? drupal_render($element[$key]) : $element[$key];
  }
  return implode($options['linebreak'], $text);
}