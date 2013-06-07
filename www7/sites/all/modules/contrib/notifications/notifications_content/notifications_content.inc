<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * Generic content subscription Thread subscription
 */
class Notifications_Content_Subscription extends Notifications_Subscription_Simple {
  /**
   * Set all the fields we can from node
   */
  public function set_node($node) {
    $node_mapping = array(
      'node:nid' => $node->nid,
      'node:type' => $node->type,
      'node:uid' => $node->uid,
    );
    foreach ($node_mapping as $index => $value) {
      if ($field = $this->get_field($index)) {
        $field->set_value($value);
      }
    }
    return $this;
  }
  /**
   * Get name
   */
  function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    else {
      $type_name = ($type = $this->get_field('node:type')) ? $type->get_name() : t('Content');
      if ($author = $this->get_field('node:uid')) {
        return t('@type posts by @author', array('@type' => $type_name, '@author' => $author->get_name()));
      }
      else {
        return t('@type posts', array('@type' => $type_name));
      }
    }
  }
}

/**
 * Thread subscription
 */
class Notifications_Node_Subscription extends Notifications_Content_Subscription {
  /**
   * Get a proper name for this type
   */
  function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    elseif ($node = $this->get_field('node:nid')->drupal_object()) {
      // If we've got the node, return node type and title
      return t('@node-type: @node-title', array('@node-type' => node_type_get_name($node), '@node-title' => $node->title));
    }
    else {
      return parent::get_name();
    }
  }
}

/**
 * Node events
 */
class Notifications_Node_Event extends Notifications_Event {
  /**
   * Set main object id when node is added to the event
   */
  public function set_object($object) {
    if ($object->type == 'node') {
      $this->oid = $object->get_value();
    }
    return parent::set_object($object);
  }
  /**
   * Get node object
   */
  public function get_node() {
    return $this->get_object('node')->get_object();
  }
  /**
   * Trigger node event
   */
  public function trigger() {
    $node = $this->get_node();
    if ($result = parent::trigger()) {
      watchdog('action', 'Triggered notifications for  @type %title..', array('@type' => node_type_get_name($node), '%title' => $node->title));
    }
    return $result;
  }
}

class Notifications_Comment extends Notifications_Object {
  public $type = 'comment';

  public function get_title() {
    return t('Comment');
  }
  /**
   * Check user access
   */
  function user_access($account) {
    if ($comment = $this->get_object()) {
      return $comment->status == COMMENT_PUBLISHED && user_access('access comments', $account) || user_access('administer comments', $account);
    }
  }
  /**
   * Load related object or data
   */
  public static function object_load($value) {
    return comment_load($value);
  }
  /**
   * Get object name, unfiltered string
   */
  public static function object_name($object) {
    return $comment->title;
  }
  /**
   * Get object key
   */
  public static function object_value($object) {
    return $object->cid;
  }
}

/**
 * Node type object
 */
class Notifications_Node_Type extends Notifications_Drupal_Object {
  public $type = 'node_type';

  public function get_title() {
    return t('Content type');
  }
  /**
   * Load related object or data
   */
  public static function object_load($value) {
    return node_type_get_type($value);
  }
  /**
   * Get object name, unfiltered string
   */
  public static function object_name($object) {
    return $object->name;
  }
  /**
   * Get object key
   */
  public static function object_value($object) {
    return $object->type;
  }
}

/**
 * Node type field
 */
class Notifications_Node_Type_Field extends Notifications_Field_Select {
  public $type = 'node:type';
  public $object_type = 'node_type';
  protected $data_type = 'varchar';
  public function get_title() {
    return t('Content type');
  }
  public function get_description() {
    return t('Content type');
  }
  /**
   * Select options, content type list
   */
  public function select_options() {
    return node_type_get_names();
  }
  /**
   * Get value from node or from node type
   */
  public function object_value($object) {
    if ($object->type == 'node' && $node = $object->get_object()) {
      return $node->type;
    }
    elseif ($object->type == 'node_type') {
      return $object->get_value();
    }
  }
}

/**
 * Event types: When a (published) node is created or an existing node is published
 */
class Notifications_Node_Post_Event extends Notifications_Node_Event {
  public function get_name() {
    return t('New content of type [node:type-name] has been submitted');
  }
}
class Notifications_Node_Update_Event extends Notifications_Node_Event {
  public function get_name() {
    return t('The [node:type-name] has been updated');
  }
}
class Notifications_Node_Comment_Event extends Notifications_Node_Event {
  public function get_name() {
    t('New comment by [comment-author-name]: [comment-title]');
  }

}

/**
 * Message templates for node events. Base class.
 */
class Notifications_Node_Event_Template extends Notifications_Message_Template {
  /**
   * Default texts for this template, may need token replacement
   */
  function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return array(
          '#tokens' => TRUE,
          '#markup' => t('Update for [node:type-name]: [node:title]', array(), $options)
        );
      case 'content':
        return array(
          '#type' => 'messaging_template_text', '#tokens' => TRUE,
          'header' => t('Update for [node:type-name]: [node:title]', array(), $options),
          'teaser' => '[node:summary]',
          'more' => array(
            '#type' => 'messaging_link',
            '#text' => t('Read more', array(), $options),
            '#url' => '[node:url]',
          ),
        );
      case 'digest':
        return array(
          '#tokens' => TRUE,
          'title' => '[node:title]',
          'more' => t('Read more [node:url]', array(), $options),
        );
      default:
        return parent::default_text($type, $options);
    }
  }
}

