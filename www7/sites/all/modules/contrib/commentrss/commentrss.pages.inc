<?php

/**
 * @file
 * Page callbacks for the commentrss module.
 */

/**
 * Generate a feed for the last 10 comments of a node.
 *
 * @param $node
 *   A node object.
 *
 * @see commentrss_feed()
 */
function commentrss_node_feed($node) {
  // Find the latest 10 visible comments on this node.
  // We do not have to check n.status = 1 specifically since we have already
  // performed a node_access('view', $node) check on this node.
  $query = db_select('comment', 'c');
  $query->fields('c', array('cid', 'created'));
  $query->condition('c.nid', $node->nid);
  $query->condition('c.status', COMMENT_PUBLISHED);
  $query->addTag('comment_access');
  $query->orderBy('c.created', 'DESC');
  $query->range(0, variable_get('feed_default_items', 10));
  $cids = $query->execute()->fetchCol();

  // Fill channel information based on node details.
  $channel['title'] = t('@site - Comments for "@title"', array(
    '@site' => variable_get('site_name', 'Drupal'),
    '@title' => $node->title,
  ));
  $channel['description'] = t('Comments for "@title"', array('@title' => $node->title));
  $node_uri = entity_uri('node', $node);
  $channel['link'] = url($node_uri['path'], $node_uri['options'] + array('absolute' => TRUE));

  return commentrss_feed($cids, $channel);
}
