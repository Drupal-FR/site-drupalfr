<?php


/**
 * @file
 *   Hacked drush command.
 *
 *   Enables drush support for the Hacked! module.
 */

/**
 * Implementation of hook_drush_help().
 */
function hacked_drush_help($section) {
  switch ($section) {
    case 'drush:hacked-list-projects':
      return dt('List projects and their hacked/unhacked status.');
    case 'drush:hacked-details':
      return dt('Show details of the files in one project, and the hacked/unhacked status of those files.');
    case 'drush:hacked-diff':
      return dt('Output a unified diff of the specified project.');

  }
}

/**
 * Implementation of hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function hacked_drush_command() {
  $items = array();

  $items['hacked-list-projects'] = array(
    'description' => "List all projects that can be analysed by Hacked! ",
    'drupal dependencies' => array('hacked'),
    'options' => array(
      '--force-rebuild' => 'Rebuild the Hacked! report instead of getting a cached version.'
    ),
    'aliases' => array('hlp'),
  );

  $items['hacked-lock-modified'] = array(
    'description' => "Lock all projects that Hacked! detects are modified, so that drush pm-updatecode will not touch them. (drush-4.x+ only)",
    'drupal dependencies' => array('hacked'),
  );

  $items['hacked-details'] = array(
    'description' => "Show the Hacked! report about a specific project.",
    'drupal dependencies' => array('hacked'),
    'arguments' => array(
      'project' => 'The machine name of the project to report on.',
    ),
    'options' => array(
      '--include-unchanged' => 'Show the files that are unchanged too.',
    ),
    'aliases' => array('hd'),
  );

  $items['hacked-diff'] = array(
    'description' => "Output a unified diff of the project specified.",
    'drupal dependencies' => array('hacked'),
    'arguments' => array(
      'project' => 'The machine name of the project to report on.',
    ),
    'options' => array(
      '--diff-options' => 'Command line options to pass through to the diff command.'
    ),
  );

  return $items;
}

/**
 * Compute the report data for hacked.
 *
 * WARNING: This function can invoke a batch process and end your current page.
 * So you'll want to be very careful if you call this!
 */
function hacked_calculate_project_data_drush($projects, $force = FALSE, $redirect = NULL) {
  include_once './includes/batch.inc';

  // Try to get the report form cache if we can.
  $cache = cache_get('hacked:drush:full-report', HACKED_CACHE_TABLE);
  if (!empty($cache->data) && !$force) {
    return $cache->data;
  }

  // Enter a batch to build the report.
  $operations = array();
  foreach ($projects as $project) {
    $operations[] = array(
      'hacked_build_report_batch',
      array($project['short_name']),
    );
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'hacked_build_report_batch_finished_drush',
    'file' => drupal_get_path('module', 'hacked') . '/hacked.report.inc',
    'title' => t('Building report'),
  );

  drush_print('Rebuilding Hacked! report');
  batch_set($batch);
  $batch =& batch_get();
  $batch['progressive'] = FALSE;
  drush_backend_batch_process();
  drush_print('Done.');

  // Now we can get the data from the cache.
  $cache = cache_get('hacked:drush:full-report', HACKED_CACHE_TABLE);
  if (!empty($cache->data)) {
    return $cache->data;
  }

  return array();
}

/**
 * Completion callback for the report batch.
 */
function hacked_build_report_batch_finished_drush($success, $results, $operations) {
  if ($success) {
    // Sort the results.
    usort($results['report'], '_hacked_project_report_sort_by_status');
    // Store them.
    cache_set('hacked:drush:full-report', $results['report'], HACKED_CACHE_TABLE, strtotime('+1 day'));
  }
}

/**
 * Drush command callback that shows the listing of changed/unchanged projects.
 */
function drush_hacked_list_projects() {

  // Go get the data:
  module_load_include('inc', 'update', 'update.report');
  if ($available = update_get_available(TRUE)) {
    module_load_include('inc', 'update', 'update.compare');
    $data = update_calculate_project_data($available);
    $force_rebuild = drush_get_option('force-rebuild', FALSE);
    $projects = hacked_calculate_project_data_drush($data, $force_rebuild);
    // Now print the data using drush:
    $rows[] = array(
      dt('Title'),
      dt('Name'),
      dt('Version'),
      dt('Status'),
      dt('Changed'),
      dt('Deleted'),
    );
    foreach ($projects as $project) {
      $row = array(
        $project['title'],
        $project['name'],
        $project['existing_version']
      );

      // Now add the status:
      switch ($project['status']) {
        case HACKED_STATUS_UNHACKED:
          $row[] = dt('Unchanged');
          break;
        case HACKED_STATUS_HACKED:
          $row[] = t('Changed');
          break;
        case HACKED_STATUS_UNCHECKED:
        default:
          $row[] = t('Unchecked');
          break;
      }

      $row[] = $project['counts']['different'];
      $row[] = $project['counts']['missing'];


      $rows[] = $row;
    }
    drush_print_table($rows, TRUE);

    return $projects;
  }
}

/**
 * Lock all of the modified files so that pm-updatecode will not
 * touch them.
 */
