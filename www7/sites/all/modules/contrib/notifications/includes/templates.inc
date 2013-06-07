<?php
/**
 * @file
 *   Notifications templates
 */

/**
 * Get notifications templates
 *
 * @param $op
 *   Operation or type of template information we want
 *   'info', 'parts', 'tokens', 'defaults'
 * @param $type
 *   Can be a template key or
 *   - 'all', to get all
 *   - 'enabled', to filter out templates for disabled event types
 */
function notifications_get_templates($op = 'info', $type = 'all', $language = NULL) {
  static $all_templates;

  switch ($op) {
    case 'info':
      if ($type == 'all' || $type == 'enabled') {
        if (!isset($all_templates)) {
          $all_templates = module_invoke_all('notifications_templates', 'info', 'all');
        }
        if ($type == 'all') {
          return $all_templates;
        }
        elseif ($type == 'enabled') {
          // Get a list of templates for enabled event types
          $enabled = array_filter(notifications_event_type(NULL, 'template'));
          return messaging_template_extract_group($enabled, $all_templates);
        }
      }
      else {
        return module_invoke_all('notifications_templates', 'info', $type, $language);
      }
      break;
    default:
      return module_invoke_all('notifications_templates', $op, $type, $language);
  }
}

/**
 * Implementation of hook_messaging_templates()
 *
 * This will be actually included and called from notifications_messaging() when needed
 *
 * For some of these templates, we don't want them to be filtered by enabled events so we provide them
 * straight to messaging module. The others will be returned by notifications_get_templates()
 *
 * Depending on $op, this hook takes different parameters and returns different pieces of information:
 *
 * - 'message groups'
 *   Get array of message groups, each of which will have one or more keys for different templates
 *   Each group should have a unique key, so it should start with the module name
 * - 'message keys'
 *   Get message template parts for a given group ($arg1)
 *   Return array of key => name for each part
 * - 'messages'
 *   Get default message templates for a given group ($arg1).
 *   It should return default texts, indexed by message key that will be the default templates
 *   These templates may be edited on the 'Messaging templates' page
 * - 'tokens'
 *   Get available tokens for a given message group and key ($arg1).
 *   Return array of token keys that will be available for this message templates
 *   The tokens themselves may be default tokens (provided by token module) or we can add new
 *   tokens implementing hook_token_list() and hook_token_value()
 *
 * @param $op
 *   Operation, type of information to retrieve
 * @param $arg1, $arg2...
 *   Different parameters depending on $op
 */
function notifications_messaging_templates($op = 'info' , $type = 'all', $language = NULL) {
  switch ($op) {
    case 'message groups':
      $type = $type ? $type : 'all';
      // First compile enabled templates only from hook_notifications_templates()
      $info = notifications_get_templates('info', 'enabled');
      if ($type == 'all' || $type == 'notifications-message') {
         $info['notifications-message'] = array(
          'module' => 'notifications',
          'name' => t('Notifications messages'),
          'description' => t('These are the common parts for one time messages sent by Notifications. These are not triggered by subscriptions and events. They are replies to operations requested by a user.'),
          'fallback' => FALSE, // This template has no fallback
        );
      }
      if ($type == 'all' || $type == 'notifications-user-unsubscribe') {
        $info['notifications-user-unsubscribe'] = array(
          'module' => 'notifications',
          'name' => t('Cancel user subscriptions'),
          'description' => t('It should contain a link to cancel all subscriptions for a user.'),
          'fallback' => 'notifications-message',
        );
      }
      if ($type == 'all' || $type == 'notifications-destination-unsubscribe') {
         $info['notifications-destination-unsubscribe'] = array(
          'module' => 'notifications',
          'name' => t('Cancel destination subscriptions'),
          'description' => t('It should contain a link to cancel all subscriptions for a destination address.'),
          'fallback' => 'notifications-message',
        );
      }
      return $info;

    case 'message keys':
      return notifications_get_templates('parts', $type);
    case 'messages':
      // The second argument should be language object
      return notifications_get_templates('defaults', $type, $language);
    case 'tokens':
      return notifications_get_templates('tokens', $type);
  }
}

/**
 * Implementation of hook_notifications_templates()
 */
