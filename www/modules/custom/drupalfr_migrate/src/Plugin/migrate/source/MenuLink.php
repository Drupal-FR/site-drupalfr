<?php

namespace Drupal\drupalfr_migrate\Plugin\migrate\source;

use Drupal\migrate_source_csv\Plugin\migrate\source\CSV;
use Drupal\migrate\Row;

/**
 * Source plugin for menu link.
 *
 * @MigrateSource(
 *   id = "drupalfr_migrate_menu_link_csv"
 * )
 */
class MenuLink extends CSV
{

  /**
   * {@inheritdoc}
   */
    public function prepareRow(Row $row)
    {
        if (!parent::prepareRow($row)) {
            return false;
        }

      // Prepare structure of options managed by menu_attributes.
        $menu_attributes_options = [
        'attributes' => [
        'title',
        'id',
        'name',
        'rel',
        'class',
        'style',
        'target',
        'accesskey',
        ],
        'item_attributes' => [
        'id',
        'class',
        'style',
        ],
        ];

        $menu_link_options = [];
        foreach ($menu_attributes_options as $options_group_key => $options) {
            $menu_link_options[$options_group_key] = [];
            foreach ($options as $option) {
                $menu_link_options[$options_group_key][$option] = $row->getSourceProperty($options_group_key . '_' . $option);
            }
        }

      // Special case for Fontawesome menu icons.
        $menu_link_options['fa_icon'] = $row->getSourceProperty('fa_icon');
        $menu_link_options['fa_icon_appearance'] = $row->getSourceProperty('fa_icon_appearance');

        $row->setSourceProperty('options', $menu_link_options);

        return true;
    }
}
