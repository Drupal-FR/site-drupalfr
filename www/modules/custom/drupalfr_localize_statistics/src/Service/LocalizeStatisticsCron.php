<?php

namespace Drupal\drupalfr_localize_statistics\Service;

use Drupal\Component\Datetime\TimeInterface;
use Drupal\Component\Utility\Html;
use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Http\ClientFactory;
use Drupal\Core\State\StateInterface;
use GuzzleHttp\Exception\ClientException;
use GuzzleHttp\Exception\ServerException;
use Psr\Log\LoggerInterface;

/**
 * Methods executed during hook_cron().
 */
class LocalizeStatisticsCron implements LocalizeStatisticsCronInterface {

  /**
   * The time service.
   *
   * @var \Drupal\Component\Datetime\TimeInterface
   */
  protected $time;

  /**
   * The state service.
   *
   * @var \Drupal\Core\State\StateInterface
   */
  protected $state;

  /**
   * The entityTypeManager service.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * The drupalfr_localize_statistics logger channel.
   *
   * @var \Psr\Log\LoggerInterface
   */
  protected $logger;

  /**
   * The HTTP client.
   *
   * @var \GuzzleHttp\Client
   */
  protected $httpClient;

  /**
   * Constructor.
   *
   * @param \Drupal\Component\Datetime\TimeInterface $time
   *   The time service.
   * @param \Drupal\Core\State\StateInterface $state
   *   The state service.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The factory for configuration objects.
   * @param \Psr\Log\LoggerInterface $logger
   *   The drupalfr_localize_statistics logger channel.
   * @param \Drupal\Core\Http\ClientFactory $http_client_factory
   *   The HTTP client factory.
   */
  public function __construct(
    TimeInterface $time,
    StateInterface $state,
    EntityTypeManagerInterface $entity_type_manager,
    LoggerInterface $logger,
    ClientFactory $http_client_factory
  ) {
    $this->time = $time;
    $this->state = $state;
    $this->entityTypeManager = $entity_type_manager;
    $this->logger = $logger;
    $this->httpClient = $http_client_factory->fromOptions();
  }

  /**
   * {@inheritdoc}
   */
  public function cron() {
    $current_time = $this->time->getRequestTime();
    $last_check_timestamp = $this->state->get('drupalfr_localize_statistics.last_check_timestamp', 0);

    // Check the statistics every day (3600x24).
    if (($last_check_timestamp + 86400) < $current_time) {
      /** @var \Drupal\taxonomy\TermInterface[] $languages */
      $languages = $this->entityTypeManager->getStorage('taxonomy_term')
        ->loadByProperties(['vid' => 'localize_glossary_languages']);

      foreach ($languages as $language) {
        $language_term_id = $language->id();
        $langcode = $this->extractLangcode($language);

        if (!empty($langcode)) {
          $html = $this->FetchData($langcode);

          if (!empty($html)) {
            $data = $this->extractData($html);

            // Create content entities.
            $localize_statistics_storage = $this->entityTypeManager->getStorage('localize_statistics');
            foreach ($data as $localize_statistics_type => $info) {
              if (!is_null($info['value'])) {
                $localize_statistics = $localize_statistics_storage->create([
                  'type' => $localize_statistics_type,
                  'language' => $language_term_id,
                  'value' => $info['value'],
                  'created' => $current_time,
                ]);
                $localize_statistics->save();
              }
            }
          }
        }
      }

      $this->state->set('drupalfr_localize_statistics.last_check_timestamp', $current_time);
    }
  }

  /**
   * Helper function to extract a langcode from a taxonomy term.
   *
   * @param \Drupal\Core\Entity\ContentEntityInterface $content
   *   The taxonomy term containing the langcode.
   *
   * @return string
   *   The langcode.
   */
  protected function extractLangcode(ContentEntityInterface $content) : string {
    $value = '';
    if ($content->hasField('field_langcode')) {
      $langcode = $content->get('field_langcode')->getValue();
      if (!empty($langcode)) {
        $value = $langcode[0]['value'];
      }
      else {
        $this->logger->error('The @entity_type with ID @id does not have its langcode field filled.', [
          '@entity_type' => $content->getEntityType()->getLabel(),
          '@id' => $content->id(),
        ]);
      }
    }

    return $value;
  }

  /**
   * Fetch data from localize.drupal.org.
   *
   * @param string $langcode
   *   The language langcode.
   *
   * @return string
   *   The HTML string.
   */
  protected function FetchData(string $langcode) : string {
    $html = '';
    $url = 'https://localize.drupal.org/translate/languages/' . $langcode;

    try {
      $response = $this->httpClient->get($url);

      $html = $response->getBody()->getContents();
    }
    catch (ServerException $exception) {
      $this->logger->error('There was a server error when requesting localize.drupal.org. URL: @url, Status: @status, Message: @message.', [
        '@url' => $url,
        '@status' => $exception->getCode(),
        '@message' => $exception->getMessage(),
      ]);
    }
    catch (ClientException $exception) {
      $this->logger->error('There was a client error when requesting localize.drupal.org. URL: @url, Status: @status, Message: @message.', [
        '@url' => $url,
        '@status' => $exception->getCode(),
        '@message' => $exception->getMessage(),
      ]);
    }

    return $html;
  }

  /**
   * Helper function to extract the data from localize.drupal.org HTML.
   *
   * @param string $html
   *   The HTML from localize.drupal.org.
   *
   * @return array
   *   An array of data. keys are bundle of localize statistics and value are
   *   the values.
   */
  protected function extractData(string $html) : array {
    $dom = Html::load($html);
    $xpath = new \DOMXPath($dom);
    $list = $xpath->query('//*[@id="block-l10n-community-stats"]/div/div/div/ul/li');

    $mapping = [
      'translations' => [
        'pattern' => '/^(([\d])+) translations recorded$/',
        'value' => NULL,
      ],
      'suggestions' => [
        'pattern' => '/^(([\d])+) suggestions awaiting approval$/',
        'value' => NULL,
      ],
      'contributors' => [
        'pattern' => '/^(([\d])+) contributors$/',
        'value' => NULL,
      ],
      'to_translate' => [
        'pattern' => '/^(([\d])+) strings to translate$/',
        'value' => NULL,
      ],
    ];

    $list_length = $list->length;
    if (is_int($list_length)) {
      for ($iteration = 0; $iteration < $list_length; $iteration++) {
        $raw_value = $list->item($iteration)->nodeValue;

        foreach ($mapping as $localize_statistics_type => $info) {
          preg_match($info['pattern'], $raw_value, $matches);

          if (!empty($matches)) {
            $mapping[$localize_statistics_type]['value'] = $matches[1];
            break;
          }
        }
      }
    }

    // Check that all the statistics could have been extracted.
    foreach ($mapping as $localize_statistics_type => $info) {
      if (is_null($info['value'])) {
        $this->logger->error('Impossible to get statistics for type @type', [
          '@type' => $localize_statistics_type,
        ]);
      }
    }

    return $mapping;
  }

}
