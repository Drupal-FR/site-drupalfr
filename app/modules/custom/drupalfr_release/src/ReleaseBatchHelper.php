<?php

namespace Drupal\drupalfr_release;

use Drupal\Core\StringTranslation\PluralTranslatableMarkup;

/**
 * Class ReleaseBatchHelper.
 *
 * Contains static method to use for batch operations.
 *
 * @package Drupal\drupalfr_release
 */
class ReleaseBatchHelper {

  /**
   * Batch operation.
   *
   * @param array $data
   *   An array of release from a release endpoint.
   * @param array $context
   *   Batch context information.
   */
  public static function importReleaseListBatch(array $data, array &$context) {
    if (empty($context['sandbox'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['max'] = count($data);
    }

    $limit = 10;
    $sub_data = array_slice($data, $context['sandbox']['progress'], $limit);

    /** @var \Drupal\drupalfr_release\Service\ReleaseHelperInterface $release_helper */
    $release_helper = \Drupal::service('drupalfr_release.release_helper');
    $result_ids = $release_helper->importReleaseListData($sub_data);

    $context['results'] = array_merge($context['results'], $result_ids);
    $context['sandbox']['progress'] += count($sub_data);
    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
  }

  /**
   * Batch finish callback.
   *
   * @param bool $success
   *   A boolean indicating whether the batch has completed successfully.
   * @param array $results
   *   The value set in $context['results'] by callback_batch_operation().
   * @param array $operations
   *   If $success is FALSE, contains the operations that remained unprocessed.
   */
  public static function importReleaseBatchBatchFinished($success, array $results, array $operations) {
    $messenger = \Drupal::messenger();
    if ($success) {
      $message = new PluralTranslatableMarkup(
        count($results),
        'One release processed.',
        '@count releases processed.'
      );
      $messenger->addStatus($message);
    }
    else {
      $messenger->addError(t('Finished with an error.'));
    }
  }

}