/**
 * Template for node inserts
 */
class Notifications_Node_Insert_Template extends Notifications_Node_Event_Template {
  public function get_title() {
    return t('Template for node posts');
  }
  public function digest_fields() {
    return array('node:type', 'author:uid');
  }
  /**
   * New nodes can be digested by type or by author
   */
  public function digest_line($field, $options = array()) {
    switch ($field) {
      case 'node:type':
        return t('[node:title] by [author-name].', array(), $options);
      case 'author:uid':
        return t('New [node:type-name]: [node:title]', array(), $options);
    }
  }
    /**
   * Default texts for this template, may need token replacement
   */
  function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return array(
          '#tokens' => TRUE,
          '#markup' => t('New [node:type-name]: [node:title]', array(), $options),
        );
      default:
        return parent::default_text($type, $options);
    }
  }
}

/**
 * Template for node updates
 */
class Notifications_Node_Update_Template extends Notifications_Node_Event_Template {
  public function get_title() {
    return t('Template for node updates');
  }
  public function digest_fields() {
    return array('node:nid', 'author:uid');
  }
  public function digest_line($field, $options = array()) {
    switch ($field) {
      case 'node:nid':
        return t('The [node:type-name] has been updated by [user:name]', array(), $options);
      case 'author:uid':
        return t('Updated [node:type-name]: [node:title]', array(), $options);
    }
  }
}

/**
 * Template for node comments
 */
class Notifications_Node_Comment_Template extends Notifications_Node_Event_Template {
  public function get_title() {
    t('Template for node comments');
  }
  public function digest_fields() {
    return array('node:nid');
  }
  public function digest_line($field, $options = array()) {
    switch ($field) {
      case 'node:nid':
        return t('New comment by [comment:name]: [comment:title]', array(), $options);
      case 'author:uid':
        return t('Commented on [node:type-name]: [node:title]', array(), $options);
    }
  }
  /**
   * Default texts for this template, may need token replacement
   */
  function default_text($type, $options) {
    switch ($type) {
      case 'subject':
        return array(
          '#tokens' => TRUE,
          '#markup' => t('Comment for [node:type-name]: [node:title]', array(), $options),
        );
      case 'content':
        return array(
          '#tokens' => TRUE,
          'header' => t('Comment by [comment:name]: [comment:title]', array(), $options),
          'content' => '[comment:body]',
          'footer' => t('Read more [comment:url]', array(), $options),
        );
      case 'digest':
        return array(
          '#tokens' => TRUE,
          'title' => '[node:title]',
          'more' => t('Read more [node:url]', array(), $options),
        );
      default:
        return parent::default_text($type, $options);
    }
  }
}

/**
 * Template for a list of nodes, list of title, links
 */
class Notifications_Content_Node_List extends Notifications_Message_Template {
  /**
   * Get node list
   */
  function node_list() {
    return !empty($this->content) ? $this->content : array();
  }

  /**
   * Subject text
   */
  protected function text_subject($options) {
    return array(
      '#tokens' => TRUE,
      '#markup' => t('Latest posts from [site:name]', array(), $options),
    );
  }
  /**
   * Content text
   */
  protected function text_content($options) {
    // @todo There should be a way to format this as an item list
    $content['list'] = array(
      '#title' => t('These are the latest posts:'),
      '#type' => 'messaging_list',
    );
    foreach ($this->node_list() as $node) {
      $content['list'][$node->nid] = array(
        '#type' => 'messaging_link',
        '#title' => $node->title,
        '#href' => 'node/' . $node->nid,
        '#options' => array('absolute' => TRUE),
      );
    }
    return $content;
  }
}

/**
 * Template for a list of nodes, teasers
 */
class Notifications_Content_Node_Teasers extends Notifications_Content_Node_List {
   /**
   * Default texts for this template, may need token replacement
   */
  protected function text_content($options) {
    return node_view_multiple($this->node_list(), 'notifications');
  }
}

/**
 * Template for a list of nodes, built with a View
 *
 * As we cannot store a View object, because we get a serialization error (PDOException)
 * we need to store only some parts of the view (content, title, etc...)
 */
class Notifications_Content_Node_View extends Notifications_Content_Node_List {
  protected $view_name = 'notifications_content_node_list';
  protected $view_content;

  /**
   * Set event and view name
   */
  public function set_event($event) {
    parent::set_event($event);
    // View may be set into action parameters, or we may get it from template info
    if ($view_name = $event->get_action_context('view')) {
      $this->view_name = $view_name;
    }
    elseif (isset($this->info['view'])) {
      $this->view_name = $this->info['view'];
    }
    return $this;
  }
  /**
   * Content text.
   *
   * @todo Better way to render view
   */
  function text_content($options) {
    if (!isset($this->view_content)) {
      $view = $this->get_view();
      // Weird: if we pass same objects, the view alters some of them, causing it to reference to PDO Object
      // Which causes an exception if trying to serialize. So we need to unlink the view from the node after render()
      $node_list = $this->node_list();
      $view->result = $node_list;
      $view->executed = TRUE;
      $this->view_content = $view->render();
      foreach ($node_list as $node) {
        unset($node->view);
      }
    }
    return $this->view_content;
  }

  /**
   * Get view
   */
  function get_view() {
    return views_get_view($this->view_name);
  }
}
