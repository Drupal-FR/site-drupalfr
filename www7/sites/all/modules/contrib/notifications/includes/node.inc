<?php
/**
 * @file
 * Notifications node API for use by plug-in modules providing node related features
 *
 * So far, this is used by:
 * - notifications_content
 * - notifications_feed
 */

/**
 * Mapping from node nid to title
 */
function notifications_node_nid2title($nid, $html = FALSE) {
  if ($node = node_load($nid)) {
    return $html ? l($node->title, "node/$nid") : check_plain($node->title);
  }
  else {
    return t('Not found');
  }
}

/**
 * Reverse mapping from node title to nid
 *
 * We also handle autocomplete values (title [nid:x]) and validate the form
 */
function notifications_node_title2nid($name, $field = NULL, $types = array()) {
  if (!empty($name)) {
    preg_match('/^(?:\s*|(.*) )?\[\s*nid\s*:\s*(\d+)\s*\]$/', $name, $matches);
    if (!empty($matches)) {
      // Explicit [nid:n].
      list(, $title, $nid) = $matches;
      if (!empty($title) && ($node = node_load($nid)) && $title != $node->title) {
        if ($field) {
          form_set_error($field, t('Node title mismatch. Please check your selection.'));
        }
        $nid = NULL;
      }
    }
    else {
      // No explicit nid.
      $reference = _notifications_node_references($name, 'equals', $types, 1);
      if (!empty($reference)) {
        $nid = key($reference);
      }
      elseif ($field) {
        form_set_error($field, t('Found no valid post with that title: %title', array('%title' => $name)));
      }
    }
  }
  return !empty($nid) ? $nid : NULL;
}

/**
 * Generates 'title [nid:$nid]' for the autocomplete field
 */
function notifications_node_nid2autocomplete($nid, $subscription = NULL) {
  if ($node = node_load($nid)) {
    return check_plain($node->title) . ' [nid:' . $nid . ']';
  }
  else {
    return t('Not found');
  }
}

/**
 * Check user access to node
 */
function notifications_node_user_access($node, $account) {
  return node_access('view', $node, $account);
}

/**
 * Menu callback; Retrieve a pipe delimited string of autocomplete suggestions for existing users
 */
function notifications_node_autocomplete_title($string = '') {
  $matches = array();

  foreach (_notifications_node_references($string) as $id => $row) {
    // Add a class wrapper for a few required CSS overrides.
    $matches[$row['title'] . " [nid:$id]"] = '<div class="reference-autocomplete">' . $row['rendered'] . '</div>';
  }
  drupal_json_output($matches);
}

/**
 * Menu callback; Retrieve a pipe delimited string of autocomplete suggestions for existing nodes
 *
 * @param $node_types
 *   Comma separated node types to query
 */
function notifications_node_autocomplete_type($node_types, $string = '') {
  $matches = array();
  $types = split(',', $node_types);
  foreach (_notifications_node_references($string, 'contains', $types) as $id => $row) {
    // Add a class wrapper for a few required CSS overrides.
    $matches[$row['title'] . " [nid:$id]"] = '<div class="reference-autocomplete">' . $row['rendered'] . '</div>';
  }
  drupal_json_output($matches);
}

/**
 * Find node title matches.
 *
 * Some code from node_reference.module
 */
function _notifications_node_references($string, $match = 'contains', $types = array(), $limit = 10) {
  $query = db_select('node', 'n');
  $node_nid_alias = $query->addField('n', 'nid');
  $node_title_alias = $query->addField('n', 'title', 'node_title');
  $node_type_alias = $query->addField('n', 'type',  'node_type');
  $query->addTag('node_access');

  if ($types) {
    $query->condition('n.type', $types, 'IN');
  }

  switch ($match) {
    default: // no match type or incorrect match type: use "="
    case 'contains':
      $query->condition('n.title', '%' . $string . '%', 'LIKE');
      break;

    case 'starts_with':
      $query->condition('n.title', $string . '%', 'LIKE');
      break;

    case 'equals':
      $query->condition('n.title', $string);
      break;
  }

  $query
    ->range(0, $limit)
    ->orderBy($node_title_alias)
    ->orderBy($node_type_alias);

  $result = $query->execute()->fetchAll();
  $references = array();
  foreach ($result as $node) {
    $references[$node->nid] = array(
      'title'    => $node->node_title,
      'rendered' => check_plain($node->node_title),
    );
  }
  return $references;
}

