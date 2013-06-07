<?php
/**
 * @file
 * Notifications node API for use by plug-in modules providing user related features
 *
 * So far, this is used by:
 * - notifications_content
 * - notifications_feed
 */

/**
 * Wrapper for author autocomplete callback.
 *
 * @param $uid
 *   uid of the user for which to return the name.
 * @param $subs_type
 *   Optional type of subscription for which to find allowed content types. Defaults to nodetype, can be any subscription type with event-type=node for which notifications_content handles content type settings.
 */
function notifications_user_name_callback($uid, $subs_type = '') {
  return messaging_user_format_name($uid);
}

function notifications_user_name2uid($name, $field = NULL) {
  if ($account = user_load(array('name' => $name))) {
    return $account->uid;
  }
  elseif ($field) {
    form_set_error($field, t('User name not found.'));
  }
}

/**
 * Check user access to user
 *
 * @param $user
 *   Target user account to check access to
 * @param $account
 *   User account who wants to access
 */
function notifications_user_user_access($user, $account) {
  return $user && $user->uid &&
    (
      // Always let users view their own profile.
      ($user->uid == $account->uid) ||
      // Administrators can view all accounts.
      user_access('administer users', $account) ||
      // The user is not blocked and logged in at least once.
      ($user->access && $user->status && user_access('access user profiles', $account))
    );
}
