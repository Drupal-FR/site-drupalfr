<?php

namespace Drupal\drupalfr_planet\Plugin\ExtraField\Display;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\extra_field\Plugin\ExtraFieldDisplayBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Display the planet feed image.
 *
 * @ExtraFieldDisplay(
 *   id = "drupalfr_planet_feed_image",
 *   label = @Translation("Feed image"),
 *   bundles = {
 *     "node.planet",
 *   }
 * )
 */
class PlanetFeedImage extends ExtraFieldDisplayBase implements ContainerFactoryPluginInterface {

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

    if ($entity->hasField('feeds_item')) {
      $feed = $entity->get('feeds_item')->getValue();
      if (!empty($feed)) {
        /** @var \Drupal\feeds\FeedInterface $feed */
        $feed = $this->entityTypeManager->getStorage('feeds_feed')->load($feed[0]['target_id']);
        if ($feed->hasField('field_feed_image') && !$feed->get('field_feed_image')->isEmpty()) {
          $elements = $feed->get('field_feed_image')->view('default');
        }
      }
    }

    return $elements;
  }

}
