<?php

namespace Drupal\drupalfr_releases\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\drupalfr_releases\Service\ReleaseHelperInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class Imports for release xml.
 *
 * @package Drupal\drupalfr_releases\Form
 */
class Import extends FormBase {

  /**
   * The release helper service.
   *
   * @var \Drupal\drupalfr_releases\Service\ReleaseHelperInterface
   */
  protected $releaseHelper;

  /**
   * Constructs a ContentEntityForm object.
   *
   * @param \Drupal\drupalfr_releases\Service\ReleaseHelperInterface $release_helper
   *   The entity type manager.
   */
  public function __construct(ReleaseHelperInterface $release_helper) {
    $this->releaseHelper = $release_helper;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('drupalfr_releases.release_helper')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'drupalfr_releases_import_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $release_config = $this->config('drupalfr_releases.settings');

    if (!empty($release_config->get('xml_address'))) {
      $form['action']['import'] = [
        '#type' => 'submit',
        '#value' => $this->t('Import releases'),
      ];
    }
    else {
      $url = Url::fromRoute('drupalfr_releases.settings');
      if ($url->renderAccess($url->toRenderArray())) {
        drupal_set_message($this->t('Please set the XML URL on <a href=":url">the settings page</a>.', [':url' => $url->toString()]), 'error');
      }
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $batch = [
      'title' => $this->t('Import releases'),
      'operations' => [
        [
          '\Drupal\drupalfr_releases\ReleasesBatchHelper::importReleaseListBatch',
          [$this->releaseHelper->getFeedReleases()],
        ],
      ],
      'finished' => '\Drupal\drupalfr_releases\ReleasesBatchHelper::importReleaseBatchBatchFinished',
    ];

    batch_set($batch);
  }

}
