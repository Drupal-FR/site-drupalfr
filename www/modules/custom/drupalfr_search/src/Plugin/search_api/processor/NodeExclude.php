<?php

namespace Drupal\drupalfr_search\Plugin\search_api\processor;

use Drupal\node\NodeInterface;
use Drupal\search_api\IndexInterface;
use Drupal\search_api\Processor\ProcessorPluginBase;

/**
 * Exclude node if body field is empty.
 *
 * @package Drupal\drupalfr_search\Plugin\Plugin\search_api\processor
 *
 * @SearchApiProcessor(
 *   id = "node_exclude",
 *   label = @Translation("Node exclude if body is empty"),
 *   description = @Translation("Exclude specific nodes if body is empty"),
 *   stages = {
 *     "alter_items" = 0
 *   }
 * )
 */
class NodeExclude extends ProcessorPluginBase
{

  /**
   * {@inheritdoc}
   */
    public static function supportsIndex(IndexInterface $index)
    {
        foreach ($index->getDatasources() as $datasource) {
            if ($datasource->getEntityTypeId() === 'node') {
                return true;
            }
        }

        return false;
    }

  /**
   * {@inheritdoc}
   */
    public function alterIndexedItems(array &$items)
    {
      /** @var \Drupal\search_api\Item\ItemInterface $item */
        foreach ($items as $item_id => $item) {
            $object = $item->getOriginalObject()->getValue();
            if ($object instanceof NodeInterface) {
                $body = $object->get('body')->isEmpty();
                if ($body) {
                    unset($items[$item_id]);
                }
            }
        }
    }
}
