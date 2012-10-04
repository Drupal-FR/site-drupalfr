<?php


/**
 * Theme project status report.
 *
 * @ingroup themeable
 */
function theme_hacked_report($data) {
  $output = '';

  $last = variable_get('hacked_last_report', 0);
  $output = '<div class="update checked">'. ($last ? t('Last checked: @time ago', array('@time' => format_interval(time() - $last))) : t('Last checked: never'));
  $output .= ' <span class="check-manually">('. l(t('Check manually'), 'admin/reports/hacked/rebuild-report') .')</span>';
  $output .= "</div>\n";

  if (!is_array($data)) {
    $output .= '<p>' . $data . '</p>';
    return $output;
  }

  $header = array();
  $rows = array();

  foreach ($data as $project) {
    if (!isset($project['status'])) {
      continue;
    }

    switch ($project['status']) {
      case HACKED_STATUS_UNHACKED:
        $class = 'ok';
        $icon = theme('image', 'misc/watchdog-ok.png', t('Unchanged'), t('Unchanged'));
        break;
      case HACKED_STATUS_HACKED:
        $class = 'error';
        $icon = theme('image', 'misc/watchdog-error.png', t('Changed'), t('Changed'));
        break;
      case HACKED_STATUS_UNCHECKED:
      default:
        $class = 'warning';
        $icon = theme('image', 'misc/watchdog-warning.png', t('Unchecked'), t('Unchecked'));
        break;
    }

    $row = '<div class="version-status">';
    switch ($project['status']) {
      case HACKED_STATUS_UNHACKED:
        $row .= t('Unchanged');
        break;
      case HACKED_STATUS_HACKED:
        $row .= '<span class="not-current">' . t('Changed!') . '</span>';
        break;
      case HACKED_STATUS_UNCHECKED:
      default:
        $row .= '<span class="">' . t('Unchecked') . '</span>';
        break;
    }
    $row .= '<span class="icon">' . $icon . '</span>';
    $row .= "</div>\n";

    $row .= '<div class="project">';
    if (isset($project['title'])) {
      if (isset($project['link'])) {
        $row .= l($project['title'], $project['link']);
      }
      else {
        $row .= check_plain($project['title']);
      }
    }
    else {
      $row .= check_plain($project['name']);
    }
    $row .= ' ' . check_plain($project['existing_version']);
    if ($project['install_type'] == 'dev' && !empty($project['datestamp'])) {
      $row .= ' <span class="version-date">(' . format_date($project['datestamp'], 'custom', 'Y-M-d') . ')</span>';
    }
    $row .= "</div>\n";


    $row .= "<div class=\"versions\">\n";
    $row .= '<table class="version version-latest">';
    $row .= '<tr>';

    $unreadable_message = "";
    if ($project['counts']['access_denied'] > 0) {
      $unreadable_message = ', ' . format_plural($project['counts']['access_denied'], '1 unreadable file', '@count unreadable files');
    }

    $row .= '<td class="version-title">'. format_plural($project['counts']['different'], '1 file changed', '@count files changed') . ', ' . format_plural($project['counts']['missing'], '1 file deleted', '@count files deleted') . $unreadable_message . "</td>\n";
    $row .= '</tr>';
    $row .= '</table>';
    $row .= '<table class="version version-latest">';
    $row .= '<tr>';
    $row .= '<td class="version-title">'. l(t('View details of changes'), 'admin/reports/hacked/' . $project['name']) ."</td>\n";
    $row .= '</tr>';

    $row .= '</table>';
    $row .= "</div>\n";

    $row .= '<div class="includes">';
    sort($project['includes']);
    $row .= t('Includes: %includes', array('%includes' => implode(', ', $project['includes'])));
    $row .= "</div>\n";

    $row .= "</div>\n"; // info div.

    if (!isset($rows[$project['project_type']])) {
      $rows[$project['project_type']] = array();
    }
    $rows[$project['project_type']][] = array(
      'class' => $class,
      'data' => array($row),
    );
  }

  $project_types = array(
    'core' => t('Drupal core'),
    'module' => t('Modules'),
    'theme' => t('Themes'),
    'disabled-module' => t('Disabled modules'),
    'disabled-theme' => t('Disabled themes'),
  );
  foreach ($project_types as $type_name => $type_label) {
    if (!empty($rows[$type_name])) {
      $output .= "\n<h3>" . $type_label . "</h3>\n";
      $output .= theme('table', $header, $rows[$type_name], array('class' => 'update'));
    }
  }
  drupal_add_css(drupal_get_path('module', 'update') . '/update.css');
  return $output;
}
