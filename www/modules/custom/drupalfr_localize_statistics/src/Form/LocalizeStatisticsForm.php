<?php

namespace Drupal\drupalfr_localize_statistics\Form;

use Drupal\Core\Entity\ContentEntityForm;
use Drupal\Core\Form\FormStateInterface;

/**
 * Form controller for localize statistics edit forms.
 *
 * @ingroup drupalfr_localize_statistics
 */
class LocalizeStatisticsForm extends ContentEntityForm
{

  /**
   * {@inheritdoc}
   */
    public function save(array $form, FormStateInterface $form_state)
    {
        $entity = $this->entity;

        $status = parent::save($form, $form_state);

        switch ($status) {
            case SAVED_NEW:
                $this->messenger()->addStatus($this->t('Created the %label localize statistics.', [
                '%label' => $entity->label(),
                ]));
                break;

            default:
                $this->messenger()->addStatus($this->t('Saved the %label localize statistics.', [
                '%label' => $entity->label(),
                ]));
        }
        $form_state->setRedirect('entity.localize_statistics.canonical', ['localize_statistics' => $entity->id()]);
    }
}
