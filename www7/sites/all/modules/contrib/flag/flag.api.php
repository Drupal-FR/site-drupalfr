<?php

/**
 * @file
 * Hooks provided by the Flag module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define one or more flag types.
 *
 * @return
 *  An array whose keys are flag type names and whose values are properties of
 *  the flag type.
 *  Flag type names must match the content type (in the rest of Drupal: entity
 *  type) a flag type works with.
 *  Properties for flag types are as follows:
 *  - 'title': The main label of the flag type.
 *  - 'description': A longer description shown in the UI when creating a new
 *    flag.
 *  - 'handler': The name of the class implementing this flag type.
 *
 * @see flag_fetch_definition()
 */
function hook_flag_definitions() {
  return array(
    'node' => array(
      'title' => t('Nodes'),
      'description' => t("Nodes are a Drupal site's primary content."),
      'handler' => 'flag_node',
    ),
  );
}

/**
 * Alter flag type definitions provided by other modules.
 *
 * @param $definitions
 *  An array of flag definitions returned by hook_flag_definitions().
 */
function hook_flag_definitions_alter(&$definitions) {

}

/**
 * Define one or more flag link types.
 *
 * @see flag_get_link_types()
 * @see hook_flag_link_types_alter()
 *
 * @return
 *  An array of one or more types, keyed by the machine name of the type, and
 *  where each value is a link type definition as an array with the following
 *  properties:
 *  - 'title': The human-readable name of the type.
 *  - 'description': The description of the link type.
 *  - 'options': An array of extra options for the link type.
 *  - 'uses standard js': Boolean, indicates whether the link requires Flag
 *    module's own JS file for links.
 *  - 'uses standard css': Boolean, indicates whether the link requires Flag
 *    module's own CSS file for links.
 */
function hook_flag_link_types() {

}

/**
 * Alter other modules' definitions of flag link types.
 *
 * @see flag_get_link_types()
 * @see hook_flag_link_types()
 */
function hook_flag_link_types_alter(&$link_types) {

}

/**
 * Define default flags.
 */
function hook_flag_default_flags() {

}

/**
 * Allow modules to alter a flag when it is initially loaded.
 *
 * @see flag_get_flags().
 */
function hook_flag_alter(&$flag) {

}

/**
 * Alter a flag's default options.
 *
 * @param $options
 *  The array of default options for the flag type, with the options for the
 *  flag's link type merged in.
 * @param $flag
 *  The flag object.
 *
 * @see flag_flag::options()
 */
function hook_flag_options_alter(&$options, $flag) {

}

/**
 * Act on a flagging.
 *
 * @param $op
 *  The operation being performed: one of 'flag' or 'unflag'.
 * @param $flag
 *  The flag object.
 * @param $content_id
 *  The id of the content (aka entity) the flag is on.
 * @param $account
 *  The user account performing the action.
 * @param $fcid
 *  The id of the flagging in the {flag_content} table.
 */
function hook_flag($op, $flag, $content_id, $account, $fcid) {

}

/**
 * Allow modules to allow or deny access to flagging.
 *
 * @param $flag
 *  The flag object.
 * @param $content_id
 *  The id of the entity in question.
 * @param $action
 *  The action to test. Either 'flag' or 'unflag'.
 * @param $account
 *  The user on whose behalf to test the flagging action.
 *
 * @see flag_flag:access()
 */
function hook_flag_access($flag, $content_id, $action, $account) {

}

/**
 * TODO
 */
function hook_flag_access_multiple() {

}

/**
 *
 */
function hook_flag_link() {

}

/**
 * TODO
 */
function flag_delete () {

}

/**
 * TODO
 */
function hook_flag_reset() {

}

