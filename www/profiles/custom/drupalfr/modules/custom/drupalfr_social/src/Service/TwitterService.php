<?php

namespace Drupal\drupalfr_social\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Abraham\TwitterOAuth\TwitterOAuth;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\Url;

/**
 * Class TwitterService.
 *
 * @package Drupal\drupalfr_social
 */
class TwitterService implements TwitterServiceInterface {
  use StringTranslationTrait;

  /**
   * The factory for configuration objects.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * The connection object.
   *
   * @var \Abraham\TwitterOAuth\TwitterOAuth
   */
  protected $connection = NULL;

  /**
   * Constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   */
  public function __construct(ConfigFactoryInterface $config_factory) {
    $this->configFactory = $config_factory;
  }

  /**
   * Get the twitter connection.
   *
   * @return \Abraham\TwitterOAuth\TwitterOAuth
   *   A TwitterOAuth object.
   */
  protected function getConnection() {
    if (is_null($this->connection)) {
      $twitter = $this->configFactory->get('drupalfr_social.twitter');

      $consumer_key = $twitter->get('consumer_key');
      $consumer_secret = $twitter->get('consumer_secret');
      $access_token = $twitter->get('access_token');
      $access_token_secret = $twitter->get('access_token_secret');

      $connection = new TwitterOAuth($consumer_key, $consumer_secret, $access_token, $access_token_secret);
      $this->connection = $connection;
    }
    else {
      $connection = $this->connection;
    }

    return $connection;
  }

  /**
   * {@inheritdoc}
   */
  public function getStatuses($path, array $options) {
    $connection = $this->getConnection();
    $connection->setTimeouts(10, 15);

    $statuses = $connection->get($path, $options);

    if ($connection->getLastHttpCode() == 200) {
      return $statuses;
    }
    else {
      $url = Url::fromRoute('drupalfr_social.config');
      if ($url->renderAccess($url->toRenderArray())) {
        drupal_set_message($this->t('Unable to request Twitter. Please check your <a href=":url">twitter connection settings</a>.', array(':url' => $url->toString())), 'error');
      }
      return array();
    }
  }

}
