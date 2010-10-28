<?php

/**
 * Implementation of hook_content_default_fields().
 */
function drupalfr_workflow_content_default_fields() {
  module_load_include('inc', 'drupalfr_workflow', 'drupalfr_workflow.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalfr_workflow_content_default_fields', $args);
}

/**
 * Implementation of hook_rules_defaults().
 */
function drupalfr_workflow_rules_defaults() {
  module_load_include('inc', 'drupalfr_workflow', 'drupalfr_workflow.defaults');
  $args = func_get_args();
  return call_user_func_array('_drupalfr_workflow_rules_defaults', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function drupalfr_workflow_views_default_views() {
  module_load_include('inc', 'drupalfr_workflow', 'drupalfr_workflow.features.views');
  $args = func_get_args();
  return call_user_func_array('_drupalfr_workflow_views_default_views', $args);
}
