<?php


/**
 * Page callback to build up a full report.
 */
function hacked_reports_hacked() {
  // We're going to be borrowing heavily from the update module
  module_load_include('inc', 'update', 'update.report');
  if ($available = update_get_available(TRUE)) {
    module_load_include('inc', 'update', 'update.compare');
    $data = update_calculate_project_data($available);
    return theme('hacked_report', hacked_calculate_project_data($data));
  }
  else {
    return theme('update_report', _update_no_data());
  }
}

/**
 * Page callback to rebuild the hacked report.
 */
function hacked_reports_rebuild() {
  // We're going to be borrowing heavily from the update module
  module_load_include('inc', 'update', 'update.report');
  if ($available = update_get_available(TRUE)) {
    module_load_include('inc', 'update', 'update.compare');
    $data = update_calculate_project_data($available);
    hacked_calculate_project_data($data, TRUE, 'admin/reports/hacked');
  }
  drupal_goto('admin/reports/hacked');
}

/**
 * Batch callback to build the hacked report.
 */
function hacked_build_report_batch($project_name, &$context) {
  if (!isset($context['results']['report'])) {
    $context['results']['report'] = array();
  }
  module_load_include('inc', 'hacked', 'includes/hacked_project');
  $project = new hackedProject($project_name);
  $context['results']['report'][$project_name] = $project->compute_report();
  $context['message'] = t('Finished processing: @name', array('@name' => $project->title()));
}

/**
 * Completion callback for the report batch.
 */
function hacked_build_report_batch_finished($success, $results, $operations) {
  if ($success) {
    // Sort the results.
    usort($results['report'], '_hacked_project_report_sort_by_status');
    // Store them.
    cache_set('hacked:full-report', $results['report'], HACKED_CACHE_TABLE, strtotime('+1 day'));
    variable_set('hacked_last_report', time());
  }
}
