<?php
// $Id: userpoints.theme.inc,v 1.1.2.3 2010/11/19 00:47:21 berdir Exp $

/**
 * @file
 * Theme functions for userpoints.module
 */

/*
 * Themes the output of users by points page.
 *
 * @return
 *   HTML of the table and a pager
 *
 * @see theme_userpoints_list_users_rows()
 */
function theme_userpoints_list_users($attributes) {
  $header = $attributes['header'];
  $rows = $attributes['rows'];
  $tid = $attributes['tid'];

  // If there is only one category, there is no sense in display the category filter dropdown
  $output = array();
  if (count(userpoints_get_categories()) > 1) {
    $output['form'] = drupal_get_form('userpoints_filter_cat_select', 'userpoints/', $tid);
  }
  $output['table'] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );
  $output['pager'] = array(
    '#markup' => theme('pager'),
  );
  return $output;
}

/**
 * Themes the header of the table on the "user by points" page.
 *
 * @return
 *   An header array used by theme_table() and TableSort.
 */
function theme_userpoints_list_users_header() {
  return array(
      array('data' => t('User'), 'field' => 'u.name'),
      array('data' => t('Category'), 'field' => 't.name'),
      array('data' => t('!Points', userpoints_translation()), 'field' => 'p.points', 'sort' => 'desc'),
  );
}

/*
 * Themes the output of a single row of the "user by points" page
 *
 * @return
 *   Array for a single row, suitable for inclusion with theme_table.
 */
function theme_userpoints_list_users_row($attributes) {
  global $user;

  $row = $attributes['row'];

  if (!$row->cat) {
    $row->cat = t('!Uncategorized', userpoints_translation());
  }

  $details = '';
  if ($user->uid == $row->uid) {
    $details = "&nbsp;&nbsp;" . l(t("(details)"), "myuserpoints");
  }

  return array(
      array('data' => theme('username', array('account' => $row)) . $details),
      array('data' => $row->cat, 'align' => 'right'),
      array('data' => $row->points, 'align' => 'right'),
  );
}

/**
 * Theme implementation to display a userpoints category.
 */
function theme_userpoints_view_category($variables) {
  $element = $variables['element'];
  $element += array(
    '#attributes' => array(),
  );

  $output = '';
  if (!empty($element['#title'])) {
    $output .= '<h3 ' . drupal_attributes(($element['#attributes'])) . '>' . $element['#title'] . '</h3>';
  }

  $output .= '<dl ' . drupal_attributes(($element['#attributes'])) . '>';
  $output .= drupal_render_children($element);
  $output .= '</dl>';

  return $output;
}

/**
 * Theme implementation to display a userpoints item.
 */
function theme_userpoints_view_item($variables) {
  $element = $variables['element'];
  $element += array(
    '#attributes' => array(),
  );

  $output = '<dt ' . drupal_attributes(($element['#attributes'])) . '>' . $element['#title'] . '</dt>';
  $output .= '<dd ' . drupal_attributes(($element['#attributes'])) . '>' . $element['#value'] . '</dd>';

  return $output;
}
