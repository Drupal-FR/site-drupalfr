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
        "EMAIL" : {"tag":"EMAIL","name":"Email Address","type":"email","required":true,"default_value":"","public":true,"display_order":1,"options":{"size":25}},
        "FNAME" : {"merge_id":1,"tag":"FNAME","name":"Pr\\u00e9nom","type":"text","required":false,"default_value":"","public":true,"display_order":2,"options":{"size":25},"help_text":"","list_id":"27e99cb1af","_links":[{"rel":"self","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/1","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json"},{"rel":"parent","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/CollectionResponse.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/CollectionLinks\\/Lists\\/MergeFields.json"},{"rel":"update","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/1","method":"PATCH","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/PATCH.json"},{"rel":"delete","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/1","method":"DELETE"}]},
        "LNAME" : {"merge_id":2,"tag":"LNAME","name":"Nom","type":"text","required":false,"default_value":"","public":true,"display_order":3,"options":{"size":25},"help_text":"","list_id":"27e99cb1af","_links":[{"rel":"self","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/2","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json"},{"rel":"parent","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/CollectionResponse.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/CollectionLinks\\/Lists\\/MergeFields.json"},{"rel":"update","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/2","method":"PATCH","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/PATCH.json"},{"rel":"delete","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/2","method":"DELETE"}]},
        "MMERGE3" : {"merge_id":3,"tag":"MMERGE3","name":"Contacted (private)","type":"text","required":false,"default_value":"","public":false,"display_order":4,"options":{"size":25},"help_text":"","list_id":"27e99cb1af","_links":[{"rel":"self","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/3","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json"},{"rel":"parent","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/CollectionResponse.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/CollectionLinks\\/Lists\\/MergeFields.json"},{"rel":"update","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/3","method":"PATCH","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/PATCH.json"},{"rel":"delete","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/3","method":"DELETE"}]},
        "MMERGE4" : {"merge_id":4,"tag":"MMERGE4","name":"ToSkip (private)","type":"text","required":false,"default_value":"","public":false,"display_order":5,"options":{"size":25},"help_text":"","list_id":"27e99cb1af","_links":[{"rel":"self","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/4","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json"},{"rel":"parent","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields","method":"GET","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/CollectionResponse.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/CollectionLinks\\/Lists\\/MergeFields.json"},{"rel":"update","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/4","method":"PATCH","targetSchema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/Response.json","schema":"https:\\/\\/us8.api.mailchimp.com\\/schema\\/3.0\\/Definitions\\/Lists\\/MergeFields\\/PATCH.json"},{"rel":"delete","href":"https:\\/\\/us8.api.mailchimp.com\\/3.0\\/lists\\/27e99cb1af\\/merge-fields\\/4","method":"DELETE"}]}
      },
      "description" : "",
      "doublein" : 1,
      "include_interest_groups" : 0,
      "placeholder" : 0
    }
  }');
  return $items;
}
