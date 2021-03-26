<?php

namespace Drupal\drupalfr_migrate\Plugin\migrate\process;

use Drupal\migrate\MigrateException;
use Drupal\migrate\ProcessPluginBase;
use Drupal\migrate\MigrateExecutableInterface;
use Drupal\migrate\Row;

/**
 * Prepare an array of plugin configuration for the license type.
 *
 * @MigrateProcessPlugin(
 *   id = "drupalfr_licence_type_role_configuration"
 * )
 */
class LicenseTypeRoleConfiguration extends ProcessPluginBase
{

  /**
   * {@inheritdoc}
   */
    public function transform($value, MigrateExecutableInterface $migrate_executable, Row $row, $destination_property)
    {
        if (empty($this->configuration['license_role_property'])) {
            throw new MigrateException('license_role_property is empty');
        }

        return [
        'license_role' => $row->getSourceProperty($this->configuration['license_role_property']),
        ];
    }
}
