<?php

namespace Drupal\drupalfr_meetup\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class Settings.
 *
 * @package Drupal\drupalfr_meetup\Form
 */
class Settings extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'drupalfr_meetup.settings',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'drupalfr_meetup_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $settings = $this->config('drupalfr_meetup.settings');

    $form['meetup'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Meetup'),
      '#description' => $this->t('These values are defined in the settings.php file.'),
    ];

    $form['meetup']['api_key'] = [
      '#type' => 'textfield',
      '#title' => $this->t('API key'),
      '#size' => 64,
      '#default_value' => $settings->get('api_key'),
    ];
    $form['meetup']['group_urlname'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Group URL name'),
      '#size' => 64,
      '#default_value' => $settings->get('group_urlname'),
    ];

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    parent::validateForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    parent::submitForm($form, $form_state);

    $this->config('drupalfr_meetup.settings')
      ->set('api_key', $form_state->getValue('api_key'))
      ->set('group_urlname', $form_state->getValue('group_urlname'))
      ->save();
  }

}
