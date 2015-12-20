<?php
/**
 * @file
 * dfr_emploi.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function dfr_emploi_default_rules_configuration() {
  $items = array();
  $items['rules_dfr_emploi_moderation_notification'] = entity_import('rules_config', '{ "rules_dfr_emploi_moderation_notification" : {
      "LABEL" : "Emploi : notification de mod\\u00e9ration",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Emploi" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_update--offre" : { "bundle" : "offre" } },
      "IF" : [
        { "NOT data_is" : { "data" : [ "node:field-flag-moderation" ], "value" : "pending" } },
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "4" : "4", "14" : "14" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "component_rules_dfr_emploi_moderation_notification_with_reason" : { "node" : [ "node" ] } },
        { "component_rules_dfr_emploi_moderation_notification_without_reason" : { "node" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_dfr_emploi_moderation_notification_with_reason'] = entity_import('rules_config', '{ "rules_dfr_emploi_moderation_notification_with_reason" : {
      "LABEL" : "Emploi : notification de mod\\u00e9ration avec raison",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Emploi" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "node", "type" : "node" } },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "node" ],
            "type" : "node",
            "bundle" : { "value" : { "offre" : "offre" } }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "node:field-message-moderation" ] } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[node:author]",
            "subject" : "Votre annonce sur drupalfr.org a \\u00e9t\\u00e9 [node:field-flag-moderation]",
            "message" : "Bonjour,\\r\\n\\r\\nVotre offre d\\u0027emploi \\u0022[node:title]\\u0022 ([node:url]) a \\u00e9t\\u00e9 [node:field-flag-moderation] pour la raison suivante :\\r\\n\\r\\n[node:field-message-moderation]\\r\\n\\r\\nCordialement,\\r\\nL\\u0027\\u00e9quipe Drupalfr",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_dfr_emploi_moderation_notification_without_reason'] = entity_import('rules_config', '{ "rules_dfr_emploi_moderation_notification_without_reason" : {
      "LABEL" : "Emploi : notification de mod\\u00e9ration sans raison",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Emploi" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "node", "type" : "node" } },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "node" ],
            "type" : "node",
            "bundle" : { "value" : { "offre" : "offre" } }
          }
        },
        { "data_is_empty" : { "data" : [ "node:field-message-moderation" ] } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[node:author]",
            "subject" : "Votre annonce sur drupalfr.org a \\u00e9t\\u00e9 [node:field-flag-moderation]",
            "message" : "Bonjour,\\r\\n\\r\\nVotre offre d\\u0027emploi \\u0022[node:title]\\u0022 ([node:url]) a \\u00e9t\\u00e9 [node:field-flag-moderation].\\r\\n\\r\\nCordialement,\\r\\nL\\u0027\\u00e9quipe Drupalfr",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_job_offer_consistent_status'] = entity_import('rules_config', '{ "rules_job_offer_consistent_status" : {
      "LABEL" : "Job offer consistent status",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Emploi" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_presave--offre" : { "bundle" : "offre" } },
      "IF" : [
        { "data_is" : { "data" : [ "node:status" ], "value" : "1" } },
        { "NOT data_is" : { "data" : [ "node:field-flag-moderation" ], "value" : "approved" } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "node:field-flag-moderation" ], "value" : "approved" } }
      ]
    }
  }');
  $items['rules_new_job_offer'] = entity_import('rules_config', '{ "rules_new_job_offer" : {
      "LABEL" : "New job offer",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Emploi" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "offre" : "offre" } } } }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "14" : "14" } },
            "subject" : "Offre d\\u0027emploi en attente de mod\\u00e9ration",
            "message" : "Bonjour,\\r\\n\\r\\nUne nouvelle offre d\\u0027emploi vient d\\u0027\\u00eatre post\\u00e9e dans la section emploi, nous vous invitons \\u00e0 y jeter un oeil afin de la mod\\u00e9rer, tant que vous ne la publierez pas l\\u0027offre ne sera pas visible.\\r\\n\\r\\nPour voir l\\u0027offre, rendez-vous ici : [node:url]\\r\\nPour modifier l\\u0027offre, rendez-vous ici : [node:edit-url]\\r\\nPage d\\u0027administration des offres d\\u0027emploi : [site:url]\\/admin\\/content\\/emploi\\r\\n\\r\\nBonne journ\\u00e9e."
          }
        }
      ]
    }
  }');
  $items['rules_updated_job_offer'] = entity_import('rules_config', '{ "rules_updated_job_offer" : {
      "LABEL" : "Updated job offer",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_update--offre" : { "bundle" : "offre" } },
      "IF" : [
        { "NOT node_is_published" : { "node" : [ "node" ] } },
        { "data_is" : { "data" : [ "node:author" ], "value" : [ "site:current-user" ] } }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "14" : "14" } },
            "subject" : "Offre d\\u0027emploi mise \\u00e0 jour par son auteur.",
            "message" : "Bonjour,\\r\\n\\r\\nL\\u0027auteur d\\u0027une offre d\\u0027emploi en attente de validation ou refus\\u00e9e la mise \\u00e0 jour, nous vous invitons \\u00e0 y jeter un oeil afin de la mod\\u00e9rer, tant que vous ne la publierez pas l\\u0027offre ne sera pas visible.\\r\\n\\r\\nPour voir l\\u0027offre, rendez-vous ici : [node:url]\\r\\nPour modifier l\\u0027offre, rendez-vous ici : [node:edit-url]\\r\\nPage d\\u0027administration des offres d\\u0027emploi : [site:url]\\/admin\\/content\\/emploi\\r\\n\\r\\nBonne journ\\u00e9e."
          }
        },
        { "entity_create" : {
            "USING" : {
              "type" : "comment",
              "param_subject" : "L\\u0027auteur a mis \\u00e0 jour son offre d\\u0027emploi.",
              "param_node" : [ "node" ],
              "param_author" : [ "node:author" ],
              "param_comment_body" : { "value" : {
                  "value" : "L\\u0027auteur a mis \\u00e0 jour son offre d\\u0027emploi.",
                  "format" : "1"
                }
              }
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Entit\\u00e9 cr\\u00e9\\u00e9e" } }
          }
        }
      ]
    }
  }');
  return $items;
}
