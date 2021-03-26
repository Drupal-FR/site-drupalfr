<?php

namespace Drupal\drupalfr_social\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Twitter configuration.
 *
 * @package Drupal\drupalfr_social\Form
 */
class Settings extends ConfigFormBase
{

  /**
   * {@inheritdoc}
   */
    protected function getEditableConfigNames()
    {
        return [
        'drupalfr_social.twitter',
        ];
    }

  /**
   * {@inheritdoc}
   */
    public function getFormId()
    {
        return 'drupalfr_social_settings_form';
    }

  /**
   * {@inheritdoc}
   */
    public function buildForm(array $form, FormStateInterface $form_state)
    {
        $twitter = $this->config('drupalfr_social.twitter');

        $form['twitter'] = [
        '#type' => 'fieldset',
        '#title' => $this->t('Twitter'),
        '#description' => $this->t('These values are defined in the settings.php file.'),
        ];

        $form['twitter']['consumer_key'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Consumer key'),
        '#size' => 64,
        '#default_value' => $twitter->get('consumer_key'),
        ];
        $form['twitter']['consumer_secret'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Consumer secret'),
        '#size' => 64,
        '#default_value' => $twitter->get('consumer_secret'),
        ];
        $form['twitter']['access_token'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Access token'),
        '#size' => 64,
        '#default_value' => $twitter->get('access_token'),
        ];
        $form['twitter']['access_token_secret'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Access token secret'),
        '#size' => 64,
        '#default_value' => $twitter->get('access_token_secret'),
        ];

        return parent::buildForm($form, $form_state);
    }

  /**
   * {@inheritdoc}
   */
    public function submitForm(array &$form, FormStateInterface $form_state)
    {
        parent::submitForm($form, $form_state);

        $this->config('drupalfr_social.twitter')
        ->set('consumer_key', $form_state->getValue('consumer_key'))
        ->set('consumer_secret', $form_state->getValue('consumer_secret'))
        ->set('access_token', $form_state->getValue('access_token'))
        ->set('access_token_secret', $form_state->getValue('access_token_secret'))
        ->save();
    }
}
