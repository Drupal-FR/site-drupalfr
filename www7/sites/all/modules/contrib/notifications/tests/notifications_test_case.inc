<?php

// Base class and some utility functions need this one
require_once drupal_get_path('module', 'messaging') . '/tests/messaging_test_case.inc';

/**
 * @file
 * Base class for Notifications Tests
 */
class NotificationsTestCase extends MessagingTestCase {
  /**
   * Set up some required modules
   */
  function setUp() {
    $modules = func_get_args();
    $modules = array_unique(array_merge(array('token', 'messaging_template', 'notifications'), $modules));
    call_user_func_array(array('parent', 'setUp'), $modules);
  }
  /**
   * Helper function. Simple row counting with conditions, uses query builder
   */
  function countQueued($params = NULL) {
    return Notifications_Queue::queue_count($params);
  }
  /**
   * Helper function to create a subscription
   */
  function contentCreateSubscription($user, $type, $fields, $confirm_texts = array(), $post = array()) {
    // Create a link for the subscription confirmation page
    $link = notifications_get_link('subscribe', array('uid' => $user->uid, 'type' => $type, 'fields' => $fields));
    $this->drupalGet($link['href'], $link['options']);
    $this->assertText(t('Confirm your subscription'), 'Subscriptions confirmation page is shown');
    foreach ($confirm_texts as $text) {
      $this->assertRaw($text);
    }
    // Submit the form and check confirmation page
    $this->drupalPost(NULL, $post, t('Subscribe'));
    $this->assertText(t('Your subscription has been created.'));
    $subs = notifications_get_subscriptions(array('uid' => $user->uid, 'type' => $type), $fields);
    $subscription = array_shift($subs);
    $this->assertTrue($subscription, "The subscription object can be retrieved from the db.");
    return $subscription;
  }
  // Enable all UI optional pages
  function enableUIPages($enable = TRUE) {
    $settings = array_keys(notifications_subscription_types());
    variable_set('notifications_ui_types', $enable ? $settings : array());
    variable_set('notifications_ui_user_options', $enable ? array('page', 'create'): array());
    menu_rebuild();
  }
  // Enable content Subscriptions for all
  function enableSubscriptionTypes($enable = TRUE) {
    $settings = array_keys(notifications_subscription_types());
    variable_set('notifications_content_type', $enable ? $settings : array());
  }
  // Create comment
  function drupalCreateComment($node, $comment = array()) {
     $comment += array(
      'subject' => $this->randomName(8),
      'comment' => $this->randomName(32),
      'uid' => $node->uid,
      'nid' => $node->nid,
      'status' => COMMENT_PUBLISHED,
      'cid' => 0, 'pid' => 0, 'format' => FILTER_FORMAT_DEFAULT,
    );
    $cid = comment_save($comment);
    $this->assertTrue($cid, "Successfully created comment: $cid for node $node->nid ($node->type).");
    return _comment_load($cid);
  }
  // Process queued notifications
  function notificationsProcessQueue($before = 0, $after = 0) {
    if ($before) {
      $this->assertTableRows('notifications_queue', $before);
    }
    $count = notifications_queue()->process_run();
    $this->assertTrue($count, "Processed $count rows from notifications queue.");
    $this->assertTableRows('notifications_queue', $after);
  }
}
