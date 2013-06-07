<?php
/**
 * @file
 * User subscriptions for Notifications
 */

/**
 * Generic content subscription Thread subscription
 */
class Notifications_User_Content_Subscription extends Notifications_Content_Subscription {
  /**
   * Set all the fields we can from node
   */
  public function set_node($node) {
    parent::set_node($node);
    $this->set_author($node->uid);
    return $this;
  }
  /**
   * Set author (user)
   */
  public function set_author($user) {
    $uid = is_object($user) ? $user->uid : $user;
    $this->get_field('node:uid')->set_value($uid);
    return $this;
  }
  /**
   * Get author (user field)
   */
  public function get_author() {
    return $this->get_field('node:uid');
  }
  /**
   * Get name
   */
  function get_name() {
    if (isset($this->name)) {
      return $this->name;
    }
    else {
      $author_name = $this->get_author()->get_name();
      if ($type = $this->get_field('node:type')) {
        return t('@type posts by @author', array('@type' => $type->get_name(), '@author' => $author_name));
      }
      else {
        return t('All posts by @author', array('@author' => $author_name));
      }
    }
  }
}

