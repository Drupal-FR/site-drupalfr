<?php

namespace Drupal\drupalfr_release\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\Core\Url;
use Drupal\drupalfr_release\Service\ReleaseHelperInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class Imports for release xml.
 *
 * @package Drupal\drupalfr_release\Form
 */
class Import extends FormBase
{

  /**
   * The release helper service.
   *
   * @var \Drupal\drupalfr_release\Service\ReleaseHelperInterface
   */
    protected $releaseHelper;

  /**
   * The messenger service.
   *
   * @var \Drupal\Core\Messenger\MessengerInterface
   */
    protected $messenger;

  /**
   * Constructs a ContentEntityForm object.
   *
   * @param \Drupal\drupalfr_release\Service\ReleaseHelperInterface $release_helper
   *   The entity type manager.
   * @param \Drupal\Core\Messenger\MessengerInterface $messenger
   *   The messenger service.
   */
    public function __construct(ReleaseHelperInterface $release_helper, MessengerInterface $messenger)
    {
        $this->releaseHelper = $release_helper;
        $this->messenger = $messenger;
    }

  /**
   * {@inheritdoc}
   */
    public static function create(ContainerInterface $container)
    {
        return new static(
            $container->get('drupalfr_release.release_helper'),
            $container->get('messenger')
        );
    }

  /**
   * {@inheritdoc}
   */
    public function getFormId()
    {
        return 'drupalfr_release_import_form';
    }

  /**
   * {@inheritdoc}
   */
    public function buildForm(array $form, FormStateInterface $form_state)
    {
        $release_config = $this->config('drupalfr_release.settings');

        if (!empty($release_config->get('xml_address'))) {
            $form['action']['import'] = [
            '#type' => 'submit',
            '#value' => $this->t('Import releases'),
            ];
        } else {
            $url = Url::fromRoute('drupalfr_release.settings');
            if ($url->renderAccess($url->toRenderArray())) {
                $this->messenger->addError($this->t('Please set the XML URL on <a href=":url">the settings page</a>.', [':url' => $url->toString()]));
            }
        }

        return $form;
    }

  /**
   * {@inheritdoc}
   */
    public function submitForm(array &$form, FormStateInterface $form_state)
    {
        $batch = [
        'title' => $this->t('Import releases'),
        'operations' => [
        [
          '\Drupal\drupalfr_release\ReleaseBatchHelper::importReleaseListBatch',
          [$this->releaseHelper->getFeedReleases()],
        ],
        ],
        'finished' => '\Drupal\drupalfr_release\ReleaseBatchHelper::importReleaseBatchBatchFinished',
        ];

        batch_set($batch);
    }
}
