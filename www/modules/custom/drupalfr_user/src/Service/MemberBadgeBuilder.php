<?php

namespace Drupal\drupalfr_user\Service;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Session\AccountProxyInterface;

/**
 * Provides a lazy builder for flag links.
 */
class MemberBadgeBuilder implements MemberBadgeBuilderInterface
{

  /**
   * The current user.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
    protected $currentUser;

  /**
   * The entityTypeManager service.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
    protected $entityTypeManager;

  /**
   * Constructor.
   *
   * @param \Drupal\Core\Session\AccountProxyInterface $current_user
   *   The current user.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The factory for configuration objects.
   */
    public function __construct(AccountProxyInterface $current_user, EntityTypeManagerInterface $entity_type_manager)
    {
        $this->currentUser = $current_user;
        $this->entityTypeManager = $entity_type_manager;
    }

  /**
   * {@inheritdoc}
   */
    public function build(int $user_id):array
    {
      /** @var \Drupal\user\UserInterface $user */
        $user = $this->entityTypeManager->getStorage('user')->load($user_id);

        return [
        '#theme' => 'drupalfr_user_member_badge',
        '#is_member' => $user->hasRole('member'),
        '#is_organization' => $user->hasRole('member_organization'),
        '#is_profile_owner' => ($this->currentUser->id() === $user->id()),
        ];
    }
}
