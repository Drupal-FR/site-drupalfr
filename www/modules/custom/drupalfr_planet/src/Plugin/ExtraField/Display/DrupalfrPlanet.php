<?php

namespace Drupal\drupalfr_planet\Plugin\ExtraField\Display;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\extra_field\Plugin\ExtraFieldDisplayBase;
use Drupal\feeds\Entity\Feed;

/**
 * Example Extra field Display.
 *
 * @ExtraFieldDisplay(
 *   id = "drupalfr_planet",
 *   label = @Translation("DrupalFr_Planet"),
 *   bundles = {
 *     "node.planet",
 *   }
 * )
 */
class DrupalfrPlanet extends ExtraFieldDisplayBase {

  /**
   * {@inheritdoc}
   */
  public function view(ContentEntityInterface $entity) {
    if ($entity->hasField('feeds_item')) {
      /** @var \Drupal\feeds\FeedInterface $feed */
      $feed = $entity->get('feeds_item')->getValue();
      if (!empty($feed)) {
        $feed = Feed::load($feed[0]['target_id']);
        if ($feed->hasField('field_feed_image') && !$feed->get('field_feed_image')->isEmpty()) {
          if ($this->viewMode == "teaser" || $this->viewMode == "search_result") {
            $elements['feed_image'] = $feed->get('field_feed_image')
              ->view('default');
          }
          else {
            $url = $feed->get('field_feed_image')->entity->uri->value;
            $elements['feed_image_url'] = file_create_url($url);
          }
        }
      }
    }
    return $elements;
  }

}
