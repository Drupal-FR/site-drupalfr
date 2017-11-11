<?php

namespace Drupal\drupalfr_meetup\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\drupalfr_meetup\Service\MeetupHelperInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class MeetupController.
 *
 * @package Drupal\drupalfr_meetup\Controller
 */
class MeetupController extends ControllerBase implements ContainerInjectionInterface {

  /**
   * A meetup helper service.
   *
   * @var \Drupal\drupalfr_meetup\Service\MeetupHelperInterface
   */
  protected $meetupHelper;

  /**
   * Construct.
   *
   * @param \Drupal\drupalfr_meetup\Service\MeetupHelperInterface $meetup_helper
   *   A twitter service.
   */
  public function __construct(
    MeetupHelperInterface $meetup_helper
  ) {
    $this->meetupHelper = $meetup_helper;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('drupalfr_meetup.meetup_helper')
    );
  }

  /**
   * Index.
   *
   * @return array
   *   Return renderable array.
   */
  public function index() {
    $page = [
      '#theme' => 'drupalfr_meetup_page',
      '#title' => $this->t('Drupal Meetups'),
      '#cache' => [
        // 15 minutes.
        'max-age' => '900',
      ],
    ];

    $events = $this->meetupHelper->getEvents();
    if (!empty($events)) {
      $page['#events_list'] = [
        '#theme' => 'drupalfr_meetup_events',
        '#events' => $events,
      ];

      $map = leaflet_map_get_info('OSM Mapnik');
      $page['#map'] = leaflet_render_map($map, $this->meetupHelper->prepareLeafletFeatures($events), '600px');
    }

    return $page;
  }

}
