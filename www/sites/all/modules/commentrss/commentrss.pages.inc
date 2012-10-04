<?php

/**
 * @file
 *   Comment RSS page (feed) generation handlers.
 */

/**
 * Publish a feed for all comments on the site.
 */
function commentrss_feed_site() {
  // Filter with all nodes.
  $items = commentrss_format_items('n.nid');
  commentrss_format_feed($items);
}

/**
 * Publish a feed for all comments of a specified node.
 */
function commentrss_feed_node($node) {
  // Filter with this specific node.
  $items = commentrss_format_items(
    'n.nid',
    '',
    'n.nid = %d AND',
    $node->nid,
    FALSE
  );
  // Fill channel information based on node details.
  $channel = array(
    'title' => t(
      '@site - Comments for "@title"',
      array(
        '@site' => check_plain(variable_get('site_name', 'Drupal')),
        '@title' => check_plain($node->title)
      )
    ),
    'description' => t('Comments for "@title"', array('@title' => $node->title)),
    'link' => url('node/' . $node->nid, array('absolute' => TRUE)),
  );
  commentrss_format_feed($items, $channel);
}

/**
 * Publish a feed for all comments with a specified term.
 */
function commentrss_feed_term($tid = NULL) {
  if (isset($tid) && is_numeric($tid) && ($term = taxonomy_get_term($tid))) {
    // Filter with specific term ID.
    $items = commentrss_format_items(
      'DISTINCT(n.nid)',
      'INNER JOIN {term_node} r ON n.nid = r.nid',
      'r.tid = %d AND',
      $term->tid
    );
    // Fill channel information based on term details.
    $channel = array(
      'title' => t(
        '@site - Comments for "@term"',
        array(
          '@site' => check_plain(variable_get('site_name', 'Drupal')),
          '@term' => check_plain($term->name)
        )
      ),
      'description' => t('Comments for the category "@term"', array('@term' => $term->name)),
      'link' => url('taxonomy/term/' . $term->tid, array('absolute' => TRUE)),
    );
    commentrss_format_feed($items, $channel);
    return TRUE;
  }
  return FALSE;
}

// == API functions ============================================================

/**
 * Format and print a complete comment feed.
 *
 * @param $items
 *   XML fragment for the items from commentrss_format_items().
 * @param $channel
 *   An associative array containing title, link, description and other keys,
 *   to be parsed by format_rss_channel() and format_xml_elements().
 *   A list of channel elements can be found at the @link http://cyber.law.harvard.edu/rss/rss.html RSS 2.0 Specification. @endlink
 *   The link should be an absolute URL.
 *
 * @see commentrss_format_items()
 */
function commentrss_format_feed($items, $channel = array()) {
  global $base_url, $language;

  $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');

  $channel_defaults = array(
    'version' => '2.0',
    'title' => t('@site_name - Comments', array('@site_name' => variable_get('site_name', 'Drupal'))),
    'link' => $base_url,
    'description' => t('Comments'),
    'language' => $language->language
  );
  $channel = array_merge($channel_defaults, $channel);

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"" . $channel["version"] . "\" xml:base=\"" . $base_url . "\" " . drupal_attributes($namespaces) . ">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language']);
  $output .= "</rss>\n";

  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
  print $output;
}

/**
 * Format RSS for comments on the nodes selected by the given SQL selector.
 *
 * All parameters are expected to be properly sanitized, and used directly
 * in the SQL query, except $param which is value for a placeholder in $where.
 *
 * @param $nidselector
 *   SQL fragment to select node IDs.
 * @param $joins
 *   SQL fragment to inject in the JOINs segment.
 * @param $where
 *   Filtering SQL fragment.
 * @param $param
 *   Single parameter to use with an optional placeholder in the $where clause.
 * @param $include_node_title
 *   Include node title above comment body.
 * @return
 *   Formatted XML of RSS items generated.
 */
function commentrss_format_items($nidselector = 'n.nid', $joins = '', $where = '', $params = array(), $include_node_title = TRUE) {
  global $base_url;

  $items = '';

  // Build SQL query from the passed elements.
  $sql = 'SELECT ' . $nidselector . ', c.cid, c.pid, c.subject, c.comment, c.timestamp, c.uid, c.name, c.format, u.name AS username, n.title FROM {node} n ' . $joins . ' INNER JOIN {comments} c ON c.nid = n.nid INNER JOIN {users} u ON c.uid = u.uid WHERE ' . $where . ' n.status = 1 AND c.status = %d ORDER BY c.timestamp DESC';

  // Add passed query parameter; plus we are looking for published comments.
  if (!is_array($params)) {
    $params = array($params);
  }
  $params[] = COMMENT_PUBLISHED;

  // Use query rewriting for node level access permission support.
  $comments = db_query_range(db_rewrite_sql($sql), $params, 0, variable_get('feed_default_items', 10));
  while ($comment = db_fetch_object($comments)) {
    if (!empty($comment->username)) {
      // Username takes precedence. The comment's name attribute should only
      // be filled in anyway if it was an anonymous comment with submitter
      // details enabled.
      $comment->name = $comment->username;
    }
    if (empty($comment->name)) {
      // Fall back on anonymous name if no other name was specified.
      $comment->name = variable_get('anonymous', 'Anonymous');
    }

    // Build up item content with node title if required.
    $content = '';
    if ($include_node_title) {
      $content = '<p>' . t('In reply to <a href="@url">@title</a>', array('@url' => url('node/' . $comment->nid, array('absolute' => TRUE)), '@title' => $comment->title)) . ":</p>\n\n";
    }

    // Format RSS item with input format used on comment. The format
    // was validated previously and the visitor might not have access to that,
    // so skip the format validation in check_markup() itself.
    $content .= check_markup($comment->comment, $comment->format, FALSE);

    $link = url('node/' . $comment->nid, array('fragment' => 'comment-' . $comment->cid, 'absolute' => TRUE));
    $extra = array(
      array(
        'key' => 'pubDate',
        'value' => gmdate(DATE_RSS, $comment->timestamp),
      ),
      array(
        'key' => 'dc:creator',
        'value' => $comment->name,
      ),
      array(
        'key' => 'guid',
        'value' => 'comment ' . $comment->cid . ' at ' . $base_url,
        'attributes' => array('isPermaLink' => 'false'),
      )
    );

    $items .= format_rss_item($comment->subject, $link, $content, $extra);
  }

  return $items;
}
