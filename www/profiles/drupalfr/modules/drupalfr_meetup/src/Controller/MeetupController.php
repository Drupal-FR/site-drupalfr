<?php

namespace Drupal\drupalfr_meetup\Controller;

use Drupal\Core\Controller\ControllerBase;

/**
 * Class MeetupController.
 *
 * @package Drupal\drupalfr_meetup\Controller
 */
class MeetupController extends ControllerBase {

  /**
   * Index.
   *
   * @return array
   *   Return renderable array.
   */
  public function index() {
    return [
      '#type' => 'markup',
      '#markup' => '',
    ];
  }

}
