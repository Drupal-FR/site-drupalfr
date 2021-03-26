<?php

namespace Drupal\drupalfr_user\Plugin\ExtraField\Display;

use Drupal\Core\Entity\ContentEntityInterface;
use Drupal\extra_field\Plugin\ExtraFieldDisplayBase;

/**
 * Display a member badge.
 *
 * @ExtraFieldDisplay(
 *   id = "drupalfr_user_member_badge",
 *   label = @Translation("Member badge"),
 *   bundles = {
 *     "user.*",
 *   }
 * )
 */
class MemberBadge extends ExtraFieldDisplayBase
{

  /**
   * {@inheritdoc}
   */
    public function view(ContentEntityInterface $entity)
    {
        $elements = [
        '#lazy_builder' => ['drupalfr_user.member_badge_builder:build',
        [
          $entity->id(),
        ],
        ],
        '#create_placeholder' => true,
        ];

        return $elements;
    }
}
