<?php

/**
 * Generate an overview table of older revisions of a node and provide
 * an input form to select two revisions for a comparison.
 */
function diff_diffs_overview(&$node) {
  $output = '';

  drupal_set_title(t('Revisions for %title', array('%title' => $node->title)));
  $output .= drupal_get_form('diff_node_revisions', $node);

  return $output;
}

/**
 * Input form to select two revisions.
 *
 * @param $node
 *   Node whose revisions are displayed for selection.
 */
function diff_node_revisions($form_state, &$node) {
  global $form_values;
  $form = array();

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $revision_list = node_revision_list($node);

  if (count($revision_list) > REVISION_LIST_SIZE) {
    // If the list of revisions is longer than the number shown on one page split the array.
    $page = isset($_GET['page']) ? $_GET['page'] : '0';
    $revision_chunks = array_chunk(node_revision_list($node), REVISION_LIST_SIZE);
    $revisions = $revision_chunks[$page];
    // Set up global pager variables as would 'pager_query' do.
    // These variables are then used in the theme('pager') call later.
    global $pager_page_array, $pager_total, $pager_total_items;
    $pager_total_items[0] = count($revision_list);
    $pager_total[0] = ceil(count($revision_list) / REVISION_LIST_SIZE);
    $pager_page_array[0] = max(0, min($page, ((int)$pager_total[0]) - 1));
  }
  else {
    $revisions = $revision_list;
  }

  $revert_permission = FALSE;
  if ((user_access('revert revisions') || user_access('administer nodes')) && node_access('update', $node)) {
    $revert_permission = TRUE;
  }
  $delete_permission = FALSE;
  if (user_access('administer nodes')) {
    $delete_permission = TRUE;
  }

  foreach ($revisions as $revision) {
    $operations = array();
    $revision_ids[$revision->vid] = '';

    if ($revision->current_vid > 0) {
      $form['info'][$revision->vid] = array(
        '#value' => t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'small'), "node/$node->nid"),
          '!username' => theme('username', $revision)))
        . (($revision->log != '') ? '<p class="revision-log">'. filter_xss($revision->log) .'</p>' : ''),
      );
    }
    else {
      $form['info'][$revision->vid] = array(
        '#value' => t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'small'), "node/$node->nid/revisions/$revision->vid/view"),
          '!username' => theme('username', $revision)))
        . (($revision->log != '') ? '<p class="revision-log">'. filter_xss($revision->log) .'</p>' : '')
      );
      if ($revert_permission) {
        $operations[] = array('#value' => l(t('revert'), "node/$node->nid/revisions/$revision->vid/revert"));
      }
      if ($delete_permission) {
        $operations[] = array('#value' => l(t('delete'), "node/$node->nid/revisions/$revision->vid/delete"));
      }
      // Set a dummy, even if the user has no permission for the other
      // operations, so that we can check if the operations array is
      // empty to know if the row denotes the current revision.
      $operations[] = array();
    }
    $form['operations'][$revision->vid] = $operations;

  }
  $new_vid = key($revision_ids);
  next($revision_ids);
  $old_vid = key($revision_ids);
  $form['diff']['old'] = array(
    '#type' => 'radios',
    '#options' => $revision_ids,
    '#default_value' => $old_vid
  );
  $form['diff']['new'] = array(
    '#type' => 'radios',
    '#options' => $revision_ids,
    '#default_value' => $new_vid
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Show diff'));

  if (count($revision_list) > REVISION_LIST_SIZE) {
    $form['#suffix'] = theme('pager', NULL, REVISION_LIST_SIZE, 0);
  }

  return $form;
}

/**
 * Submit code for input form to select two revisions.
 */
function diff_node_revisions_submit($form, &$form_state) {
  // the ids are ordered so the old revision is always on the left
  $old_vid = min($form_state['values']['old'], $form_state['values']['new']);
  $new_vid = max($form_state['values']['old'], $form_state['values']['new']);
  $form_state['redirect'] =  'node/'. $form_state['values']['nid'] .'/revisions/view/'. $old_vid .'/'. $new_vid;
}

