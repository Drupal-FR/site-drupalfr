<?php

namespace Drupal\drupalfr_releases\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class Settings for release xml.
 *
 * @package Drupal\drupalfr_releases\Form
 */
class Settings extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'drupalfr_releases.settings',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'drupalfr_releases_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $twitter = $this->config('drupalfr_releases.settings');

    $form['xml_address'] = [
      '#type' => 'textfield',
      '#title' => $this->t('XML address'),
      '#default_value' => $twitter->get('xml_address'),
      '#required' => TRUE,
    ];

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    parent::submitForm($form, $form_state);

    $this->config('drupalfr_releases.settings')
      ->set('xml_address', $form_state->getValue('xml_address'))
      ->save();
  }

}
