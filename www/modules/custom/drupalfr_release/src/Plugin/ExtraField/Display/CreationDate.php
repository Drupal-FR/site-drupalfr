<?php

namespace Drupal\drupalfr_release\Plugin\ExtraField\Display;

use Drupal\Core\Datetime\DateFormatterInterface;
use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\extra_field\Plugin\ExtraFieldDisplayBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Display the creation date.
 *
 * @ExtraFieldDisplay(
 *   id = "drupalfr_release_creation_date",
 *   label = @Translation("Creation date"),
 *   bundles = {
 *     "node.release",
 *   }
 * )
 */
class CreationDate extends ExtraFieldDisplayBase implements ContainerFactoryPluginInterface {
  use StringTranslationTrait;

  /**
   * The date formatter service.
   *
   * @var \Drupal\Core\Datetime\DateFormatterInterface
   */
  protected $dateFormatter;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, DateFormatterInterface $date_formatter) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->dateFormatter = $date_formatter;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('date.formatter')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function view(ContentEntityInterface $entity) {
    /** @var \Drupal\node\NodeInterface $entity */

    $elements = [
      '#markup' => $this->t('Created the @date', [
        '@date' => $this->dateFormatter->format($entity->getCreatedTime(), 'jour_mois_annee_heure'),
      ]),
    ];

    return $elements;
  }

}
