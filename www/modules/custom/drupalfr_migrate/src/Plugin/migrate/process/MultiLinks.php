<?php

namespace Drupal\drupalfr_migrate\Plugin\migrate\process;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\migrate\MigrateException;
use Drupal\migrate\Plugin\MigrationInterface;
use Drupal\migrate\ProcessPluginBase;
use Drupal\migrate\MigrateExecutableInterface;
use Drupal\migrate\Row;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Prepare an array of links.
 *
 * Get the URIs and titles from the referenced properties.
 *
 * @MigrateProcessPlugin(
 *   id = "drupalfr_multilinks"
 * )
 */
class MultiLinks extends ProcessPluginBase implements ContainerFactoryPluginInterface {

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
    if (empty($this->configuration['uri_property'])) {
      throw new MigrateException('uri_property is empty');
    }

    if (empty($this->configuration['delimiter'])) {
      throw new MigrateException('delimiter is empty');
    }

    $uris = explode($this->configuration['delimiter'], $row->getSourceProperty($this->configuration['uri_property']));
    $titles = [];
    if (!empty($this->configuration['title_property'])) {
      $titles = explode($this->configuration['delimiter'], $row->getSourceProperty($this->configuration['title_property']));
    }

    $targets = [];
    if (!empty($this->configuration['target_property'])) {
      $targets = explode($this->configuration['delimiter'], $row->getSourceProperty($this->configuration['target_property']));
    }

    $rels = [];
    if (!empty($this->configuration['rel_property'])) {
      $rels = explode($this->configuration['delimiter'], $row->getSourceProperty($this->configuration['rel_property']));
    }

    $classes = [];
    if (!empty($this->configuration['class_property'])) {
      $classes = explode($this->configuration['delimiter'], $row->getSourceProperty($this->configuration['class_property']));
    }

    $value = [];
    foreach ($uris as $key => $uri) {
      // Prepare options.
      $options = [
        'attributes' => [
          'target' => isset($targets[$key]) ? $targets[$key] : '',
          'rel' => isset($rels[$key]) ? $rels[$key] : '',
          'class' => isset($classes[$key]) ? $classes[$key] : '',
        ],
      ];

      $value[] = [
        'uri' => $this->getUriFromUuid($uri),
        'title' => isset($titles[$key]) ? $titles[$key] : '',
        'options' => $options,
      ];
    }

    return $value;
  }

  /**
   * {@inheritdoc}
   */
  public function multiple() {
    return TRUE;
  }

  /**
   * Get an URI from an UUID.
   */
  protected function getUriFromUuid($value) {
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