/**
 * Validation for input form to select two revisions.
 */
function diff_node_revisions_validate($form, &$form_state) {
  $old_vid = $form_state['values']['old'];
  $new_vid = $form_state['values']['new'];
  if ($old_vid==$new_vid || !$old_vid || !$new_vid) {
    form_set_error('diff', t('Select different revisions to compare.'));
  }
}

/**
 * Create output string for a comparison of 'node' between
 * versions 'old_vid' and 'new_vid'.
 *
 * @param $node
 *   Node on which to perform comparison
 * @param $old_vid
 *   Version ID of the old revision.
 * @param $new_vid
 *   Version ID of the new revision.
 */
function diff_diffs_show(&$node, $old_vid, $new_vid) {

  // Set same title as on the 'Revisions' tab for consistency
  drupal_set_title(t('Revisions for %title', array('%title' => $node->title)));

  $node_revisions = node_revision_list($node);

  $old_node = node_load($node->nid, $old_vid);
  $new_node = node_load($node->nid, $new_vid);

  // Generate table header (date, username, logmessage).
  $old_header = t('!date by !username', array(
    '!date' => l(format_date($old_node->revision_timestamp), "node/$node->nid/revisions/$old_node->vid/view"),
    '!username' => theme('username', $node_revisions[$old_vid]),
  ));
  $new_header = t('!date by !username', array(
    '!date' => l(format_date($new_node->revision_timestamp), "node/$node->nid/revisions/$new_node->vid/view"),
    '!username' => theme('username', $node_revisions[$new_vid]),
  ));

  $old_log = $old_node->log != '' ? '<p class="revision-log">'. filter_xss($old_node->log) .'</p>' : '';
  $new_log = $new_node->log != '' ? '<p class="revision-log">'. filter_xss($new_node->log) .'</p>' : '';

  // Generate previous diff/next diff links.
  $next_vid = _diff_get_next_vid($node_revisions, $new_vid);
  if ($next_vid) {
    $next_link = l(t('next diff >'), 'node/'. $node->nid .'/revisions/view/'. $new_vid .'/'. $next_vid);
  }
  else {
    $next_link = '';
  }
  $prev_vid = _diff_get_previous_vid($node_revisions, $old_vid);
  if ($prev_vid) {
    $prev_link = l(t('< previous diff'), 'node/'. $node->nid .'/revisions/view/'. $prev_vid .'/'. $old_vid);
  }
  else {
    $prev_link = '';
  }

  $cols = _diff_default_cols();
  $header = _diff_default_header($old_header, $new_header);
  $rows = array();
  if ($old_log || $new_log) {
    $rows[] = array(
      array(
        'data' => $old_log,
        'colspan' => 2
      ),
      array(
        'data' => $new_log,
        'colspan' => 2
      )
    );
  }
  $rows[] = array(
    array(
      'data' => $prev_link,
      'class' => 'diff-prevlink',
      'colspan' => 2
    ),
    array(
      'data' => $next_link,
      'class' => 'diff-nextlink',
      'colspan' => 2
    )
  );
  $rows = array_merge($rows, _diff_body_rows($old_node, $new_node));
  $output = theme('diff_table', $header, $rows, array('class' => 'diff'), NULL, $cols);

  if ($node->vid == $new_vid) {
    $output .= '<div class="diff-section-title">'. t('Current revision:') .'</div>';
  }
  else {
    $output .= '<div class="diff-section-title">'. t('Revision of !new_date:', array('!new_date' => format_date($new_node->revision_timestamp))) .'</div>';
  }
  // Don't include node links (final argument) when viewing the diff.
  $output .= node_view($new_node, FALSE, FALSE, FALSE);
  return $output;
}

/**
 * Creates an array of rows which represent a diff between $old_node and $new_node.
 * The rows can be used via theme('diff_table') to be displayed.
 *
 * @param $old_node
 *   Node for comparison which will be displayed on the left side.
 * @param $new_node
 *   Node for comparison which will be displayed on the right side.
 */
