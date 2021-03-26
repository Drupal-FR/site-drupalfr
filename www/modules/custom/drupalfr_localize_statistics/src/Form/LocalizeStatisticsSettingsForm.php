<?php

namespace Drupal\drupalfr_localize_statistics\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Localize statistic settings.
 *
 * @ingroup drupalfr_localize_statistics
 */
class LocalizeStatisticsSettingsForm extends FormBase
{

  /**
   * Returns a unique string identifying the form.
   *
   * @return string
   *   The unique string identifying the form.
   */
    public function getFormId()
    {
        return 'localizestatistics_settings';
    }

  /**
   * Form submission handler.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   The current state of the form.
   */
    public function submitForm(array &$form, FormStateInterface $form_state)
    {
      // Empty implementation of the abstract submit class.
    }

  /**
   * Defines the settings form for localize statistics entities.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   The current state of the form.
   *
   * @return array
   *   Form definition array.
   */
    public function buildForm(array $form, FormStateInterface $form_state)
    {
        $form['localizestatistics_settings']['#markup'] = 'Settings form for localize statistics entities. Manage field settings here.';
        return $form;
    }
}
