<?php

/**
 * @file
 * Mailchimp hook definitions.
 */

/**
 * Alter mergevars before they are sent to Mailchimp.
 *
 * @param array $mergevars
 *   Array of Mailchimp mergevars.
 * @param object $entity
 *   The entity used as a source for mergevar values.
 * @param string $entity_type
 *   The type of entity used as a source for mergevar values.
 * @param string $list_id
 *   The ID of the Mailchimp list containing the mergevars.
 */
function hook_mailchimp_lists_mergevars_alter(&$mergevars, $entity, $entity_type, $list_id) {
}

/**
 * Perform an action during the firing of a Mailchimp webhook.
 *
 * Refer to http://apidocs.mailchimp.com/webhooks for more details.
 *
 * @string $type
 *   The type of webhook firing.
 * @array $data
 *   The data contained in the webhook.
 */
function hook_mailchimp_process_webhook($type, $data) {

}

/**
 * Perform an action after a subscriber has been subscribed.
 *
 * @string $list_id
 *   Mailchimp list id.
 * @string $email
 *   Subscriber email address.
 * @array $merge_vars
 *   Submitted user values.
 */
function hook_mailchimp_subscribe_user($list_id, $email, $merge_vars) {

}

/**
 * Perform an action after a subscriber has been unsubscribed.
 *
 * @string $list_id
 *   Mailchimp list id.
 * @string $email
 *   Subscriber email address.
 */
function hook_mailchimp_unsubscribe_user($list_id, $email) {

}

/**
 * Alter the key for a given api request.
 *
 * @string &$api_key
 *   The Mailchimp API key.
 * @array $context
 *   The Mailchimp API classname of the API object.
 */
function hook_mailchimp_api_key_alter(&$api_key, $context) {

}

/**
 * Alter the entity options list on the automations entity form.
 *
 * @param array $entity_type_options
 *   The full list of Drupal entities.
 * @param string $automation_entity_label
 *   The label for the automation entity, if it exists.
 */
function hook_mailchimp_automations_entity_options(&$entity_type_options, $automation_entity_label) {

}

/**
 * Alter mergevars before a workflow automation is triggered.
 *
 * @param array $merge_vars
 *   The merge vars that will be passed to Mailchimp.
 * @param object $automation_entity
 *   The MailchimpAutomationEntity object.
 * @param object $wrapped_entity
 *   The EntityMetadataWrapper for the triggering entity.
 */
function hook_mailchimp_automations_mergevars_alter(&$merge_vars, $automation_entity, $wrapped_entity) {

}

/**
 * Perform an action after a successful Mailchimp workflow automation.
 *
 * @param object $automation_entity
 *   The MailchimpAutomationEntity object.
 * @param string $email
 *   The email_property value from the MailchimpAutomationEntity.
 * @param object $wrapped_entity
 *   The EntityMetadataWrapper for the triggering entity.
 */
function hook_mailchimp_automations_workflow_email_triggered($automation_entity, $email, $wrapped_entity) {

}
