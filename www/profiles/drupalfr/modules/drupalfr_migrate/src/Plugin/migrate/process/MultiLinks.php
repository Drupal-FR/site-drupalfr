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

    $value = [];
    foreach ($uris as $key => $uri) {
      $value[] = [
        'uri' => $uri,
        'title' => isset($titles[$key]) ? $titles[$key] : '',
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
