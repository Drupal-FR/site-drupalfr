<?php

namespace Drupal\drupalfr_meetup\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\drupalfr_meetup\Service\MeetupHelperInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'DrupalfrMeetupHomeBlock' block.
 *
 * @Block(
 *  id = "drupalfr_meetup_home_block",
 *  admin_label = @Translation("Drupalfr meetup home block"),
 * )
 */
class DrupalfrMeetupHomeBlock extends BlockBase implements ContainerFactoryPluginInterface {

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
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
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
  public function defaultConfiguration() {
    return [
      'sub_title' => '',
      'description' => '',
    ] + parent::defaultConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['sub_title'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Sub-title'),
      '#default_value' => $this->configuration['sub_title'],
    ];
    $form['description'] = [
      '#type' => 'text_format',
      '#title' => $this->t('Description'),
      '#default_value' => $this->configuration['description'],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['sub_title'] = $form_state->getValue('sub_title');
    $this->configuration['description'] = $form_state->getValue('description');
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [];
    $build['sub_title']['#markup'] = '<h3>' . $this->configuration['sub_title'] . '</h3>';
    $build['description']['#markup'] = check_markup($this->configuration['description']['value'], $this->configuration['description']['format']);
    $build['discover']['#markup'] = "<a href='/association'><h4>Découvrez l'association</h4><p>Rejoignez la communauté et profitez de l'ensemble des ressources mises à votre disposition.</p></a>";

    $events = $this->meetupHelper->getEvents();
    if (!empty($events)) {
      $map = leaflet_map_get_info('OSM Mapnik');
      $build['map'] = leaflet_render_map($map, $this->meetupHelper->prepareLeafletFeatures($events), '400px');
      $build['map']['#cache'] = [
        // 15 minutes.
        'max-age' => '900',
      ];

      // Limit to three meetup.
      $events = array_slice($events, 0, 3);
      $build['events_list'] = [
        '#theme' => 'drupalfr_meetup_events',
        '#events' => $events,
        '#cache' => [
          // 15 minutes.
          'max-age' => '900',
        ],
      ];
    }

    return $build;
  }

}
