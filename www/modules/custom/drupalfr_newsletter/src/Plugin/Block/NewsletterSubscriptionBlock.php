<?php

namespace Drupal\drupalfr_newsletter\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'NewsletterSubscriptionBlock' block.
 *
 * @Block(
 *  id = "newsletter_subscription_block",
 *  admin_label = @Translation("Newsletter subscription block"),
 * )
 */
class NewsletterSubscriptionBlock extends BlockBase {
  public const DRUPALFR_NEWSLETTER_DEFAULT_ACCOUNT_HASH = 'eb7ebedd32b3b2fc0ac192ee9';
  public const DRUPALFR_NEWSLETTER_DEFAULT_MAILING_LIST_HASH = '27e99cb1af';
  public const DRUPALFR_NEWSLETTER_DEFAULT_ANTI_SPAM_TOKEN = 'b_eb7ebedd32b3b2fc0ac192ee9_27e99cb1af';

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['account_hash'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Account hash'),
      '#description' => $this->t('Mailchimp account hash.'),
      '#default_value' => $this->configuration['account_hash'],
      '#required' => TRUE,
    ];
    $form['mailing_list_hash'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Mailing list hash'),
      '#description' => $this->t('Mailchimp mailing list hash.'),
      '#default_value' => $this->configuration['mailing_list_hash'],
      '#required' => TRUE,
    ];
    $form['anti_spam_token'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Anti-spam token'),
      '#description' => $this->t('Mailchimp anti-spam token'),
      '#default_value' => $this->configuration['anti_spam_token'],
      '#required' => TRUE,
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['account_hash'] = $form_state->getValue('account_hash');
    $this->configuration['mailing_list_hash'] = $form_state->getValue('mailing_list_hash');
    $this->configuration['anti_spam_token'] = $form_state->getValue('anti_spam_token');
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [
      '#theme' => 'drupalfr_newsletter_subscription_block',
      '#account_hash' => $this->configuration['account_hash'],
      '#mailing_list_hash' => $this->configuration['mailing_list_hash'],
      '#anti_spam_token' => $this->configuration['anti_spam_token'],
    ];

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return [
      'account_hash' => self::DRUPALFR_NEWSLETTER_DEFAULT_ACCOUNT_HASH,
      'mailing_list_hash' => self::DRUPALFR_NEWSLETTER_DEFAULT_MAILING_LIST_HASH,
      'anti_spam_token' => self::DRUPALFR_NEWSLETTER_DEFAULT_ANTI_SPAM_TOKEN,
    ];
  }

}
