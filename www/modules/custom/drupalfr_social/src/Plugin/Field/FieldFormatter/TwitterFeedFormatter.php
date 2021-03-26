<?php

namespace Drupal\drupalfr_social\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\drupalfr_social\Service\TwitterServiceInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Plugin implementation of the 'twitter feed' formatter.
 *
 * @FieldFormatter(
 *   id = "drupalfr_social_twitter_feed",
 *   label = @Translation("Twitter feed (Drupalfr)"),
 *   field_types = {
 *     "string"
 *   }
 * )
 */
class TwitterFeedFormatter extends FormatterBase implements ContainerFactoryPluginInterface
{

  /**
   * The twitter service.
   *
   * @var \Drupal\drupalfr_social\Service\TwitterServiceInterface
   */
    protected $twitterService;

  /**
   * Constructs a TwitterFeedFormatter object.
   *
   * @param string $plugin_id
   *   The plugin_id for the formatter.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Field\FieldDefinitionInterface $field_definition
   *   The definition of the field to which the formatter is associated.
   * @param array $settings
   *   The formatter settings.
   * @param string $label
   *   The formatter label display setting.
   * @param string $view_mode
   *   The view mode.
   * @param array $third_party_settings
   *   Third party settings.
   * @param \Drupal\drupalfr_social\Service\TwitterServiceInterface $twitter_service
   *   The twitter service.
   */
    public function __construct(
        $plugin_id,
        $plugin_definition,
        FieldDefinitionInterface $field_definition,
        array $settings,
        $label,
        $view_mode,
        array $third_party_settings,
        TwitterServiceInterface $twitter_service
    ) {
        parent::__construct($plugin_id, $plugin_definition, $field_definition, $settings, $label, $view_mode, $third_party_settings);
        $this->twitterService = $twitter_service;
    }

  /**
   * {@inheritdoc}
   */
    public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition)
    {
        return new static(
            $plugin_id,
            $plugin_definition,
            $configuration['field_definition'],
            $configuration['settings'],
            $configuration['label'],
            $configuration['view_mode'],
            $configuration['third_party_settings'],
            $container->get('drupalfr_social.twitter')
        );
    }

  /**
   * {@inheritdoc}
   */
    public function viewElements(FieldItemListInterface $items, $langcode = null)
    {
        $element = [];

        foreach ($items as $delta => $item) {
            $statuses = $this->twitterService->getStatuses('statuses/user_timeline', [
            'screen_name' => $item->value,
            'count' => 3,
            'exclude_replies' => false,
            ]);

            if (!empty($statuses)) {
                  $element[$delta] = [
                    '#theme' => 'drupalfr_social_twitter_statuses',
                    '#statuses' => $statuses,
                    '#cache' => [
                  // 15 minutes.
                  'max-age' => '900',
                    ],
                  ];
            }
        }
        return $element;
    }
}
