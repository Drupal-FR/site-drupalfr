<?php


function hacked_reports_hacked_diff($project, $file) {

  if (!module_exists('diff')) {
    return t('The diff module is required to use this feature.');
  }

  $project->identify_project();

  // Find a better way to do this:
  $breadcrumb = array(
    l('Home', '<front>'),
    l('Administer', 'admin'),
    l('Reports', 'admin/reports'),
    l('Hacked', 'admin/reports/hacked'),
    l($project->title(), 'admin/reports/hacked/' . $project->name),
  );
  drupal_set_breadcrumb($breadcrumb);

  if ($project->file_is_diffable($file)) {
    return hacked_diff_changed($project, $file);
  }
  return t('Cannot hash binary file or file not found: %file', array('%file' => $file));
}

function hacked_diff_changed($project, $file) {
  // Load up the two files and diff them:
  module_load_include('php', 'diff', 'DiffEngine');
  $formatter = new DrupalDiffFormatter();

  $original_file = $project->file_get_location('remote', $file);
  $installed_file = $project->file_get_location('local', $file);

  $original_array = file_exists($original_file) ? file($original_file) : array();
  $installed_array = file_exists($installed_file) ? file($installed_file) : array();

  $diff = new Diff($original_array, $installed_array);
  $output = theme('diff_table', array(t('Orignal'), '', t('Current'), ''), $formatter->format($diff), array('class' => 'diff hacked-diff'));
  return $output;

}
