<?php

/**
 * @file
 * Describe hooks provided by the publishcontent module.
 */

/**
 * Allow other modules the ability to modify access to the publish controls.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform publish action on a node.
 *
 * @param node $node
 *   A node object being checked
 * @param user $account
 *   The user wanting to publish the node.
 *
 * @return bool|NULL
 *   TRUE - if the user should access return TRUE
 *   FALSE - if the user definetley should not access return FALSE
 *   NULL - This module wan't change the outcome. It is typically
 *   better to return NULL than FALSE. If no module returns TRUE
 *   then the user will be denied access. If one module returns
 *   FALSE then the user will denied even if another module returns
 *   TRUE.
 */
function hook_publishcontent_publish_access($node, $account) {
  $access = !$node->status &&
    (user_access('administer nodes')
    || user_access('publish any content')
    || (user_access('publish own content') && $account->uid == $node->uid)
    || (user_access('publish editable content') && node_access('update', $node))
    || (user_access('publish own ' . check_plain($node->type) . ' content', $account) && $account->uid == $node->uid)
    || user_access('publish any ' . check_plain($node->type) . ' content')
    || (user_access('publish editable ' . check_plain($node->type) . ' content') && node_access('update', $node))
  );

  if ($access) {
    return TRUE;
  }
}

/**
 * Allow other modules the ability to modify access to the unpublish controls.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform unpublish action on a node.
 *
 * @param node $node
 *   A node object being checked
 * @param user $account
 *   The user wanting to unpublish the node.
 *
 * @return bool|NULL
 *   TRUE - if the user should access return TRUE
 *   FALSE - if the user definetley should not access return FALSE
 *   NULL - This module wan't change the outcome. It is typically
 *   better to return NULL than FALSE. If no module returns TRUE
 *   then the user will be denied access. If one module returns
 *   FALSE then the user will denied even if another module returns
 *   TRUE.
 */
function hook_publishcontent_unpublish_access($node, $account) {
  $access = $node->status &&
    (user_access('administer nodes')
    || user_access('unpublish any content')
    || (user_access('unpublish own content') && $user->uid == $node->uid)
    || (user_access('unpublish editable content') && node_access('update', $node))
    || (user_access('unpublish own ' . check_plain($node->type) . ' content', $user) && $user->uid == $node->uid)
    || user_access('unpublish any ' . check_plain($node->type) . ' content')
    || (user_access('unpublish editable ' . check_plain($node->type) . ' content') && node_access('update', $node))
  );

  if ($access) {
    return TRUE;
  }
}
