<?php

namespace Drupal\drupalfr_homepage\Controller;

use Drupal\Core\Controller\ControllerBase;

/**
 * Class HomepageController.
 *
 * @package Drupal\drupalfr_homepage\Controller
 */
class HomepageController extends ControllerBase {

  /**
   * Index.
   *
   * @return string
   *   Return empty string.
   */
  public function index() {
    return [
      '#type' => 'markup',
      '#markup' => '',
    ];
  }

}
