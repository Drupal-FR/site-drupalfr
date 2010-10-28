<?php

function drupalfr4_format_date($timestamp) {
  $interval = time() - $timestamp;
  if (abs($interval) < 7 * 24 * 60 * 60) {
    return t('%ago ago', array('%ago' => format_interval($interval)));
  }
  else {
    return format_date($timestamp);
  }
}

function drupalfr4_user_badge($account) {
  $profile = content_profile_load('profile', $account->uid);

  $picture = content_format('field_picture', $profile->field_picture[0], 'userbadge_default');
  $name = check_plain($account->name);

  if (user_access('access user profiles')) {
    $picture = l($picture, 'user/' . $account->uid, array('html' => TRUE));
    $name = l($name, 'user/' . $account->uid, array('html' => TRUE));
  }

  if (module_exists('userpoints')) {
    $points = (int) userpoints_get_current_points($account->uid, variable_get(USERPOINTS_CATEGORY_PROFILE_DISPLAY_TID, 0));
  }

  return '<div class="user-badge clear-fix">' . $picture . '<p class="name">' . $name . '</p><p class="points">' . $points . '</p></div>';
}

function drupalfr4_preprocess_comment(&$variables) {
  if (!empty($variables['node']->flagged_comments[$variables['comment']->cid])) {
    $variables['comment_class'] = 'comment-accepted';
  }
  else {
    $variables['comment_class'] = '';
  }
}

function drupalfr4_fieldset($element) {
  if (!empty($element['#collapsible'])) {
    drupal_add_js('misc/collapse.js');

    if (!isset($element['#attributes']['class'])) {
      $element['#attributes']['class'] = '';
    }

    $element['#attributes']['class'] .= ' collapsible';
    if (!empty($element['#collapsed'])) {
      $element['#attributes']['class'] .= ' collapsed';
    }
  }

  $element['#attributes']['id'] = form_clean_id('edit-'. implode('-', $element['#parents']));

  return '<fieldset'. drupal_attributes($element['#attributes']) .'>'. ($element['#title'] ? '<legend>'. $element['#title'] .'</legend>' : '') . (isset($element['#description']) && $element['#description'] ? '<div class="description">'. $element['#description'] .'</div>' : '') . (!empty($element['#children']) ? $element['#children'] : '') . (isset($element['#value']) ? $element['#value'] : '') ."</fieldset>\n";
}

