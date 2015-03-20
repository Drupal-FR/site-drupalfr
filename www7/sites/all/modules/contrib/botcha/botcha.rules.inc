<?php

/**
 * @file
 * Rules integration for the BOTCHA module.
 */

/**
 * Implements hook_rules_event_info().
 */
function botcha_rules_event_info() {
  return array(
    'botcha_form_approved' => array(
      'label' => t('A form submission is accepted'),
      // @todo Abstract it.
      'group' => t('BOTCHA'),
      'variables' => botcha_rules_events_hook_variables(),
    ),
    'botcha_form_rejected' => array(
      'label' => t('A form submission is rejected'),
      // @todo Abstract it.
      'group' => t('BOTCHA'),
      'variables' => botcha_rules_events_hook_variables(),
    ),
  );
}

/**
 * Describes the arguments available for the botcha_rules_event_info().
 */
function botcha_rules_events_hook_variables() {
  return array(
//    'form' => array('type' => 'form', 'label' => t('Form')),
//    'form_state' => array('type' => 'form_state', 'label' => t('Form state')),
    'form_id' => array('type' => 'string', 'label' => t('Form ID')),
    'total_recipes' => array(
      'type' => 'number',
      'label' => t('Total recipes'),
    ),
    'passed_recipes' => array(
      'type' => 'number',
      'label' => t('Total passed recipes'),
    ),
    'passed_recipes_names' => array(
      'type' => 'string',
      'label' => t('Passed recipes names'),
    ),
    'last_recipe_name' => array(
      'type' => 'string',
      'label' => t('Last recipe name'),
    ),
    'fail' => array(
      'type' => 'string',
      'label' => t('Fail condition'),
    ),
    'failed_field' => array(
      'type' => 'string',
      'label' => t('Failed field (can be obscured)'),
    ),
    'failed_error' => array(
      'type' => 'string',
      'label' => t('Failed error text (can be obscured)'),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function botcha_rules_action_info() {
  return array(
    'botcha_rules_action_regenerate_seed' => array(
      'label' => t('Regenerate seed (invalidates all prepared forms that have not yet been submitted)'),
      'arguments' => array(
      ),
      'group' => t('BOTCHA'),
    ),
  );
}

/**
 * Action: Regenerate seed.
 */
function botcha_rules_action_regenerate_seed() {
  // Generate unique secret for this site
  $secret = md5(uniqid(mt_rand(), TRUE));
  variable_set('botcha_secret', $secret);
//  drupal_set_message(t('New BOTCHA secret key have been generated.'));
}
