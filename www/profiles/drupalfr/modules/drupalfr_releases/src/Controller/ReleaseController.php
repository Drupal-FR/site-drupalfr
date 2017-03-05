<?php


namespace Drupal\drupalfr_release\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Routing\TrustedRedirectResponse;
use Drupal\drupalfr_release\Release;
use Drupal\node\Entity\Node;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Class ReleaseController.
 *
 * @package Drupal\drupalfr_release\Controller
 */
class ReleaseController extends ControllerBase {

  /**
   * Redirect to drupal.org on the last stable release of Drupal
   *
   * @return string
   *   Return empty string.
   */
  public function getLast() {
    $release = new Release();
    $result = $release->getLastPublished();

    if(empty($result)){
      return $this->redirect('drupalfr_homepage.homepage_controller_index');
    }

    $drupal_release = Node::load(reset($result));
    $field_link_value = $drupal_release->get('field_release_link')->getValue();

    if(empty($field_link_value[0]['uri'])){
      return $this->redirect('drupalfr_homepage.homepage_controller_index');
    }

    return new TrustedRedirectResponse($field_link_value[0]['uri']);
  }

}
