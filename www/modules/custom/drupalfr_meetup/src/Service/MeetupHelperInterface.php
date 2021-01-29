<?php

namespace Drupal\drupalfr_meetup\Service;

/**
 * Interface Helper.
 *
 * @package Drupal\drupalfr_meetup
 */
interface MeetupHelperInterface {

  /**
   * Get the upcoming events.
   *
   * @return array
   *   An array of events.
   *
   * @see: https://github.com/blobaugh/Meetup-API-client-for-PHP/wiki
   * @see: https://www.meetup.com/meetup_api/docs/2/open_events/
   */
  public function getEvents();

  /**
   * Prepare Leaflet features from meetup events.
   *
   * @param array $events
   *   An array of events retrieved from the meetup API.
   *
   * @return array
   *   An array of events.
   */
  public function prepareLeafletFeatures(array $events);

}
