<?php

/**
 * @file
 * Contains the SearchApiAlterNodeStatus class.
 */

/**
 * Exclude unpublished nodes from node indexes.
 */
class SearchApiAlterNodeStatus extends SearchApiAbstractAlterCallback {

  /**
   * Check whether this data-alter callback is applicable for a certain index.
   *
   * Returns TRUE only for indexes on nodes.
   *
   * @param SearchApiIndex $index
   *   The index to check for.
   *
   * @return boolean
   *   TRUE if the callback can run on the given index; FALSE otherwise.
   */
  public function supportsIndex(SearchApiIndex $index) {
    if ($this->isMultiEntityIndex($index)) {
      return in_array('node', $index->options['datasource']['types']);
    }
    else {
      return $index->getEntityType() == 'node';
    }
  }

  /**
   * Alter items before indexing.
   *
   * Items which are removed from the array won't be indexed, but will be marked
   * as clean for future indexing.
   *
   * @param array $items
   *   An array of items to be altered, keyed by item IDs.
   */
  public function alterItems(array &$items) {
    foreach ($items as $id => $item) {
      $node = $this->getNode($item);

      // Skip non-node items.
      if (!empty($node) && empty($node->status)) {
        unset($items[$id]);
      }
    }
  }

  /**
   * Retrieves the node related to a search item.
   *
   * @param object $item
   *  The item which is being indexed.
   *
   * @return object|null
   *  Either the node object, or NULL if the current item is not a node.
   */
  protected function getNode($item) {
    if ($this->isMultiEntityIndex()) {
      // For multi-entity indexes, check the item is a node.
      if ($item->item_type == 'node') {
        // The node is stored on a property of the item.
        return $item->node;
      }
      else {
        // Not a node.
        return NULL;
      }
    }
    else {
      // Normal node index: the whole item is the node.
      return $item;
    }
  }

}