function drush_hacked_lock_modified() {
  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  module_load_include('inc', 'update', 'update.report');
  if (isset($drupal_root) && ($available = update_get_available(TRUE))) {
    module_load_include('inc', 'update', 'update.compare');
    $data = update_calculate_project_data($available);
    $projects = hacked_calculate_project_data_drush($data, TRUE);

    foreach ($projects as $project) {
      $row = array(
        empty($project['title']) ? $project['name'] : $project['title'],
        $project['name'],
        $project['existing_version']
      );

      // Lock the file if it is not already locked.
      switch ($project['status']) {
        case HACKED_STATUS_HACKED:
          $project_ob = NULL;
          $project_ob = hacked_project_load($project['project_name']);
          $lockfile = $project_ob->file_get_location('local', '.drush-lock-update');
          if (!file_exists($lockfile)) {
            drush_op('file_put_contents', $lockfile, dt("Locked: modified."));
            drush_print(dt("Locked: @project", array('@project' => $project['name'])));
          }
          else {
            drush_print(dt("@project is modified and already locked", array('@project' => $project['name'])));
          }
          break;
        case HACKED_STATUS_UNHACKED:
        case HACKED_STATUS_UNCHECKED:
        default:
          break;
      }
    }
  }
}

/**
 * Add a --lock-modified flag to pm-updatecode
 */
function drush_hacked_pre_pm_updatecode() {
  if (drush_get_option('lock-modified')) {
    drush_print(dt('Hacked! is checking for modified projects...'));
    drush_hacked_lock_modified();
    drush_print(dt('Hacked! modification check complete.'));
  }
}

/**
 * Add --lock-modified to the pm-updatecode and pm-update help
 */
function hacked_drush_help_alter(&$command) {
  if (($command['command'] == 'pm-updatecode') || ($command['command'] == 'pm-update')) {
    $command['sub-options']['--lock']['--lock-modified'] = "Lock any project that Hacked! determines is modified.";
  }
}

/**
 * Validate hook for the hacked_details drush command.
 */
function drush_hacked_details_validate($short_name = '') {
  return drush_hacked_drush_command_validate($short_name);
}

/**
 * Validate hook for the hacked drush commands that need a project.
 */
function drush_hacked_drush_command_validate($short_name = '') {
  if (empty($short_name)) {
    return drush_set_error('HACKED_PROJECT_NOT_FOUND', dt('A valid project must be specified', array('@project' => $short_name)));
  }

  $project = hacked_project_load($short_name);
  $project->identify_project();
  if (!$project->project_identified) {
    return drush_set_error('HACKED_PROJECT_NOT_FOUND', dt('Could not find project: @project', array('@project' => $short_name)));
  }
  $project = NULL;
}

/**
 * Drush command callback that shows the list of changes/unchanged files in a project.
 *
 * You may specify the --include-unchanged option to show unchanged files too,
 * otherwise just the changed and deleted files are shown.
 */
function drush_hacked_details($short_name) {
  $project = hacked_project_load($short_name);
  $report = $project->compute_details();

  drush_print(dt('Details for project: @name', array('@name' => $project->title())));
  drush_print(dt('Total files: @total_files, files changed: @changed_files, deleted files: @deleted_files', array(
    '@total_files' => count($report['files']),
    '@changed_files' => $report['counts']['different'],
    '@deleted_files' => $report['counts']['missing'],
  )));
  drush_print('');

  drush_print(dt('Detailed results:'));
  // Sort the results:
  arsort($report['files']);

  $rows[] = array(
    dt('Status'),
    dt('File'),
  );
  $show_unchanged = drush_get_option('include-unchanged', FALSE);
  foreach ($report['files'] as $file => $status) {
    if (!$show_unchanged && $status == HACKED_STATUS_UNHACKED) {
      continue;
    }
    $row = array(
    );

    // Now add the status:
    switch ($status) {
      case HACKED_STATUS_UNHACKED:
        $row[] = dt('Unchanged');
        break;
      case HACKED_STATUS_HACKED:
        $row[] = t('Changed');
        break;
      case HACKED_STATUS_DELETED:
        $row[] = t('Deleted');
        break;
      case HACKED_STATUS_UNCHECKED:
      default:
        $row[] = t('Unchecked');
        break;
    }

    $row[] = $file;


    $rows[] = $row;
  }
  drush_print_table($rows, TRUE);


}

/**
 * Validate hook for the hacked_diff drush command.
 */
function drush_hacked_diff_validate($short_name = '') {
  return drush_hacked_drush_command_validate($short_name);
}

/**
 * Drush command callback that shows the list of changes/unchanged files in a project.
 *
 * You may specify the --include-unchanged option to show unchanged files too,
 * otherwise just the changed and deleted files are shown.
 */
function drush_hacked_diff($short_name) {
  $project = hacked_project_load($short_name);

  $local_location = $project->file_get_location('local', '');
  $clean_location = $project->file_get_location('remote', '');

  // If the hasher is our ignore line endings one, then ignore line endings.
  if (variable_get('hacked_selected_file_hasher', HACKED_DEFAULT_FILE_HASHER) == 'hacked_ignore_line_endings') {
    $default_options = '-uprb';
  }
  else {
    $default_options = '-upr';
  }

  $diff_options = drush_get_option('diff-options', $default_options);
  drush_shell_exec("diff $diff_options $clean_location $local_location");

  $lines = drush_shell_exec_output();
  $local_location_trim = dirname($local_location . '/dummy.file') . '/';
  $clean_location_trim = dirname($clean_location . '/dummy.file') . '/';
  foreach ($lines as $line) {
    if (strpos($line, '+++') === 0) {
      $line = str_replace($local_location_trim, '', $line);
    }
    if (strpos($line, '---') === 0) {
      $line = str_replace($clean_location_trim, '', $line);
    }
    if (strpos($line, 'diff -upr') === 0) {
      $line = str_replace($clean_location_trim, 'a/', $line);
      $line = str_replace($local_location_trim, 'b/', $line);
    }

    drush_print($line);

  }

}
