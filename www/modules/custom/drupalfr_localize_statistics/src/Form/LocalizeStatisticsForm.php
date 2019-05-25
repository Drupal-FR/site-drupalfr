<?php

namespace Drupal\drupalfr_localize_statistics\Form;

use Drupal\Core\Entity\ContentEntityForm;
use Drupal\Core\Form\FormStateInterface;

/**
 * Form controller for Statistique de la traduction edit forms.
 *
 * @ingroup drupalfr_localize_statistics
 */
class LocalizeStatisticsForm extends ContentEntityForm {

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    /* @var $entity \Drupal\drupalfr_localize_statistics\Entity\LocalizeStatistics */
    $form = parent::buildForm($form, $form_state);

    $entity = $this->entity;

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $entity = $this->entity;

    $status = parent::save($form, $form_state);

    switch ($status) {
      case SAVED_NEW:
        drupal_set_message($this->t('Created the %label Statistique de la traduction.', [
          '%label' => $entity->label(),
        ]));
        break;

      default:
        drupal_set_message($this->t('Saved the %label Statistique de la traduction.', [
          '%label' => $entity->label(),
        ]));
    }
    $form_state->setRedirect('entity.localize_statistics.canonical', ['localize_statistics' => $entity->id()]);
  }

}
