<?php

namespace Drupal\drupalfr_localize_statistics\Form;

use Drupal\Core\Entity\EntityForm;
use Drupal\Core\Form\FormStateInterface;

/**
 * Localize Statistics type form.
 */
class LocalizeStatisticsTypeForm extends EntityForm {

  /**
   * {@inheritdoc}
   */
  public function form(array $form, FormStateInterface $form_state) {
    $form = parent::form($form, $form_state);

    $localize_statistics_type = $this->entity;
    $form['label'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Label'),
      '#maxlength' => 255,
      '#default_value' => $localize_statistics_type->label(),
      '#description' => $this->t("Label for the localize statistics type."),
      '#required' => TRUE,
    ];

    $form['id'] = [
      '#type' => 'machine_name',
      '#default_value' => $localize_statistics_type->id(),
      '#machine_name' => [
        'exists' => '\Drupal\drupalfr_localize_statistics\Entity\LocalizeStatisticsType::load',
      ],
      '#disabled' => !$localize_statistics_type->isNew(),
    ];

    /* You will need additional form elements for your custom properties. */

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $localize_statistics_type = $this->entity;
    $status = $localize_statistics_type->save();

    switch ($status) {
      case SAVED_NEW:
        $this->messenger()->addStatus($this->t('Created the %label localize statistics type.', [
          '%label' => $localize_statistics_type->label(),
        ]));
        break;

      default:
        $this->messenger()->addStatus($this->t('Saved the %label localize statistics type.', [
          '%label' => $localize_statistics_type->label(),
        ]));
    }
    $form_state->setRedirectUrl($localize_statistics_type->toUrl('collection'));
  }

}
