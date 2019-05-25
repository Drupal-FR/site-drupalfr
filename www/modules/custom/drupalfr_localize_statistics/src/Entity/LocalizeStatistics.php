<?php

namespace Drupal\drupalfr_localize_statistics\Entity;

use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityChangedTrait;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\user\UserInterface;

/**
 * Defines the Statistique de la traduction entity.
 *
 * @ingroup drupalfr_localize_statistics
 *
 * @ContentEntityType(
 *   id = "localize_statistics",
 *   label = @Translation("Statistique de la traduction"),
 *   bundle_label = @Translation("Statistique de la traduction type"),
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
 *   admin_permission = "administer statistique de la traduction entities",
 *   entity_keys = {
 *     "id" = "id",
 *     "bundle" = "type",
 *     "label" = "name",
 *     "uuid" = "uuid",
 *     "uid" = "user_id",
 *     "langcode" = "langcode",
 *     "status" = "status",
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

  use EntityChangedTrait;

  /**
   * {@inheritdoc}
   */
  public static function preCreate(EntityStorageInterface $storage_controller, array &$values) {
    parent::preCreate($storage_controller, $values);
    $values += [
      'user_id' => \Drupal::currentUser()->id(),
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    return $this->get('name')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function setName($name) {
    $this->set('name', $name);
    return $this;
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
  public function getOwner() {
    return $this->get('user_id')->entity;
  }

  /**
   * {@inheritdoc}
   */
  public function getOwnerId() {
    return $this->get('user_id')->target_id;
  }

  /**
   * {@inheritdoc}
   */
  public function setOwnerId($uid) {
    $this->set('user_id', $uid);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function setOwner(UserInterface $account) {
    $this->set('user_id', $account->id());
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function isPublished() {
    return (bool) $this->getEntityKey('status');
  }

  /**
   * {@inheritdoc}
   */
  public function setPublished($published) {
    $this->set('status', $published ? TRUE : FALSE);
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields = parent::baseFieldDefinitions($entity_type);

    $fields['user_id'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Authored by'))
      ->setDescription(t('The user ID of author of the Statistique de la traduction entity.'))
      ->setRevisionable(TRUE)
      ->setSetting('target_type', 'user')
      ->setSetting('handler', 'default')
      ->setTranslatable(TRUE)
      ->setDisplayOptions('view', [
        'label' => 'hidden',
        'type' => 'author',
        'weight' => 0,
      ])
      ->setDisplayOptions('form', [
        'type' => 'entity_reference_autocomplete',
        'weight' => 5,
        'settings' => [
          'match_operator' => 'CONTAINS',
          'size' => '60',
          'autocomplete_type' => 'tags',
          'placeholder' => '',
        ],
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE);

    $fields['name'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Name'))
      ->setDescription(t('The name of the Statistique de la traduction entity.'))
      ->setSettings([
        'max_length' => 50,
        'text_processing' => 0,
      ])
      ->setDefaultValue('')
      ->setDisplayOptions('view', [
        'label' => 'above',
        'type' => 'string',
        'weight' => -4,
      ])
      ->setDisplayOptions('form', [
        'type' => 'string_textfield',
        'weight' => -4,
      ])
      ->setDisplayConfigurable('form', TRUE)
      ->setDisplayConfigurable('view', TRUE)
      ->setRequired(TRUE);

    $fields['status'] = BaseFieldDefinition::create('boolean')
      ->setLabel(t('Publishing status'))
      ->setDescription(t('A boolean indicating whether the Statistique de la traduction is published.'))
      ->setDefaultValue(TRUE)
      ->setDisplayOptions('form', [
        'type' => 'boolean_checkbox',
        'weight' => -3,
      ]);

    $fields['created'] = BaseFieldDefinition::create('created')
      ->setLabel(t('Created'))
      ->setDescription(t('The time that the entity was created.'));

    $fields['changed'] = BaseFieldDefinition::create('changed')
      ->setLabel(t('Changed'))
      ->setDescription(t('The time that the entity was last edited.'));

    return $fields;
  }

}
