<?php

namespace Drupal\drupalfr_meetup\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\Url;
// @codingStandardsIgnoreStart
use MeetupEvents;
use MeetupKeyAuthConnection;
use MeetupUnauthorizedRequestException;
// @codingStandardsIgnoreEnd

/**
 * Helper.
 *
 * @package Drupal\drupalfr_meetup
 */
class MeetupHelper implements MeetupHelperInterface {
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
   * @var \MeetupKeyAuthConnection
   */
  protected $connection = NULL;

  /**
   * The meetup events object.
   *
   * @var \MeetupEvents
   */
  protected $meetupEvents = NULL;

  /**
   * Constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Messenger\MessengerInterface $messenger
   *   The messenger service.
   */
  public function __construct(ConfigFactoryInterface $config_factory, MessengerInterface $messenger) {
    $this->configFactory = $config_factory;
    $this->messenger = $messenger;
  }

  /**
   * Get the meetup connection.
   *
   * @return \MeetupKeyAuthConnection
   *   A Meetup connection object.
   */
  protected function getConnection() {
    if (is_null($this->connection)) {
      $settings = $this->configFactory->get('drupalfr_meetup.settings');
      $this->connection = new MeetupKeyAuthConnection($settings->get('api_key'));
    }

    return $this->connection;
  }

  /**
   * Get the Meetup Event object.
   *
   * @return \MeetupEvents
   *   A Meetup event object.
   */
  protected function getMeetupEvents() {
    if (is_null($this->meetupEvents)) {
      $this->meetupEvents = new MeetupEvents($this->getConnection());
    }

    return $this->meetupEvents;
  }

  /**
   * {@inheritdoc}
   */
  public function getEvents() {
    $settings = $this->configFactory->get('drupalfr_meetup.settings');

    $events = [];
    try {
      $events = $this->getMeetupEvents()->getEvents([
        'group_urlname' => $settings->get('group_urlname'),
        'status' => 'upcoming',
      ]);
    }
    catch (MeetupUnauthorizedRequestException $exception) {
      $url = Url::fromRoute('drupalfr_meetup.config');
      if ($url->renderAccess($url->toRenderArray())) {
        $this->messenger->addError($this->t('Unable to request Meetup. Please check your <a href=":url">meetup connection settings</a>.', [':url' => $url->toString()]));
      }
    }
    return $events;
  }

  /**
   * {@inheritdoc}
   */
  public function prepareLeafletFeatures(array $events) {
    $features = [];
    foreach ($events as $event) {
      if (isset($event['venue'])) {
        $features[] = [
          'type' => 'point',
          'lat' => $event['venue']['lat'],
          'lon' => $event['venue']['lon'],
        ];
      }
    }
    return $features;
  }

}
