<?php
/**
 * @file
 *   User pages for User Interface for subscriptions modules
 */

/**
 * Page callback: Add subscription form
 */
function notifications_ui_page_add_subscription($type = NULL) {
  $account = $GLOBALS['user'];
  $subscription = notifications_subscription($type)
    ->instance()
    ->set_user($account);
  return drupal_get_form('notifications_subscription_form', 'subscribe', $subscription);
}
