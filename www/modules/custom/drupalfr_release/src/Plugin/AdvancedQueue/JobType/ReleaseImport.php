<?php

namespace Drupal\drupalfr_release\Plugin\AdvancedQueue\JobType;

use Drupal\advancedqueue\Job;
use Drupal\advancedqueue\JobResult;
use Drupal\advancedqueue\Plugin\AdvancedQueue\JobType\JobTypeBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\drupalfr_release\Service\ReleaseHelperInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides the job type for importing Drupal release.
 *
 * @AdvancedQueueJobType(
 *   id = "drupalfr_release_import",
 *   label = @Translation("Release import"),
 * )
 */
class ReleaseImport extends JobTypeBase implements ContainerFactoryPluginInterface {

  /**
   * The release helper service.
   *
   * @var \Drupal\drupalfr_release\Service\ReleaseHelperInterface
   */
  protected $releaseHelper;

  /**
   * Constructs a new ReleaseImport object.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin ID for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\drupalfr_release\Service\ReleaseHelperInterface $release_helper
   *   The release helper service.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ReleaseHelperInterface $release_helper) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->releaseHelper = $release_helper;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
          $configuration,
          $plugin_id,
          $plugin_definition,
          $container->get('drupalfr_release.release_helper')
      );
  }

  /**
   * {@inheritdoc}
   */
  public function process(Job $job) {
    $data = $job->getPayload()['data'];

    $import_result = $this->releaseHelper->importReleaseListData([$data]);

    if (empty($import_result)) {
      return JobResult::failure('Import failed.');
    }

    return JobResult::success();
  }

}