function _diff_body_rows(&$old_node, &$new_node) {
  drupal_add_css(drupal_get_path('module', 'diff') .'/diff.css', 'module', 'all', FALSE);
  module_load_include('php', 'diff', 'DiffEngine');
  module_load_include('inc', 'diff', 'node');
  if (module_exists('taxonomy')) {
    module_load_include('inc', 'diff', 'taxonomy');
  }
  if (module_exists('upload')) {
    module_load_include('inc', 'diff', 'upload');
  }

  $rows = array();
  $any_visible_change = FALSE;
  $node_diffs = module_invoke_all('diff', $old_node, $new_node);

  // We start off assuming all form elements are in the correct order.
  $node_diffs['#sorted'] = TRUE;

  // Recurse through all child elements.
  $count = 0;
  foreach (element_children($node_diffs) as $key) {
    // Assign a decimal placeholder weight to preserve original array order.
    if (!isset($node_diffs[$key]['#weight'])) {
      $node_diffs[$key]['#weight'] = $count/1000;
    }
    else {
      // If one of the child elements has a weight then we will need to sort
      // later.
      unset($node_diffs['#sorted']);
    }
    $count++;
  }

  // One of the children has a #weight.
  if (!isset($node_diffs['#sorted'])) {
    uasort($node_diffs, "element_sort");
  }

  foreach ($node_diffs as $node_diff) {
    $diff = new Diff($node_diff['#old'], $node_diff['#new']);
    $formatter = new DrupalDiffFormatter();
    if (isset($node_diff['#format'])) {
      $formatter->show_header = $node_diff['#format']['show_header'];
    }
    $diff_rows = $formatter->format($diff);
    if (count($diff_rows)) {
      $rows[] = array(
        array(
          'data' => t('Changes to %name', array('%name' => $node_diff['#name'])),
          'class' => 'diff-section-title',
          'colspan' => 4
        )
      );
      $rows = array_merge($rows, $diff_rows);
      $any_visible_change = TRUE;
    }
  }
  if (!$any_visible_change) {
    $rows[] = array(
      array(
        'data' => t('No visible changes'),
        'class' => 'diff-section-title',
        'colspan' => 4
      ),
    );
    // Needed to keep safari happy
    $rows[] = array(
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
    );
  }
  return $rows;
}

/**
 * Get the entry in the revisions list after $vid.
 * Returns FALSE if $vid is the last entry.
 *
 * @param $node_revisions
 *   Array of node revision IDs in descending order.
 * @param $vid
 *   Version ID to look for.
 */
function _diff_get_next_vid(&$node_revisions, $vid) {
  $previous = NULL;
  foreach ($node_revisions as $revision) {
    if ($revision->vid == $vid) {
      return ($previous ? $previous->vid : FALSE);
    }
    $previous = $revision;
  }
  return FALSE;
}

/**
 * Get the entry in the revision list before $vid.
 * Returns FALSE if $vid is the first entry.
 *
 * @param $node_revisions
 *   Array of node revision IDs in descending order.
 * @param $vid
 *   Version ID to look for.
 */
function _diff_get_previous_vid(&$node_revisions, $vid) {
  $previous = NULL;
  foreach ($node_revisions as $revision) {
    if ($previous && $previous->vid == $vid) {
      return $revision->vid;
    }
    $previous = $revision;
  }
  return FALSE;
}

/**
 * Helper function to create default 'cols' array for diff table.
 */
function _diff_default_cols() {
  return array(
    array(
      array(
        'class' => 'diff-marker',
      ),
      array(
        'class' => 'diff-content',
      ),
      array(
        'class' => 'diff-marker',
      ),
      array(
        'class' => 'diff-content',
      ),
    ),
  );
}

/**
 * Helper function to create default 'header' array for diff table.
 */
