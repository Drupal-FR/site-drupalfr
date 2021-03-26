<?php

namespace Drupal\drupalfr_meetup\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\drupalfr_meetup\Service\MeetupHelperInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'DrupalfrMeetupSidebarBlock' block.
 *
 * @Block(
 *  id = "drupalfr_meetup_sidebar_block",
 *  admin_label = @Translation("Drupalfr meetup sidebar block"),
 * )
 */
class DrupalfrMeetupSidebarBlock extends BlockBase implements ContainerFactoryPluginInterface
{

  /**
   * Drupal\drupalfr_meetup\Service\MeetupHelper definition.
   *
   * @var \Drupal\drupalfr_meetup\Service\MeetupHelperInterface
   */
    protected $meetupHelper;

  /**
   * Constructs a new DrupalfrMeetupHomeBlock object.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param string $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\drupalfr_meetup\Service\MeetupHelperInterface $drupalfr_meetup_meetup_helper
   *   The meetup helper service.
   */
    public function __construct(
        array $configuration,
        $plugin_id,
        $plugin_definition,
        MeetupHelperInterface $drupalfr_meetup_meetup_helper
    ) {
        parent::__construct($configuration, $plugin_id, $plugin_definition);
        $this->meetupHelper = $drupalfr_meetup_meetup_helper;
    }

  /**
   * {@inheritdoc}
   */
    public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition)
    {
        return new static(
            $configuration,
            $plugin_id,
            $plugin_definition,
            $container->get('drupalfr_meetup.meetup_helper')
        );
    }

  /**
   * {@inheritdoc}
   */
    public function build()
    {
        $build = [
        '#cache' => [
        // 15 minutes.
        'max-age' => '900',
        ],
        ];
        $events = $this->meetupHelper->getEvents();
        if (!empty($events)) {
          // Limit to three meetups.
            $events = array_slice($events, 0, 3);
            $build['events_list'] = [
            '#theme' => 'drupalfr_meetup_events',
            '#events' => $events,
            ];
        }

        return $build;
    }
}
