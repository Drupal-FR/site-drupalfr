<?php

namespace Drupal\drupalfr_planet\Plugin\ExtraField\Display;

use Drupal\Core\Cache\CacheableMetadata;
use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\extra_field\Plugin\ExtraFieldDisplayBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Display the planet feed image URL.
 *
 * @ExtraFieldDisplay(
 *   id = "drupalfr_planet_feed_image_url",
 *   label = @Translation("Feed image URL"),
 *   bundles = {
 *     "node.planet",
 *   }
 * )
 */
class PlanetFeedImageUrl extends ExtraFieldDisplayBase implements ContainerFactoryPluginInterface {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, EntityTypeManagerInterface $entity_type_manager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entity_type_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
          $configuration,
          $plugin_id,
          $plugin_definition,
          $container->get('entity_type.manager')
      );
  }

  /**
   * {@inheritdoc}
   */
  public function view(ContentEntityInterface $entity) {
    /** @var \Drupal\node\NodeInterface $entity */

    $elements = [];
    $cache = new CacheableMetadata();

    if ($entity->hasField('feeds_item')) {
      $feed = $entity->get('feeds_item')->getValue();
      if (!empty($feed)) {
        /** @var \Drupal\feeds\FeedInterface $feed */
        $feed = $this->entityTypeManager->getStorage('feeds_feed')->load($feed[0]['target_id']);
        $cache->addCacheableDependency($feed);

        if ($feed->hasField('field_feed_image') && !$feed->get('field_feed_image')->isEmpty()) {
          $cache->addCacheableDependency($feed->get('field_feed_image')->entity);

          $uri = $feed->get('field_feed_image')->entity->uri->value;
          $elements = [
            '#markup' => file_url_transform_relative(file_create_url($uri)),
          ];
        }
      }
    }

    $cache->applyTo($elements);
    return $elements;
  }

}
