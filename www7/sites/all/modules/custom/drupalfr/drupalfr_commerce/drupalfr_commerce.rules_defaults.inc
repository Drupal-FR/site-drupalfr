<?php
/**
 * @file
 * drupalfr_commerce.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function drupalfr_commerce_default_rules_configuration() {
  $items = array();
  $items['rules_display_add_to_cart_message'] = entity_import('rules_config', '{ "rules_display_add_to_cart_message" : {
      "LABEL" : "Display add to cart message",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "commerce_order", "commerce_cart" ],
      "USES VARIABLES" : {
        "order" : { "label" : "Order", "type" : "commerce_order" },
        "added_product" : { "label" : "Added product", "type" : "commerce_product" }
      },
      "IF" : [
        { "NOT commerce_order_contains_product_type" : {
            "commerce_order" : [ "order" ],
            "product_type" : { "value" : { "individual_membership" : "individual_membership" } },
            "operator" : "\\u003E=",
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "commerce_cart_add_to_cart_message" : { "commerce_product" : [ "added-product" ] } }
      ]
    }
  }');
  $items['rules_membership_expiration_reminder'] = entity_import('rules_config', '{ "rules_membership_expiration_reminder" : {
      "LABEL" : "membership expiration reminder",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "member" : { "label" : "member", "type" : "user" } },
      "ACTION SET" : [
        { "mail" : {
            "to" : "[member:mail]",
            "subject" : "Votre adh\\u00e9sion \\u00e0 l\\u0027association Drupalfr expirera dans 1 mois",
            "message" : "Bonjour [member:name],\\r\\n\\r\\nVotre adh\\u00e9sion \\u00e0 l\\u0027association Drupal France et francophonie expirera dans 1 mois.\\r\\n\\r\\nN\\u0027oubliez pas de r\\u00e9adh\\u00e9rer si vous le souhaitez.\\r\\n\\r\\nCordialement,\\r\\nl\\u0027association Drupal France et francophonie",
            "language" : [ "member:language" ]
          }
        }
      ]
    }
  }');
  $items['rules_one_membership_per_order'] = entity_import('rules_config', '{ "rules_one_membership_per_order" : {
      "LABEL" : "One membership per order",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "commerce_order", "commerce_cart", "rules" ],
      "USES VARIABLES" : { "order" : { "label" : "Order", "type" : "commerce_order" } },
      "IF" : [
        { "commerce_order_contains_product_type" : {
            "commerce_order" : [ "order" ],
            "product_type" : { "value" : { "individual_membership" : "individual_membership" } },
            "operator" : "\\u003E=",
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "commerce_cart_empty" : { "commerce_order" : [ "order" ] } },
        { "drupal_message" : {
            "message" : "Vous ne pouvez avoir qu\\u0027une seule adh\\u00e9sion dans votre panier.",
            "type" : "warning",
            "repeat" : 0
          }
        }
      ]
    }
  }');
  $items['rules_plan_membership_reminder'] = entity_import('rules_config', '{ "rules_plan_membership_reminder" : {
      "LABEL" : "Plan membership reminder",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [
        "commerce_order",
        "rules",
        "rules_scheduler",
        "commerce_checkout",
        "commerce_payment"
      ],
      "ON" : {
        "commerce_checkout_complete" : [],
        "commerce_payment_order_paid_in_full" : []
      },
      "IF" : [
        { "commerce_order_contains_product" : {
            "commerce_order" : [ "commerce_order" ],
            "product_id" : "individual_membership",
            "operator" : "\\u003E=",
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "schedule" : {
            "component" : "rules_membership_expiration_reminder",
            "date" : "+11 month",
            "identifier" : "Membership reminder for [commerce-order:owner]",
            "param_member" : [ "commerce-order:owner" ]
          }
        }
      ]
    }
  }');
  $items['rules_redirect_futur_member_to_cart'] = entity_import('rules_config', '{ "rules_redirect_futur_member_to_cart" : {
      "LABEL" : "Redirect futur member to cart",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "commerce_cart" ],
      "ON" : { "commerce_cart_product_add" : [] },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "commerce-line-item" ],
            "type" : "commerce_line_item",
            "bundle" : { "value" : { "individual_membership" : "individual_membership" } }
          }
        }
      ],
      "DO" : [ { "redirect" : { "url" : "cart" } } ]
    }
  }');
  $items['rules_set_membership_number'] = entity_import('rules_config', '{ "rules_set_membership_number" : {
      "LABEL" : "Set membership number",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "drupalfr_user" ],
      "USES VARIABLES" : { "user" : { "label" : "user", "type" : "user" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "user" ], "field" : "field_membership_number" } },
        { "data_is_empty" : { "data" : [ "user:field-membership-number" ] } }
      ],
      "DO" : [ { "drupalfr_user_assign_membership_number" : { "user" : [ "user" ] } } ]
    }
  }');
  $items['rules_set_membership_number_rule'] = entity_import('rules_config', '{ "rules_set_membership_number_rule" : {
      "LABEL" : "Set membership number",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "commerce_order", "rules", "commerce_checkout", "commerce_payment" ],
      "ON" : {
        "commerce_checkout_complete" : [],
        "commerce_payment_order_paid_in_full" : []
      },
      "IF" : [
        { "commerce_order_contains_product" : {
            "commerce_order" : [ "commerce_order" ],
            "product_id" : "individual_membership",
            "operator" : "\\u003E=",
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "component_rules_set_membership_number" : { "user" : [ "commerce-order:owner" ] } }
      ]
    }
  }');
  $items['rules_update_order_status_after_payement'] = entity_import('rules_config', '{ "rules_update_order_status_after_payement" : {
      "LABEL" : "Update order status after payement",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "-5",
      "OWNER" : "rules",
      "REQUIRES" : [ "commerce_order", "commerce_payment" ],
      "ON" : { "commerce_payment_order_paid_in_full" : [] },
      "DO" : [
        { "commerce_order_update_status" : { "commerce_order" : [ "commerce_order" ], "order_status" : "completed" } }
      ]
    }
  }');
  return $items;
}
