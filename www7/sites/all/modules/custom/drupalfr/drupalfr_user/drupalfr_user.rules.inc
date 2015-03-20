<?php

/**
 * Implements hook_rules_action_info().
 */
function drupalfr_user_rules_action_info() {
  $items['drupalfr_user_assign_membership_number'] = array(
    'label' => t('Assign membership number'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
      ),
    ),
    'group' => t('Drupalfr user'),
  );
  return $items;
}

/**
 * Attribute a membership number to a user.
 */
function drupalfr_user_assign_membership_number($user) {
  $next_membership_number = variable_get('drupalfr_user_next_membership_number');

  // Attribute the next number.
  $user_wrapper = entity_metadata_wrapper('user', $user);
  $user_wrapper->field_membership_number->set($next_membership_number);
  $user_wrapper->save();

  // Calculate the next number.
  variable_set('drupalfr_user_next_membership_number', $next_membership_number + 1);

  return array('user' => $user);
}
