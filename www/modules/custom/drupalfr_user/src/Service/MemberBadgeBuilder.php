<?php

namespace Drupal\drupalfr_user\Service;

use Drupal\Core\Session\AccountProxyInterface;
use Drupal\user\Entity\User;

/**
 * Provides a lazy builder for flag links.
 */
class MemberBadgeBuilder implements MemberBadgeBuilderInterface {

  /**
   * The current user.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $currentUser;

  /**
   * Constructor.
   *
   * @param \Drupal\Core\Session\AccountProxyInterface $current_user
   *   The current user.
   */
  public function __construct(AccountProxyInterface $current_user) {
    $this->currentUser = $current_user;
  }

  /**
   * {@inheritdoc}
   */
  public function build($user_id) {
    /** @var \Drupal\user\UserInterface $user */
    $user = User::load($user_id);

    return [
      '#theme' => 'drupalfr_user_member_badge',
      '#is_member' => $user->hasRole('member'),
      '#is_organization' => $user->hasRole('member_organization'),
      '#is_profile_owner' => ($this->currentUser->id() == $user->id()),
    ];
  }

}
