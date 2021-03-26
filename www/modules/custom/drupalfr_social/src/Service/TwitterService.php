<?php

namespace Drupal\drupalfr_social\Service;

use Abraham\TwitterOAuth\TwitterOAuthException;
use Drupal\Core\Config\ConfigFactoryInterface;
use Abraham\TwitterOAuth\TwitterOAuth;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\Url;

/**
 * Twitter service interface.
 *
 * @package Drupal\drupalfr_social
 */
class TwitterService implements TwitterServiceInterface
{

    use StringTranslationTrait;

  /**
   * The factory for configuration objects.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
    protected $configFactory;

  /**
   * The messenger service.
   *
   * @var \Drupal\Core\Messenger\MessengerInterface
   */
    protected $messenger;

  /**
   * The connection object.
   *
   * @var \Abraham\TwitterOAuth\TwitterOAuth
   */
    protected $connection = null;

  /**
   * Constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Messenger\MessengerInterface $messenger
   *   The messenger service.
   */
    public function __construct(ConfigFactoryInterface $config_factory, MessengerInterface $messenger)
    {
        $this->configFactory = $config_factory;
        $this->messenger = $messenger;
    }

  /**
   * Get the twitter connection.
   *
   * @return \Abraham\TwitterOAuth\TwitterOAuth
   *   A TwitterOAuth object.
   */
    protected function getConnection()
    {
        if (is_null($this->connection)) {
            $twitter = $this->configFactory->get('drupalfr_social.twitter');

            $consumer_key = $twitter->get('consumer_key');
            $consumer_secret = $twitter->get('consumer_secret');
            $access_token = $twitter->get('access_token');
            $access_token_secret = $twitter->get('access_token_secret');

            $this->connection = new TwitterOAuth($consumer_key, $consumer_secret, $access_token, $access_token_secret);
        }

        return $this->connection;
    }

  /**
   * {@inheritdoc}
   */
    public function getStatuses($path, array $options)
    {
        $error = false;
        $connection = $this->getConnection();

        try {
            $statuses = $connection->get($path, $options);

            if ($connection->getLastHttpCode() != 200) {
                $statuses = [];
                $error = true;
            }
        } catch (TwitterOAuthException $exception) {
            $statuses = [];
            $error = true;
        }

        if ($error) {
            $url = Url::fromRoute('drupalfr_social.config');
            if ($url->renderAccess($url->toRenderArray())) {
                $this->messenger->addError($this->t('Unable to request Twitter. Please check your <a href=":url">twitter connection settings</a>.', [':url' => $url->toString()]));
            }
        }

        return $statuses;
    }
}
