<?php

namespace Drupal\drupalfr_localize_statistics\Entity;

use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityTypeInterface;

/**
 * Defines the localize statistics entity.
 *
 * @ingroup drupalfr_localize_statistics
 *
 * @ContentEntityType(
 *   id = "localize_statistics",
 *   label = @Translation("localize statistics"),
 *   bundle_label = @Translation("localize statistics type"),
 *   handlers = {
 *     "view_builder" = "Drupal\Core\Entity\EntityViewBuilder",
 *     "list_builder" = "Drupal\drupalfr_localize_statistics\LocalizeStatisticsListBuilder",
 *     "views_data" = "Drupal\drupalfr_localize_statistics\Entity\LocalizeStatisticsViewsData",
 *
 *     "form" = {
 *       "default" = "Drupal\drupalfr_localize_statistics\Form\LocalizeStatisticsForm",
 *       "add" = "Drupal\drupalfr_localize_statistics\Form\LocalizeStatisticsForm",
 *       "edit" = "Drupal\drupalfr_localize_statistics\Form\LocalizeStatisticsForm",
 *       "delete" = "Drupal\drupalfr_localize_statistics\Form\LocalizeStatisticsDeleteForm",
 *     },
 *     "access" = "Drupal\drupalfr_localize_statistics\LocalizeStatisticsAccessControlHandler",
 *     "route_provider" = {
 *       "html" = "Drupal\drupalfr_localize_statistics\LocalizeStatisticsHtmlRouteProvider",
 *     },
 *   },
 *   base_table = "localize_statistics",
 *   admin_permission = "administer localize statistics entities",
 *   entity_keys = {
 *     "id" = "id",
 *     "bundle" = "type",
 *     "uuid" = "uuid",
 *   },
 *   links = {
 *     "canonical" = "/admin/content/localize_statistics/{localize_statistics}",
 *     "add-page" = "/admin/content/localize_statistics/add",
 *     "add-form" = "/admin/content/localize_statistics/add/{localize_statistics_type}",
 *     "edit-form" = "/admin/content/localize_statistics/{localize_statistics}/edit",
 *     "delete-form" = "/admin/content/localize_statistics/{localize_statistics}/delete",
 *     "collection" = "/admin/content/localize_statistics",
 *   },
 *   bundle_entity_type = "localize_statistics_type",
 *   field_ui_base_route = "entity.localize_statistics_type.edit_form"
 * )
 */
class LocalizeStatistics extends ContentEntityBase implements LocalizeStatisticsInterface {

  /**
   * {@inheritdoc}
   *
   * Format as: langcode - type - date.
   */
  public function label() {
    $label_parts = [];
    /** @var \Drupal\Core\Datetime\DateFormatterInterface $date_formatter */
    $date_formatter = \Drupal::service('date.formatter');

    // Langcode.
    $languages = $this->get('language')->referencedEntities();
    if (!empty($languages)) {
      /** @var \Drupal\taxonomy\TermInterface $language */
      $language = array_shift($languages);

      if ($language->hasField('field_langcode')) {
        $langcode = $language->get('field_langcode')->getValue();
        if (!empty($langcode)) {
          $label_parts[] = $langcode[0]['value'];
        }
      }
    }

    $label_parts[] = $this->bundle();
    $label_parts[] = $date_formatter->format($this->getCreatedTime(), 'fallback');
    return implode(' - ', $label_parts);
  }

  /**
   * {@inheritdoc}
   */
  public function getCreatedTime() {
    return $this->get('created')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function setCreatedTime($timestamp) {
    $this->set('created', $timestamp);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields = parent::baseFieldDefinitions($entity_type);

    $fields['created'] = BaseFieldDefinition::create('created')
      ->setLabel(t('Created'))
      ->setDescription(t('The time that the entity was created.'));

    $fields['language'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Language'))
      ->setDescription(t('The statistics language.'))
      ->setCardinality(1)
      ->setRequired(TRUE)
      ->setSetting('target_type', 'taxonomy_term')
      ->setSetting('handler', 'default')
      ->setSetting('handler_settings', [
        'target_bundles' => [
          'localize_glossary_languages' => 'localize_glossary_languages',
        ],
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    $fields['value'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Value'))
      ->setDescription(t('The statistics value.'))
      ->setCardinality(1)
      ->setRequired(TRUE)
      // Positive only.
      ->setSetting('unsigned', TRUE)
      ->setSetting('min', 0)
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    return $fields;
  }

}
