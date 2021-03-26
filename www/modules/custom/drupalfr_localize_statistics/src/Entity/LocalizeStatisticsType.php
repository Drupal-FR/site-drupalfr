<?php

namespace Drupal\drupalfr_localize_statistics\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBundleBase;

/**
 * Defines the localize statistics type entity.
 *
 * @ConfigEntityType(
 *   id = "localize_statistics_type",
 *   label = @Translation("localize statistics type"),
 *   handlers = {
 *     "view_builder" = "Drupal\Core\Entity\EntityViewBuilder",
 *     "list_builder" = "Drupal\drupalfr_localize_statistics\LocalizeStatisticsTypeListBuilder",
 *     "form" = {
 *       "add" = "Drupal\drupalfr_localize_statistics\Form\LocalizeStatisticsTypeForm",
 *       "edit" = "Drupal\drupalfr_localize_statistics\Form\LocalizeStatisticsTypeForm",
 *       "delete" = "Drupal\drupalfr_localize_statistics\Form\LocalizeStatisticsTypeDeleteForm"
 *     },
 *     "route_provider" = {
 *       "html" = "Drupal\Core\Entity\Routing\AdminHtmlRouteProvider",
 *     },
 *   },
 *   config_prefix = "localize_statistics_type",
 *   admin_permission = "administer site configuration",
 *   bundle_of = "localize_statistics",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *     "uuid" = "uuid"
 *   },
 *   links = {
 *     "canonical" = "/admin/structure/localize_statistics_type/{localize_statistics_type}",
 *     "add-form" = "/admin/structure/localize_statistics_type/add",
 *     "edit-form" = "/admin/structure/localize_statistics_type/{localize_statistics_type}/edit",
 *     "delete-form" = "/admin/structure/localize_statistics_type/{localize_statistics_type}/delete",
 *     "collection" = "/admin/structure/localize_statistics_type"
 *   }
 * )
 */
class LocalizeStatisticsType extends ConfigEntityBundleBase implements LocalizeStatisticsTypeInterface {

  /**
   * The localize statistics type ID.
   *
   * @var string
   */
  protected $id;

  /**
   * The localize statistics type label.
   *
   * @var string
   */
  protected $label;

}