function _diff_default_header($old_header = '', $new_header = '') {
  return array(
    array(
      'data' => $old_header,
      'colspan' => 2
    ),
    array(
      'data' => $new_header,
      'colspan' => 2
    )
  );
}

/**
 * Generate a rendered inline diff of the node body.
 */
function diff_inline_diff($node, $target_vid) {
  module_load_include('php', 'diff', 'DiffEngine');
  $new = drupal_clone($node);

  // We could use a node_load() / node_prepare() combo here but for now
  // we would rather save queries...
  $old = db_fetch_object(db_query("SELECT body, format FROM {node_revisions} WHERE nid = %d AND vid = %d", $node->nid, $target_vid));
  $old->body = check_markup($old->body, $old->format, FALSE);

  $new = preg_split('/(<[^>]+?>| )/', $new->body, -1, PREG_SPLIT_DELIM_CAPTURE);
  $old = preg_split('/(<[^>]+?>| )/', $old->body, -1, PREG_SPLIT_DELIM_CAPTURE);

  $diff = new Diff($old, $new);
  $diff->edits = _diff_inline_process_edits($diff->edits);

  // Assemble highlighted output
  $output = '';
  foreach ($diff->edits as $chunk) {
    switch ($chunk->type) {
      case 'copy':
        $output .= implode("", $chunk->closing);
        break;
      case 'delete':
        foreach ($chunk->orig as $i => $piece) {
          if (strpos($piece, '<') === 0 && substr($piece, strlen($piece) - 1) === '>') {
            $output .= $piece;
          }
          else {
            $output .= theme('diff_inline_chunk', $piece, $chunk->type);
          }
        }
        break;
      default:
        $chunk->closing = _diff_inline_process_chunk($chunk->closing);

        foreach ($chunk->closing as $i => $piece) {
          if ($piece === ' ' || (strpos($piece, '<') === 0 && substr($piece, strlen($piece) - 1) === '>')) {
            $output .= $piece;
          }
          else {
            $output .= theme('diff_inline_chunk', $piece, $chunk->type);
          }
        }
        break;
    }
  }
  return $output;
}

/**
 * Merge chunk segments between tag delimiters.
 */
function _diff_inline_process_chunk($chunk) {
  $processed = array();
  $j = 0;
  foreach ($chunk as $i => $piece) {
    $next = isset($chunk[$i+1]) ? $chunk[$i+1] : NULL;
    if (strpos($piece, '<') === 0 && substr($piece, strlen($piece) - 1) === '>') {
      $processed[$j] = $piece;
      $j++;
    }
    else if (isset($next) && strpos($next, '<') === 0 && substr($next, strlen($next) - 1) === '>') {
      $processed[$j] .= $piece;
      $j++;
    }
    else {
      $processed[$j] .= $piece;
    }
  }
  return $processed;
}

/**
 * Merge copy and equivalent edits into intelligible chunks.
 */
function _diff_inline_process_edits($edits) {
  $processed = array();
  $current = array_shift($edits);

  // Make two passes -- first merge space delimiter copies back into their originals.
  while ($chunk = array_shift($edits)) {
    if ($chunk->type == 'copy' && $chunk->orig === array(' ')) {
      $current->orig = array_merge((array) $current->orig, (array) $chunk->orig);
      $current->closing = array_merge((array) $current->closing, (array) $chunk->closing);
    }
    else {
      $processed[] = $current;
      $current = $chunk;
    }
  }
  $processed[] = $current;

  // Initial setup
  $edits = $processed;
  $processed = array();
  $current = array_shift($edits);

  // Second, merge equivalent chunks into each other.
  while ($chunk = array_shift($edits)) {
    if ($current->type == $chunk->type) {
      $current->orig = array_merge((array) $current->orig, (array) $chunk->orig);
      $current->closing = array_merge((array) $current->closing, (array) $chunk->closing);
    }
    else {
      $processed[] = $current;
      $current = $chunk;
    }
  }
  $processed[] = $current;

  return $processed;
}