function notifications_notifications_templates($op = 'info', $type = 'all', $language = NULL) {
  switch ($op) {
    case 'info':
      $info = array();
      // Generic notifications event
      if ($type == 'all' || $type == 'notifications-event') {
        $info['notifications-event'] = array(
          'module' => 'notifications',
          'name' => t('Notifications event'),
          'description' => t('Common parts for all Notifications messages for a single event. This is useful for defining a common header and/or footer for all these messages.'),
          'fallback' => FALSE, // This template has no fallback
        );
      }
      if ($type == 'digest' || $type == 'notifications-digest') {
        $info['notifications-digest'] = array(
          'module' => 'notifications',
          'name' => t('Notifications digest'),
          'description' => t('Depending on your settings for each Send interval, Notifications may be digested, this is grouped and summarized in a single message. These are the common parts for Notifications digests.'),
          'fallback' => FALSE, // This template has no fallback
        );
      }
      return $info;
    case 'parts':
      switch ($type) {
        case 'notifications-event':
        case 'notifications-message':
        case 'notifications-user-unsubscribe':
        case 'notifications-destination-unsubscribe':
          // Event notifications
          return array(
            'subject' => t('Subject'),
            'header' => t('Header'),
            'main' => t('Content'),
            'footer' => t('Footer'),
          );
        case 'notifications-digest':
          return array(
            'subject' => t('Subject'),
            'header' =>  t('Header'),
            'main' => t('Line for digested events'),
            'closing' => t('Group closing'),
            'footer' => t('Footer'),
          );
      }
      break;
    case 'defaults':
      switch ($type) {
      // Event notifications
        case 'notifications-event':
          return array(
            'subject' => t('Event notification for [user] from [site-name]', array(), $language->language),
            'header' => t("Greetings [user],", array(), $language->language),
            'main' => t("A item to which you are subscribed has been updated", array(), $language->language),
            'footer' => array(
                t('This is an automatic message from [site-name]', array(), $language->language),
                t('To manage your subscriptions, browse to [subscriptions-manage]', array(), $language->language),
                t('You can unsubscribe at [unsubscribe-url]', array(), $language->language),
            ),
          );
        case 'notifications-message':
          return array(
            'subject' => t('This is a message for [user] from [site-name]', array(), $language->language),
            'header' => t("Greetings [user],", array(), $language->language),
            'footer' => array(
                t('This is an automatic message from [site-name]', array(), $language->language),
            ),
          );
        // Digested messages
        case 'notifications-digest':
          return array(
            'subject' => t('[site-name] subscription update for [user]', array(), $language->language),
            'header' => t("Greetings, [user].\n\nThese are your messages", array(), $language->language),
            'main' => t("A [type] has been updated: [title]\n\n[event_list]", array(), $language->language),
            'closing' => '...',
            'footer' => array(
              t('This is an automatic message from [site-name]', array(), $language->language),
              t('To manage your subscriptions, browse to [subscriptions-manage]', array(), $language->language),
              // There is not single unsubscribe url as these usuarlly contain multiple subscriptions
            ),
          );
        case 'notifications-user-unsubscribe':
          return array(
            'subject' => t('Cancel all subscriptions to [site-name]', array(), $language->language),
            'header' => t("Greetings [user],\n\nSomeone has requested to cancel all subscriptions for your account.", array(), $language->language),
            'main' => array(
              t("To cancel all your subscriptions, browse to [unsubscribe-url-global]", array(), $language->language),
              t('To edit your subscriptions, browse to [subscriptions-manage]', array(), $language->language),
            ),
          );
        case 'notifications-destination-unsubscribe':
          return array(
            'subject' => t('Cancel all subscriptions to [site-name]', array(), $language->language),
            'header' => t("Greetings from [site-name]\n\nSomeone has requested to cancel all subscriptions for [destination-address]", array(), $language->language),
            'main' => array(
              t("To cancel all your subscriptions, browse to [destination-unsubscribe-url]", array(), $language->language),
              t('To edit your subscriptions, browse to [destination-manage-url]', array(), $language->language),
            ),
          );
      }
      break;
    case 'tokens':
      $args = explode('-', $type);
      $tokens = array();
      switch ($type) {
        case 'notifications-user-unsubscribe':
          // This one has all of the following two plus 'user'
          $tokens[] = 'user';
        case 'notifications-destination-unsubscribe':
        case 'notifications-message':
          $tokens[] = 'destination';
          break;

        default:
          // These are the token groups that will be used for this module's messages
          if ($args[0] == 'notifications') {
            $tokens = array('subscription', 'user');
            if ($args[1] == 'event') {
              // If the template is for a single event, it can use event tokens
              $tokens[] = 'event';
            }
            elseif ($args[1] == 'digest') {
              // Template for multiple events, can use events (note the plural) tokens
              $tokens[] = 'events';
            }
          }
      }
      return $tokens;
  }
}
