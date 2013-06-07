<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Taxonomy term object
 */
class Notifications_Taxonomy_Term extends Notifications_Object {
  public $type = 'taxonomy_term';

  /**
   * Title
   */
  public function get_title() {
    return t('Taxonomy term');
  }
  /**
   * Token type
   */
  public function get_token_type() {
    return 'term';
  }
  /**
   * Load object
   */
  public static function object_load($value) {
    return taxonomy_term_load($value);
  }
  /**
   * Get name for object
   */
  public static function object_name($term) {
    return $term->name;
  }
  /**
   * Get value for object
   */
  public static function object_value($term) {
    return $term->tid;
  }
}

/**
 * Taxonomy term field
 */
class Notifications_Taxonomy_Term_Field extends Notifications_Field_Autocomplete {
  public $type = 'term:tid';
  public $object_type = 'taxonomy_term';
  /**
   * Get field name
   */
  function get_title() {
    return t('Term');
  }
  /**
   * Get description
   */
  function get_description() {
    return t('Taxonomy term');
  }
  /**
   * Get system path
   */
  function get_path() {
    return isset($this->value) ? 'taxonomy/term/' . $this->value : '';
  }
   /**
   * Get autocomplete path
   */
  function autocomplete_path() {
    return 'notifications_tags/autocomplete/simple';
  }

  /**
   * Parse value from autocomplete, get the first matching term
   */
  function autocomplete_parse($string) {
    if ($terms = taxonomy_get_term_by_name($string)) {
      $term = reset($terms);
      return $term->tid;
    }
  }
  /**
   * Get value/s from object
   */
  function object_value($object) {
    if ($object->type == 'node') {
      // If the object is a node, get all tids for it
      $node = $object->get_object();
      return array_keys(notifications_tags_node_terms($node));
    }
    else {
      return parent::object_value($object);
    }
  }
}

class Notifications_Taxonomy_Vocabulary_Subscription extends Notifications_Subscription_Simple {
  /**
   * Get proper name
   */
  public function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif ($vocabulary = $this->get_field('vocabulary:vid')->drupal_object()) {
      return t('Terms in @name', array('@name' => $vocabulary->name));
    }
    else {
      parent::get_name();
    }
  }
}


/**
 * Taxonomy vocabulary object
 */
class Notifications_Taxonomy_Vocabulary extends Notifications_Object {
  public $type = 'taxonomy_vocabulary';

  /**
   * Title
   */
  public function get_title() {
    return t('Vocabulary');
  }
  /**
   * Token type
   */
  public function get_token_type() {
    return 'vocabulary';
  }
  /**
   * Load object
   */
  public static function object_load($value) {
    return taxonomy_vocabulary_load($value);
  }
  /**
   * Get name for object
   */
  public static function object_name($vocabulary) {
    return $vocabulary->name;
  }
  /**
   * Get value for object
   */
  public static function object_value($vocabulary) {
    return $vocabulary->vid;
  }
}

/**
 * Taxonomy term field
 */
class Notifications_Taxonomy_Vocabulary_Field extends Notifications_Field_Select {
  public $type = 'vocabulary:vid';
  public $object_type = 'taxonomy_vocabulary';
  protected static $vocabulary_names;
  /**
   * Get field name
   */
  function get_title() {
    return t('Vocabulary');
  }

  /**
   * Get description
   */
  function get_description() {
    return t('Taxonomy vocabulary');
  }

  /**
   * Get system path
   */
  function get_path() {
    return isset($this->value) ? 'taxonomy/vocabulary/' . $this->value : '';
  }

  /**
   * List of options
   */
  function select_options() {
    if (!isset($this->vocabulary_names)) {
      //foreach (taxonomy_get_vocabularies() as $vocab) {
      $this->vocabulary_names = db_query('SELECT vid, name FROM {taxonomy_vocabulary}')->fetchAllKeyed();
    }
    return $this->vocabulary_names;
  }
}


/**
 * Subscription to simple taxonomy term
 */
