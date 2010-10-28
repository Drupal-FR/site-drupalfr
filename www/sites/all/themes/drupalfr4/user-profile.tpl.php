<?php
  // print_r(array_keys($profile));
  unset($profile['Informations personnelles']);
  unset($profile['Evènements']);
  unset($profile['simplenews']);
  $full_profile = content_profile_load('profile', $account->uid);
?>
<div class="profile">
<div class="grid-4 alpha left-pane">
<?php
  print content_format('field_picture', $full_profile->field_picture[0], 'userpicture_default');
?>
<?php
  if (isset($profile['userpoints'])) {
    $points = array(
      '#type' => 'user_profile_item',
      '#title' => t('!Points', userpoints_translation()),
      '#value' => userpoints_get_current_points($account->uid, variable_get(USERPOINTS_CATEGORY_PROFILE_DISPLAY_TID, 0))
    );
    print '<div class="points">' . drupal_render($points) . '</div>';
  }
  unset($profile['userpoints']);
?>
</div>
<div class="grid-6 center-pane">
<?php print $profile['content_profile']; unset($profile['content_profile']); ?>
</div>
<div class="grid-3 omega">
<?php print implode('', $profile) ?>
</div>
</div>
