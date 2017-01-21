<?php

/**
 * @file
 * Mailchimp Automations entity class.
 */

class MailchimpAutomationsEntity extends Entity {
  public
    $mailchimp_automations_entity_id,
    $name,
    $label,
    $entity_type,
    $bundle,
    $email_property,
    $list_id,
    $workflow_id,
    $workflow_email_id;

  /**
   * Basic __construct implementation.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'mailchimp_automations_entity');
  }

  /**
   * Overrides Entity\Entity::uri().
   */
  public function uri() {
    return array(
      'path' => 'admin/config/services/mailchimp/automations/manage/' . $this->name,
      'options' => array(
        'entity_type' => $this->entityType,
        'entity' => $this,
      ),
    );
  }

}
