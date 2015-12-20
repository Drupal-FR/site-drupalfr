<?php
/**
 * @file
 * drupalfr_user.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_user_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_default_mailchimp_signup().
 */
function drupalfr_user_default_mailchimp_signup() {
  $items = array();
  $items['newsletter'] = entity_import('mailchimp_signup', '{
    "name" : "newsletter",
    "mc_lists" : { "27e99cb1af" : "27e99cb1af" },
    "mode" : "1",
    "title" : "Newsletter",
    "settings" : {
      "path" : "",
      "submit_button" : "Soumettre",
      "confirmation_message" : "Vous avez \\u00e9t\\u00e9 enregistr\\u00e9 avec succ\\u00e8s.",
      "destination" : "",
      "mergefields" : {
        "EMAIL" : {
          "name" : "Adresse email",
          "req" : true,
          "field_type" : "email",
          "public" : true,
          "show" : true,
          "order" : "1",
          "default" : "",
          "helptext" : "",
          "size" : "25",
          "tag" : "EMAIL",
          "id" : 0
        },
        "FNAME" : {
          "name" : "Pr\\u00e9nom",
          "req" : false,
          "field_type" : "text",
          "public" : true,
          "show" : true,
          "order" : "2",
          "default" : "",
          "helptext" : "",
          "size" : "25",
          "tag" : "FNAME",
          "id" : 1
        },
        "LNAME" : {
          "name" : "Nom",
          "req" : false,
          "field_type" : "text",
          "public" : true,
          "show" : true,
          "order" : "3",
          "default" : "",
          "helptext" : "",
          "size" : "25",
          "tag" : "LNAME",
          "id" : 2
        },
        "MMERGE3" : 0,
        "MMERGE4" : 0
      },
      "description" : "",
      "doublein" : 1,
      "send_welcome" : 0,
      "include_interest_groups" : 0
    }
  }');
  return $items;
}