class Notifications_Taxonomy_Term_Subscription extends Notifications_Subscription_Simple {
  /**
   * Get proper name
   */
  public function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif ($term = $this->get_field('term:tid')->drupal_object()) {
      return t('Content tagged with @term', array('@term' => $term->name));
    }
    else {
      parent::get_name();
    }
  }
  /**
   * Add term object
   */
  public function add_term($term) {
    $this->add_field('term:tid', $term->tid);
    return $this;
  }
  /**
   * Add term by name (and optional vid)
   */
  public function add_term_by_name($name, $vid = NULL) {
    $conditions = array('name' => trim($name));
    if ($vid) {
      $conditions['vid'] = $vid;
    }
    if ($terms = taxonomy_term_load_multiple(array(), $conditions)) {
      $this->add_term(reset($terms));
    }
    return $this;
  }
}

/**
 * Subscription to conent type + taxonomy term
 */
class Notifications_Content_Type_Term_Subscription extends Notifications_Taxonomy_Term_Subscription {
  /**
   * Get proper name
   */
  public function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif (($term = $this->get_field('term:tid')) && ($type = $this->get_field('node:type'))) {
      return t('@type posts tagged with @term', array('@type' => $type->get_name(), '@term' => $term->get_name()));
    }
    else {
      parent::get_name();
    }
  }
}

/**
 * Subscription to simple taxonomy term
 */
class Notifications_Taxonomy_Term_Multiple_Subscription extends Notifications_Subscription_Multiple {
  /**
   * Get proper name
   */
  public function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif ($terms = $this->get_fields()) {
      foreach ($terms as $term_field) {
        $names[] = $term_field->get_name();
      }
      return t('Content tagged with @terms', array('@terms' => implode(', ', $names)));
    }
    else {
      parent::get_name();
    }
  }
  /**
   * Add term object
   */
  public function add_term($term) {
    return $this->add_field('term:tid', $term->tid);
  }
  /**
   * Add term by name (and optional vid)
   */
  public function add_term_by_name($name, $vid = NULL) {
    $conditions = array('name' => trim($name));
    if ($vid) {
      $conditions['vid'] = $vid;
    }
    if ($terms = taxonomy_term_load_multiple(array(), $conditions)) {
      $this->add_term(reset($terms));
    }
    return $this;
  }
}

/**
 * Node events
 */
class Notifications_Taxonomy_Term_Create_Event extends Notifications_Event {
  /**
   * Set main object id and vocabulary id when term is added
   */
  public function set_object($object) {
    if ($object->type == 'taxonomy_term') {
      $this->oid = $object->get_value();
      $this->add_object('taxonomy_vocabulary', $object->get_object()->vid);
    }
    return parent::set_object($object);
  }

  /**
   * Trigger node event
   */
  public function trigger() {
    $term = $this->get_object('taxnomy_term');
    if ($result = parent::trigger()) {
      watchdog('action', 'Triggered notifications for new taxonomy term @name..', array('@name' => $term->name));
    }
    return $result;
  }
}

/**
 * Message templates for node events. Base class.
 */
class Notifications_Taxonomy_Term_Create_Template extends Notifications_Message_Template {
  /**
   * Default texts for this template, may need token replacement
   */
  function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return array(
          '#tokens' => TRUE,
          '#markup' => t('Update for [vocabulary:name]', array(), $options),
        );
      case 'content':
        return array(
          '#type' => 'messaging_template_text',
          '#tokens' => TRUE,
          'header' => t('Update for [vocabulary:name]:', array(), $options),
          'teaser' => 'A new term has been created: [term:name]',
          'more' => array(
            '#type' => 'messaging_link',
            '#text' => t('Read more', array(), $options),
            '#url' => '[term:url]',
          ),
        );
      case 'digest':
        return t('New term [term:name] created for vocabulary [vocabulary:name].', array(), $options);
      default:
        return parent::default_text($type, $options);
    }
  }
}
