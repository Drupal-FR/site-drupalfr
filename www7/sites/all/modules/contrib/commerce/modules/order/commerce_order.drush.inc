<?php

/**
 * @file
 * Drush commands for Commerce Order.
 */

/**
 * Implements hook_drush_command().
 */
function commerce_order_drush_command() {
  $items = array();

  $items['commerce-order-update-placed-timestamp'] = array(
    'description' => 'Set the order placed timestamp to match the created timestamp.',
    'examples' => array(
      'drush commerce-order-update-placed-timestamp' => dt('Update the placed timestamp, Defaults to 1000 orders per batch run.'),
      'drush coupt 500' => dt('Update the placed timestamp, 500 orders per batch run.'),
    ),
    'arguments' => array(
      'limit' => dt("The number of orders to update per batch run. Defaults to 1000."),
    ),
    'aliases' => array('coupt'),
  );

  return $items;
}

/**
 * Update the placed timestamp for existing placed orders.

 * @param integer $limit
 *   Number of orders to update per batch run.
 */
function drush_commerce_order_update_placed_timestamp($limit = 1000) {
  $pending_statuses = commerce_order_statuses(array('state' => 'pending'));
  $completed_statuses = commerce_order_statuses(array('state' => 'completed'));
  $statuses = array_merge(array_keys($pending_statuses), array_keys($completed_statuses));
  $orders_count = db_select('commerce_order')
    ->condition('placed', 0)
    ->condition('status', $statuses, 'IN')
    ->countQuery()
    ->execute()
    ->fetchField();

  if ($orders_count <= 0) {
    drush_log(dt('There are currently no pending or completed orders that should have their placed timestamp updated.'), 'error');
  }
  drush_log(dt('@count orders are about to be processed.', array('@count' => $orders_count)));

  $batch = array(
    'title' => dt('Updating orders'),
    'progress_message' => dt('Completed about @percentage% of the order update operation.'),
    'operations' => array(
      array('_commerce_order_update_placed_timestamp_batch_callback', array($statuses, $limit, $orders_count)),
    ),
    'finished' => '_commerce_order_update_placed_timestamp_finished',
  );
  batch_set($batch);
  drush_backend_batch_process();
}

/**
 * Batch API callback: Update the placed timestamp.
 *
 * @param array $statuses
 *   An array containing pending/completed order statuses.
 * @param integer $limit
 *   Number of orders to update per batch run.
 * @param integer $orders_count
 *   Total number orders to update.
 * @param $context
 *   An array (or object implementing ArrayAccess) containing the batch context.
 */
function _commerce_order_update_placed_timestamp_batch_callback($statuses, $limit, $orders_count, &$context) {
  // Persistent data among batch runs.
  if (!isset($context['sandbox']['limit'])) {
    $context['sandbox']['limit'] = ceil($orders_count / $limit);
    $context['sandbox']['progress'] = 0;
  }
  // Persistent data for results.
  if (!isset($context['results']['updated_count'])) {
    $context['results']['updated_count'] = 0;
  }
  $rows_affected = db_query('UPDATE {commerce_order} SET placed = created WHERE placed = 0 AND status IN (:statuses) LIMIT ' . $limit, array(':statuses' => $statuses), array('return' => Database::RETURN_AFFECTED));
  $context['results']['updated_count'] += $rows_affected;
  $context['sandbox']['progress']++;
  $context['message'] = dt('@progress orders processed out of @count.', array('@progress' => $context['results']['updated_count'], '@count' => $orders_count));
  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['limit'];
}

/**
 * Batch API finishing callback for the placed timestamp update.
 *
 * @param boolean $success
 *   Whether the batch finished successfully.
 * @param array $results
 *   Detailed information about the result.
 */
function _commerce_order_update_placed_timestamp_finished($success, $results) {
  if ($success) {
    drush_log(dt('The placed timestamp was successfully updated for @count orders.', array('@count' => $results['updated_count'])), 'success');
  }
  else {
    drush_log(dt('There was an error while updating the placed timestamp for orders.'), 'error');
  }
}
