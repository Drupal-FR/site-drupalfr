<?php
/**
 * @file
 * drupalfr_edito.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function drupalfr_edito_default_rules_configuration() {
  $items = array();
  $items['rules_comment_is_created'] = entity_import('rules_config', '{ "rules_comment_is_created" : {
      "LABEL" : "Comment is created",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "userpoints_rules", "comment" ],
      "ON" : { "comment_insert" : [] },
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "comment:author" ],
            "points" : "1",
            "tid" : "0",
            "entity" : [ "comment" ],
            "operation" : "Insert",
            "display" : 0,
            "moderate" : "approved"
          }
        }
      ]
    }
  }');
  $items['rules_comment_is_flagged'] = entity_import('rules_config', '{ "rules_comment_is_flagged" : {
      "LABEL" : "Comment is flagged",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "userpoints_rules", "flag" ],
      "ON" : { "flag_flagged_accepted_answer" : [] },
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "user", "id" : [ "flagged-comment:author:uid" ] },
            "PROVIDE" : { "entity_fetched" : { "comment_author" : "Comment\\u0027s author" } }
          }
        },
        { "userpoints_action_grant_points" : {
            "user" : [ "flagged-comment:author" ],
            "points" : "10",
            "tid" : "0",
            "entity" : [ "flagged-comment" ],
            "operation" : "Insert",
            "display" : 0,
            "moderate" : "approved"
          }
        },
        { "entity_fetch" : {
            "USING" : { "type" : "node", "id" : [ "flagged-comment:node:nid" ] },
            "PROVIDE" : { "entity_fetched" : { "node" : "Node" } }
          }
        },
        { "data_set" : {
            "data" : [ "flagged-comment:node:title" ],
            "value" : "[R\\u00e9solu] [node:title]"
          }
        }
      ]
    }
  }');
  $items['rules_comment_is_published'] = entity_import('rules_config', '{ "rules_comment_is_published" : {
      "LABEL" : "Comment is published",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "userpoints_rules", "comment" ],
      "ON" : { "comment_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "comment-unchanged:status" ], "value" : "0" } },
        { "data_is" : { "data" : [ "comment:status" ], "value" : "1" } }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "comment:author" ],
            "points" : "1",
            "tid" : "0",
            "entity" : [ "comment" ],
            "operation" : "Insert",
            "display" : 0,
            "moderate" : "approved"
          }
        }
      ]
    }
  }');
  $items['rules_comment_is_unflagged'] = entity_import('rules_config', '{ "rules_comment_is_unflagged" : {
      "LABEL" : "Comment is unflagged",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "userpoints_rules", "flag" ],
      "ON" : { "flag_unflagged_accepted_answer" : [] },
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "user", "id" : [ "flagged-comment:author:uid" ] },
            "PROVIDE" : { "entity_fetched" : { "comment_author" : "Comment\\u0027s author" } }
          }
        },
        { "userpoints_action_grant_points" : {
            "user" : [ "flagged-comment:author" ],
            "points" : "-10",
            "tid" : "0",
            "entity" : [ "flagged-comment" ],
            "operation" : "Remove",
            "display" : 0,
            "moderate" : "approved"
          }
        }
      ]
    }
  }');
  $items['rules_comment_is_unpublished'] = entity_import('rules_config', '{ "rules_comment_is_unpublished" : {
      "LABEL" : "Comment is unpublished",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "userpoints_rules", "comment" ],
      "ON" : { "comment_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "comment:status" ], "value" : "0" } },
        { "data_is" : { "data" : [ "comment-unchanged:status" ], "value" : "1" } }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "comment:author" ],
            "points" : "-1",
            "tid" : "0",
            "entity" : [ "comment" ],
            "operation" : "Remove",
            "display" : 0,
            "moderate" : "approved"
          }
        }
      ]
    }
  }');
  $items['rules_forum_post_is_published'] = entity_import('rules_config', '{ "rules_forum_post_is_published" : {
      "LABEL" : "Forum post is published",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "userpoints_rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "doc" : "doc", "forum" : "forum" } }
          }
        }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "node:author" ],
            "points" : "1",
            "tid" : "0",
            "entity" : [ "node" ],
            "operation" : "Insert",
            "display" : 0,
            "moderate" : "approved"
          }
        }
      ]
    }
  }');
  return $items;
}
