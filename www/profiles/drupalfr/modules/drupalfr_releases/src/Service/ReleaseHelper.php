<?php

namespace Drupal\drupalfr_releases\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Logger\LoggerChannelFactoryInterface;
use GuzzleHttp\Client;
use Drupal\node\Entity\Node;

/**
 * Functions for parse the xml, return the last stable release ...
 */
class ReleaseHelper implements ReleaseHelperInterface {

  /**
   * The factory for configuration objects.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * The entityTypeManager service.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * The logger service.
   *
   * @var \Drupal\Core\Logger\LoggerChannelFactoryInterface
   */
  protected $loggerFactory;

  /**
   * Constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The factory for configuration objects.
   * @param \Drupal\Core\Logger\LoggerChannelFactoryInterface $logger_factory
   *   The logger service.
   */
  public function __construct(ConfigFactoryInterface $config_factory, EntityTypeManagerInterface $entity_type_manager, LoggerChannelFactoryInterface $logger_factory) {
    $this->configFactory = $config_factory;
    $this->entityTypeManager = $entity_type_manager;
    $this->loggerFactory = $logger_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedReleases() {
    $release_config = $this->configFactory->get('drupalfr_releases.settings');

    $client = new Client();
    $response = $client->get($release_config->get('xml_address'));

    // Parsed XML and convert it back to array recursively.
    $parsed_response = json_decode(json_encode((array) simplexml_load_string($response->getBody()->getContents())), 1);
    return $parsed_response['releases']['release'];
  }

  /**
   * {@inheritdoc}
   */
  public function importReleaseListData(array $data) {
    $imported_release_ids = [];
    foreach ($data as $result) {
      // If node exist, we update it.
      $nid = $this->checkNodeExist('drupal_release', $result['name']);

      if (empty($nid)) {
        $node = Node::create(['type' => 'drupal_release']);
      }
      else {
        $node = Node::load($nid);
      }

      // Set values of fields.
      $node->set('title', $result['name']);
      $node->set('status', TRUE);
      $node->set('created', $result['date']);
      $node->set('field_release_version_major', $result['version_major']);
      if (isset($result['version_minor'])) {
        $node->set('field_release_version_minor', $result['version_minor']);
      }
      if (isset($result['version_patch'])) {
        $node->set('field_release_version_patch', $result['version_patch']);
      }
      if (isset($result['version_extra'])) {
        $node->set('field_release_version_extra', $result['version_extra']);
      }
      $node->set('field_release_link', $result['release_link']);
      $node->set('field_release_file_targz', $result['files']['file'][0]['url']);
      $node->set('field_release_file_zip', $result['files']['file'][1]['url']);
      $node->save();

      $imported_release_ids[] = $node->id();
      $this->loggerFactory->get('drupalfr_releases_import')
        ->notice('Release (' . $result['name'] . ') was imported/updated');
    }

    return $imported_release_ids;
  }

  /**
   * Check if node exist.
   *
   * @param string $type
   *   The type of node to check.
   * @param string $title
   *   The title of the node.
   *
   * @return mixed
   *   The node id if it exists.
   */
  protected function checkNodeExist($type, $title) {
    $query = $this->entityTypeManager->getStorage('node')->getQuery('AND')
      ->condition('type', $type)
      ->condition('title', $title)
      ->range(0, 1);

    $result = $query->execute();
    return reset($result);
  }

}
