<?php

namespace Drupal\drupalfr_migrate\Plugin\migrate\process;

use Drupal\migrate\MigrateException;
use Drupal\migrate\ProcessPluginBase;
use Drupal\migrate\MigrateExecutableInterface;
use Drupal\migrate\Row;

/**
 * Prepare an array of links.
 *
 * Get the URIs and titles from the referenced properties.
 *
 * @MigrateProcessPlugin(
 *   id = "drupalfr_multilinks"
 * )
 */
class MultiLinks extends ProcessPluginBase {

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
        'uri' => $uri,
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

}
