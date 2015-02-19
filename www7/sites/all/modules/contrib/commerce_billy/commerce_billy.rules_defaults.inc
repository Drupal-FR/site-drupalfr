<?php
/**
 * @file
 * commerce_billy.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_billy_default_rules_configuration() {
  $items = array();
  $items['rules_commerce_billy_generate_invoice_number_on_order_insert'] = entity_import('rules_config', '{ "rules_commerce_billy_generate_invoice_number_on_order_insert" : {
      "LABEL" : "Generate invoice number on order insert",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_billy" ],
      "REQUIRES" : [ "rules", "entity" ],
      "ON" : [ "commerce_order_insert" ],
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "invoiced" } }
      ],
      "DO" : [
        { "component_rules_commerce_billy_set_to_invoiced" : { "order" : [ "commerce-order" ] } }
      ]
    }
  }');
  $items['rules_commerce_billy_generate_invoice_number_on_order_update'] = entity_import('rules_config', '{ "rules_commerce_billy_generate_invoice_number_on_order_update" : {
      "LABEL" : "Generate invoice number on order update",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_billy" ],
      "REQUIRES" : [ "rules", "entity" ],
      "ON" : [ "commerce_order_update" ],
      "IF" : [
        { "NOT data_is" : { "data" : [ "commerce-order-unchanged:status" ], "value" : "invoiced" } },
        { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "invoiced" } }
      ],
      "DO" : [
        { "component_rules_commerce_billy_set_to_invoiced" : { "order" : [ "commerce-order" ] } }
      ]
    }
  }');
  $items['rules_commerce_billy_invoice_order_on_completion'] = entity_import('rules_config', '{ "rules_commerce_billy_invoice_order_on_completion" : {
      "LABEL" : "Invoice order on completion",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_billy" ],
      "REQUIRES" : [ "rules", "commerce_order", "entity" ],
      "ON" : [ "commerce_order_update" ],
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "completed" } },
        { "NOT data_is" : { "data" : [ "commerce-order-unchanged:status" ], "value" : "completed" } }
      ],
      "DO" : [
        { "commerce_order_update_status" : { "commerce_order" : [ "commerce_order" ], "order_status" : "invoiced" } }
      ]
    }
  }');
  // Only activate this rule if auto-billing is set.
  $items['rules_commerce_billy_invoice_order_on_completion']->active = variable_get('commerce_billy_auto_invoice', FALSE);

  $items['rules_commerce_billy_set_cancel_date'] = entity_import('rules_config', '{ "rules_commerce_billy_set_cancel_date" : {
      "LABEL" : "Set cancel date",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_billy" ],
      "REQUIRES" : [ "rules", "entity" ],
      "ON" : [ "commerce_order_update" ],
      "IF" : [
        { "NOT data_is" : { "data" : [ "commerce-order-unchanged:status" ], "value" : "canceled" } },
        { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "canceled" } },
        { "entity_has_field" : {
            "entity" : [ "commerce-order" ],
            "field" : "field_commerce_billy_cancel_date"
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "commerce-order:field-commerce-billy-cancel-date" ],
            "value" : "now"
          }
        }
      ]
    }
  }');
  $items['rules_commerce_billy_set_to_canceled'] = entity_import('rules_config', '{ "rules_commerce_billy_set_to_canceled" : {
      "LABEL" : "Set order to canceled",
      "PLUGIN" : "rule",
      "TAGS" : [ "commerce_billy" ],
      "REQUIRES" : [ "rules", "commerce_order" ],
      "USES VARIABLES" : { "order" : { "label" : "Order", "type" : "commerce_order" } },
      "DO" : [
        { "commerce_order_update_state" : { "commerce_order" : [ "order" ], "order_state" : "canceled" } }
      ]
    }
  }');
  $items['rules_commerce_billy_set_to_invoiced'] = entity_import('rules_config', '{ "rules_commerce_billy_set_to_invoiced" : {
      "LABEL" : "Set order to invoiced",
      "PLUGIN" : "rule",
      "TAGS" : [ "commerce_billy" ],
      "REQUIRES" : [ "rules", "commerce_order", "commerce_billy" ],
      "USES VARIABLES" : { "order" : { "label" : "Order", "type" : "commerce_order" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "order" ], "field" : "field_commerce_billy_i_date" } },
        { "data_is" : { "data" : [ "order:order-number" ], "value" : [ "order:order-id" ] } }
      ],
      "DO" : [
        { "commerce_order_update_state" : { "commerce_order" : [ "order" ], "order_state" : "invoiced" } },
        { "commerce_billy_invoice_nr" : { "order" : [ "order" ] } },
        { "data_set" : { "data" : [ "order:field-commerce-billy-i-date" ], "value" : "now" } }
      ]
    }
  }');
  return $items;
}
