<?php


/**
 * Implements hook_rules_action_info().
 */
function commerce_billy_rules_action_info() {
  return array(
    'commerce_billy_invoice_nr' => array(
      'label' => t('Set invoice number on order'),
      'parameter' => array(
        'order' => array('type' => 'commerce_order', 'label' => t('Order')),
      ),
      'group' => t('Commerce Billy'),
    ),
  );
}
