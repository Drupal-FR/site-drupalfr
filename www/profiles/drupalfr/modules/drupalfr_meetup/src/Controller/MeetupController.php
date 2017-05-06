<?php

namespace Drupal\drupalfr_meetup\Controller;

use Drupal\Core\Controller\ControllerBase;
use MeetupEvents;
use MeetupKeyAuthConnection;

/**
 * Class MeetupController.
 *
 * @package Drupal\drupalfr_meetup\Controller
 */
class MeetupController extends ControllerBase {

  /**
   * Index.
   *
   * @return array
   *   Return renderable array.
   */
  public function index() {
    $settings = $this->config('drupalfr_meetup.settings');

    $connection = new MeetupKeyAuthConnection($settings->get('api_key'));
    $meetup_events = new MeetupEvents($connection);
    $events = $meetup_events->getEvents([
      'group_urlname' => $settings->get('group_urlname'),
      'status' => 'upcoming',
    ]);

    $page = [];
    if (!empty($events)) {
      $page['events'] = [
        '#theme' => 'drupalfr_meetup_events',
        '#events' => $events,
        '#cache' => [
          // 15 minutes.
          'max-age' => '900',
        ],
      ];
    }

    return $page;
  }

}
