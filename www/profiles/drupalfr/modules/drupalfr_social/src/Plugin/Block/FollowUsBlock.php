<?php

namespace Drupal\drupalfr_social\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'FollowUsBlock' block.
 *
 * @Block(
 *  id = "follow_us_block",
 *  admin_label = @Translation("Follow us block"),
 * )
 */
class FollowUsBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['url_facebook'] = array(
      '#type' => 'url',
      '#title' => $this->t('Facebook URL'),
      '#default_value' => $this->configuration['url_facebook'],
    );
    $form['url_twitter'] = array(
      '#type' => 'url',
      '#title' => $this->t('Twitter URL'),
      '#default_value' => $this->configuration['url_facebook'],
    );
    $form['url_linkedin'] = array(
      '#type' => 'url',
      '#title' => $this->t('LinkedIn URL'),
      '#default_value' => $this->configuration['url_facebook'],
    );
    $form['url_viadeo'] = array(
      '#type' => 'url',
      '#title' => $this->t('Viadeo URL'),
      '#default_value' => $this->configuration['url_facebook'],
    );
    $form['url_youtube'] = array(
      '#type' => 'url',
      '#title' => $this->t('Youtube URL'),
      '#default_value' => $this->configuration['url_facebook'],
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['url_facebook'] = $form_state->getValue('url_facebook');
    $this->configuration['url_twitter'] = $form_state->getValue('url_twitter');
    $this->configuration['url_linkedin'] = $form_state->getValue('url_linkedin');
    $this->configuration['url_viadeo'] = $form_state->getValue('url_viadeo');
    $this->configuration['url_youtube'] = $form_state->getValue('url_youtube');
  }

  /**
   * {@inheritdoc}
   */
  public function build() {

    $build = array(
      '#theme' => 'drupalfr_social_follow_us_block',
      '#url_facebook' => $this->configuration['url_facebook'],
      '#url_twitter' => $this->configuration['url_twitter'],
      '#url_linkedin' => $this->configuration['url_linkedin'],
      '#url_viadeo' => $this->configuration['url_viadeo'],
      '#url_youtube' => $this->configuration['url_youtube'],
    );

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return array(
      'url_facebook' => '',
      'url_twitter' => '',
      'url_linkedin' => '',
      'url_viadeo' => '',
      'url_youtube' => '',
    );
  }

}
