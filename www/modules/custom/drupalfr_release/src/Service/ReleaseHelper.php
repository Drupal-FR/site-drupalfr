<?php

namespace Drupal\drupalfr_release\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Logger\LoggerChannelFactoryInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;
use GuzzleHttp\ClientInterface;

/**
 * Functions for parse the xml, return the last stable release ...
 */
class ReleaseHelper implements ReleaseHelperInterface {
  use StringTranslationTrait;

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
   * The http client.
   *
   * Use \GuzzleHttp\Client for the property PHPDoc instead of
   * \GuzzleHttp\ClientInterface to get auto-completion on Guzzle's methods.
   *
   * @var \GuzzleHttp\Client
   */
  protected $httpClient;

  /**
   * Constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The factory for configuration objects.
   * @param \Drupal\Core\Logger\LoggerChannelFactoryInterface $logger_factory
   *   The logger service.
   * @param \GuzzleHttp\ClientInterface $http_client
   *   The http client.
   */
  public function __construct(
    ConfigFactoryInterface $config_factory,
    EntityTypeManagerInterface $entity_type_manager,
    LoggerChannelFactoryInterface $logger_factory,
    ClientInterface $http_client
  ) {
    $this->configFactory = $config_factory;
    $this->entityTypeManager = $entity_type_manager;
    $this->loggerFactory = $logger_factory;
    $this->httpClient = $http_client;
  }

  /**
   * {@inheritdoc}
   */
  public function getFeedReleases() {
    $release_config = $this->configFactory->get('drupalfr_release.settings');

    $response = $this->httpClient->get($release_config->get('xml_address'));

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
      $nid = $this->checkNodeExist('release', $result['name']);

      if (empty($nid)) {
        $node = Node::create(['type' => 'release']);
      }
      else {
        $node = $this->entityTypeManager->getStorage('node')->load($nid);
      }

      // Set values of fields.
      $node->set('title', $result['name']);
      $node->set('status', NodeInterface::PUBLISHED);
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
      $node->set('field_release_link', [
        'uri' => $result['release_link'],
        'title' => $result['name'],
      ]);
      $node->set('field_release_file_targz', [
        'uri' => $result['files']['file'][0]['url'],
        'title' => $this->t('Download (tar.gz)'),
        'options' => [
          'attributes' => [
            'class' => 'button hollow',
          ],
        ],
      ]);
      $node->set('field_release_file_zip', [
        'uri' => $result['files']['file'][1]['url'],
        'title' => $this->t('Download (zip)'),
        'options' => [
          'attributes' => [
            'class' => 'button primary',
          ],
        ],
      ]);
      if (isset($result['terms'])) {
        $node->set('field_release_type', $this->extractReleaseTypes($result['terms']['term']));
      }

      $node->save();

      $imported_release_ids[] = $node->id();
      $this->loggerFactory->get('drupalfr_release_import')
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

  /**
   * Convert release types from XML into Drupal value.
   *
   * @param array $terms
   *   The release types from the XML.
   *
   * @return array
   *   The array of release types.
   */
  protected function extractReleaseTypes(array $terms) {
    $release_types = [];

    if (!$this->isNumericArray($terms)) {
      return $this->extractReleaseTypes([$terms]);
    }

    foreach ($terms as $term) {
      $release_type = $this->extractReleaseType($term);
      if ($release_type) {
        $release_types[] = $release_type;
      }
    }

    return $release_types;
  }

  /**
   * Return the corresponding Drupal value of a release type.
   *
   * @param array $term
   *   The release types from the XML.
   *
   * @return string|bool
   *   The Drupal value. FALSE if no match is found.
   */
  protected function extractReleaseType(array $term) {
    $release_type = FALSE;
    switch ($term['value']) {
      case 'Security update':
        $release_type = 'security_update';
        break;

      case 'Bug fixes':
        $release_type = 'bug_fixes';
        break;

      case 'New features':
        $release_type = 'new_features';
        break;
    }

    return $release_type;
  }

  /**
   * Check if a array is numeric.
   *
   * @param array $array
   *   The array to check.
   *
   * @return bool
   *   TRUE if the array is numeric. FALSE in case of associative array.
   */
  protected function isNumericArray(array $array) {
    foreach ($array as $a => $b) {
      if (!is_int($a)) {
        return FALSE;
      }
    }
    return TRUE;
  }

}
