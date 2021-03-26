<?php


function deleteNodesMultiple($nids)
{
    $storage_controller = \Drupal::service('entity_type.manager')->getStorage('node');

    $nodes = $storage_controller->loadMultiple($nids);
    $storage_controller->delete($nodes);
}

function deleteNodeFromNid($nid)
{
    $storage_controller = \Drupal::service('entity_type.manager')->getStorage('node');
    $node = $storage_controller->load($nid);
    $storage_controller->delete([$node]);
}

function finishBatch($nids): void
{
    \Drupal::service('messenger')->addMessage(sprintf('%s nodes deleted.', count($nids)));
}
