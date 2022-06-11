<?php

namespace Drupal\drupalfr_migrate\Plugin\migrate\process;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\migrate\Plugin\MigrationInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\migrate\ProcessPluginBase;
use Drupal\migrate\MigrateExecutableInterface;
use Drupal\migrate\Row;

/**
 * Get an URI from an UUID.
 *
 * @MigrateProcessPlugin(
 *   id = "drupalfr_geturifromuuid"
 * )
 */
class GetUriFromUuid extends ProcessPluginBase implements ContainerFactoryPluginInterface {

  /**
   * An entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, MigrationInterface $migration, EntityTypeManagerInterface $entity_type_manager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entity_type_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition, MigrationInterface $migration = NULL) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $migration,
      $container->get('entity_type.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function transform($value, MigrateExecutableInterface $migrate_executable, Row $row, $destination_property) {
    // Expected structure: 'entity:node:uuid'.
    $value_part = explode(':', $value);

    // Handles only the case of entity.
    if ($value_part[0] != 'entity') {
      return $value;
    }

    /** @var \Drupal\Core\Entity\EntityInterface[] $entities */
    $entities = $this->entityTypeManager->getStorage($value_part[1])->loadByProperties(['uuid' => $value_part[2]]);

    if (!empty($entities)) {
      $entity = array_shift($entities);
      return $value_part[0] . ':' . $value_part[1] . '/' . $entity->id();
    }
    else {
      return $value;
    }
  }

}
