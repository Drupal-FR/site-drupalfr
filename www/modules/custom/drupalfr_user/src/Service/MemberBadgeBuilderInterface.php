<?php

namespace Drupal\drupalfr_user\Service;

/**
 * Provides a lazy builder for member's badge.
 */
interface MemberBadgeBuilderInterface
{

  /**
   * Lazy builder callback for displaying a member's badge.
   *
   * @param int $user_id
   *   The user id which profile is viewed.
   *
   * @return array
   *   A render array for the member's badge.
   */
    public function build(int $user_id): array;
}
