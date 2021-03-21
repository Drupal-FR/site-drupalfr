<?php

/**
 * @file
 * Include to delete all nodes of a type.
 */

/**
 * Delete nodes multiple.
 */
function delete_nodes_multiple($nids) {
  $storage_controller = \Drupal::service('entity_type.manager')->getStorage('node');

  $nodes = $storage_controller->loadMultiple($nids);
  $storage_controller->delete($nodes);
}

/**
 * Message at end of batch.
 */
function finish_batch($nids): void {
  \Drupal::service('messenger')->addMessage(sprintf('%s nodes deleted.', count($nids)));
}
