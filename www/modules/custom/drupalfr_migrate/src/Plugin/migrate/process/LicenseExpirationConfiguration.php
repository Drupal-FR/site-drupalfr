<?php

namespace Drupal\drupalfr_migrate\Plugin\migrate\process;

use Drupal\migrate\MigrateException;
use Drupal\migrate\ProcessPluginBase;
use Drupal\migrate\MigrateExecutableInterface;
use Drupal\migrate\Row;

/**
 * Prepare an array of plugin configuration for the license expiration.
 *
 * @MigrateProcessPlugin(
 *   id = "drupalfr_licence_expiration_configuration"
 * )
 */
class LicenseExpirationConfiguration extends ProcessPluginBase
{

  /**
   * {@inheritdoc}
   */
    public function transform($value, MigrateExecutableInterface $migrate_executable, Row $row, $destination_property)
    {
        if (empty($this->configuration['license_expiration_value_property'])) {
            throw new MigrateException('license_expiration_value_property is empty');
        }
        if (empty($this->configuration['license_expiration_period_property'])) {
            throw new MigrateException('license_expiration_period_property is empty');
        }

        return [
        'interval' => [
        'interval' => $row->getSourceProperty($this->configuration['license_expiration_value_property']),
        'period' => $row->getSourceProperty($this->configuration['license_expiration_period_property']),
        ],
        ];
    }
}
