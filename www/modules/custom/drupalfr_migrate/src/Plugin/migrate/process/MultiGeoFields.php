<?php

namespace Drupal\drupalfr_migrate\Plugin\migrate\process;

use Drupal\migrate\MigrateException;
use Drupal\migrate\ProcessPluginBase;
use Drupal\migrate\MigrateExecutableInterface;
use Drupal\migrate\Row;

/**
 * Prepare an array of geoPHP object in wkt.
 *
 * @MigrateProcessPlugin(
 *   id = "drupalfr_multigeofields"
 * )
 */
class MultiGeoFields extends ProcessPluginBase {

  /**
   * {@inheritdoc}
   */
  public function transform($value, MigrateExecutableInterface $migrate_executable, Row $row, $destination_property) {
    if (empty($this->configuration['source'])) {
      throw new MigrateException('source is empty');
    }

    if (empty($this->configuration['delimiter'])) {
      throw new MigrateException('delimiter is empty');
    }

    $geophp_objects = explode($this->configuration['delimiter'], $row->getSourceProperty($this->configuration['source']));

    $value = [];
    foreach ($geophp_objects as $geophp_object) {
      $value[] = [
        'value' => $geophp_object,
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
