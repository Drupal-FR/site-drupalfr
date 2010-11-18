<?php
// $Id: file.inc,v 1.17.2.4 2010/03/21 19:41:15 quicksketch Exp $

/**
 * @file
 * Webform module file component.
 */

/**
 * Implementation of _webform_defaults_component().
 */
function _webform_defaults_file() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'email' => 1,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'filtering' => array(
        'types' => array('gif', 'jpg', 'png'),
        'addextensions' => '',
        'size' => 800,
      ),
      'savelocation' => '',
      'width' => '',
      'description' => '',
      'attributes' => array(),
    ),
  );
}

/**
 * Implementation of _webform_theme_component().
 */
function _webform_theme_file() {
  return array(
    'webform_edit_file' => array(
      'arguments' => array('form' => NULL),
    ),
    'webform_render_file' => array(
      'arguments' => array('element' => NULL),
    ),
    'webform_display_file' => array(
      'arguments' => array('element' => NULL),
    ),
  );
}

/**
 * Implementation of _webform_edit_component().
 */
function _webform_edit_file($component) {
    $form = array();
    $form['#theme'] = 'webform_edit_file';

    $form['validation']['filtering'] = array(
      '#element_validate' => array('_webform_edit_file_filtering_validate'),
      '#parents' => array('extra', 'filtering'),
    );

    // Find the list of all currently valid extensions.
    $current_types = isset($component['extra']['filtering']['types']) ? $component['extra']['filtering']['types'] : array();

    $types = array('gif', 'jpg', 'png');
    $form['validation']['filtering']['types']['webimages'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Web Images'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('bmp', 'eps', 'tif', 'pict', 'psd');
    $form['validation']['filtering']['types']['desktopimages'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Desktop Images'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('txt', 'rtf', 'html', 'odf', 'pdf', 'doc', 'docx', 'ppt', 'pptx', 'xls', 'xlsx', 'xml');
    $form['validation']['filtering']['types']['documents'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Documents'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('avi', 'mov', 'mp3', 'ogg', 'wav');
    $form['validation']['filtering']['types']['media'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Media'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('bz2', 'dmg', 'gz', 'jar', 'rar', 'sit', 'tar', 'zip');
    $form['validation']['filtering']['types']['archives'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Archives'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $form['validation']['filtering']['addextensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Additional Extensions'),
      '#default_value' => $component['extra']['filtering']['addextensions'],
      '#description' => t('Enter a list of additional file extensions for this upload field, seperated by commas.<br /> Entered extensions will be appended to checked items above.'),
      '#size' => 60,
      '#weight' => 3,
      '#default_value' => $component['extra']['filtering']['addextensions'],
    );

    $form['validation']['filtering']['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Max Upload Size'),
      '#default_value' => $component['extra']['filtering']['size'],
      '#description' => t('Enter the max file size a user may upload (in KB).'),
      '#size' => 10,
      '#weight' => 3,
      '#default_value' => $component['extra']['filtering']['size'],
      '#parents' => array('extra', 'filtering', 'size'),
    );
    $form['extra']['savelocation'] = array(
      '#type' => 'textfield',
      '#title' => t('Upload Directory'),
      '#default_value' => $component['extra']['savelocation'],
      '#description' => '<div style="display: block">' . t('Webform uploads are always saved in the site files directory. You may optionally specify a subfolder to store your files.') . '</div>',
      '#weight' => 3,
      '#element_validate' => array('_webform_edit_file_check_directory'),
      '#after_build' => array('_webform_edit_file_check_directory'),
    );
    $form['display']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $component['extra']['width'],
      '#description' => t('Width of the file field.') . ' ' . t('Leaving blank will use the default size.'),
      '#size' => 5,
      '#maxlength' => 10,
      '#weight' => 4,
      '#parents' => array('extra', 'width')
    );
    return $form;
}

/**
 * A Form API after build function.
 *
 * Ensure that the destination directory exists and is writable.
 */
function _webform_edit_file_check_directory($element) {
  $base_dir = file_directory_path() . '/webform';
  $base_success = file_check_directory($base_dir, FILE_CREATE_DIRECTORY);

  $destination_dir = $base_dir . '/' . $element['#value'];
  $destination_success = file_check_directory($destination_dir, FILE_CREATE_DIRECTORY);
  if (!$base_success || !$destination_success) {
    form_set_error('savelocation', t('The save directory %directory could not be created. Check that the webform files directory is writtable.', array('%directory' => $destination_dir)));
  }
  return $element;
}

/**
 * A Form API element validate function.
 *
 * Change the submitted values of the component so that all filtering extensions
 * are saved as a single array.
 */
function _webform_edit_file_filtering_validate($element, &$form_state) {
  // Predefined types.
  $extensions = array();
  foreach (element_children($element['types']) as $category) {
    foreach (array_keys($element['types'][$category]['#value']) as $extension) {
      if ($element['types'][$category][$extension]['#value']) {
        $extensions[] = $extension;
      }
    }
  }

  // Additional types.
  $additional_extensions = explode(',', $element['addextensions']['#value']);
  foreach ($additional_extensions as $extension) {
    $clean_extension = drupal_strtolower(trim($extension));
    if (!empty($clean_extension) && !in_array($clean_extension, $extensions)) {
      $extensions[] = $clean_extension;
    }
  }

  form_set_value($element['types'], $extensions, $form_state);
}

function theme_webform_edit_file($form) {
  // Add a little JavaScript to check all the items in one type.
  $javascript = '
    function check_category_boxes() {
      var checkValue = !document.getElementById("edit-extra-filtering-types-"+arguments[0]+"-"+arguments[1]).checked;
      for(var i=1; i < arguments.length; i++) {
        document.getElementById("edit-extra-filtering-types-"+arguments[0]+"-"+arguments[i]).checked = checkValue;
      }
    }
 ';
  drupal_add_js($javascript, 'inline');

  // Format the components into a table.
  $per_row = 6;
  $rows = array();
  foreach (element_children($form['validation']['filtering']['types']) as $key => $filtergroup) {
    $row = array();
    $first_row = count($rows);
    if ($form['validation']['filtering']['types'][$filtergroup]['#type'] == 'checkboxes') {
      // Add the title.
      $row[] = $form['validation']['filtering']['types'][$filtergroup]['#title'];
      $row[] = '&nbsp;';
      // Convert the checkboxes into individual form-items.
      $checkboxes = expand_checkboxes($form['validation']['filtering']['types'][$filtergroup]);
      // Render the checkboxes in two rows.
      $checkcount = 0;
      $jsboxes = '';
      foreach (element_children($checkboxes) as $key) {
        $checkbox = $checkboxes[$key];
        if ($checkbox['#type'] == 'checkbox') {
          $checkcount++;
          $jsboxes .= "'" . $checkbox['#return_value'] . "',";
          if ($checkcount <= $per_row) {
            $row[] = array('data' => drupal_render($checkbox));
          }
          elseif ($checkcount == $per_row + 1) {
            $rows[] = array('data' => $row, 'style' => 'border-bottom: none;');
            $row = array(array('data' => '&nbsp;'), array('data' => '&nbsp;'));
            $row[] = array('data' => drupal_render($checkbox));
          }
          else {
            $row[] = array('data' => drupal_render($checkbox));
          }
        }
      }
      // Pretty up the table a little bit.
      $current_cell = $checkcount % $per_row;
      if ($current_cell > 0) {
        $colspan = $per_row - $current_cell   + 1;
        $row[$current_cell + 1]['colspan'] = $colspan;
      }
      // Add the javascript links.
      $jsboxes = drupal_substr($jsboxes, 0, drupal_strlen($jsboxes) - 1);
      $rows[] = array('data' => $row);
      $select_link = ' <a href="javascript:check_category_boxes(\'' . $filtergroup . '\',' . $jsboxes . ')">(select)</a>';
      $rows[$first_row]['data'][1] = array('data' => $select_link, 'width' => 40);
      unset($form['validation']['filtering']['types'][$filtergroup]);
    }
    elseif ($filtergroup != 'size') {
      // Add other fields to the table (ie. additional extensions).
      $row[] = $form['validation']['filtering']['types'][$filtergroup]['#title'];
      unset($form['validation']['filtering']['types'][$filtergroup]['#title']);
      $row[] = array(
        'data' => drupal_render($form['validation']['filtering']['types'][$filtergroup]),
        'colspan' => $per_row + 1,
      );
      unset($form['validation']['filtering']['types'][$filtergroup]);
      $rows[] = array('data' => $row);
    }
  }
  $header = array(array('data' => t('Category'), 'colspan' => '2'), array('data' => t('Types'), 'colspan' => $per_row));

  // Create the table inside the form.
  $form['validation']['filtering']['types']['table'] = array(
    '#value' => theme('table', $header, $rows)
  );

  $output = drupal_render($form);

  // Prefix the upload location field with the default path for webform.
  $output = str_replace('Upload Directory: </label>', 'Upload Directory: </label>' . file_directory_path() . '/webform/', $output);

  return $output;
}

/**
 * Implementation of _webform_render_component().
 */
function _webform_render_file($component, $value = NULL) {
  $node = node_load($component['nid']);
  $form_key = implode('_', webform_component_parent_keys($node, $component));
  $element[$form_key] = array(
    '#type'          => $component['type'],
    '#title'         => $component['name'],
    //'#required'      => $component['mandatory'], // Drupal core bug with required file uploads.
    '#weight'        => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#attributes'    => $component['extra']['attributes'],
    '#tree'          => FALSE, // file_check_upload assumes a flat $_FILES structure.
    '#element_validate' => array(
      '_webform_validate_file',
      '_webform_required_file',  // Custom required routine.
    ),
    '#webform_component' => $component,
  );
  $element['#webform_required'] = $component['mandatory'];
  $element['#webform_form_key'] = $form_key;
  $element['#weight'] = $component['weight'];
  $element['#theme'] = 'webform_render_file';
  $element['#prefix'] = '<div class="webform-component webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">';
  $element['#suffix'] = '</div>';


  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $element[$form_key]['#size'] = $component['extra']['width'];
  }

  // Add a hidden element to store the FID for new files.
  $element['_fid'] = array(
    '#type' => 'hidden',
    '#default_value' => '',
  );

  // Add a hidden element to store the FID for existing files.
  $element['_old'] = array(
    '#type' => 'hidden',
    '#value' => isset($value[0]) ? $value[0] : NULL,
  );

  return $element;
}

/**
 * Render a File component.
 */
function theme_webform_render_file($element) {
  // Add information about the existing file, if any.
  $value = $element['_fid']['#value'] ? $element['_fid']['#value'] : $element['_old']['#value'];
  if ($value && ($file = webform_get_file($value))) {
    $element['#suffix'] = ' ' . l(t('Download !filename', array('!filename' => webform_file_name($file->filepath))), webform_file_url($file->filepath)) . (isset($element['#suffix']) ? $element['#suffix'] : '');
    $element['#description'] = t('Uploading a new file will replace the current file.');
  }

  // Add the required asterisk.
  if ($element['#webform_required']) {
    $element[$element['#webform_form_key']]['#required'] = TRUE;
  }

  return drupal_render($element);
}

/**
 * A Form API element validate function.
 *
 * Fix Drupal core's handling of required file fields.
 */
function _webform_required_file($element, $form_state) {
  $component = $element['#webform_component'];
  $parents = $element['#array_parents'];
  array_pop($parents);
  $form_key = implode('_', $parents);

  // Do not validate requiredness on back or draft button.
  if (isset($form_state['clicked_button']['#validate']) && empty($form_state['clicked_button']['#validate'])) {
    return;
  }

  // Check if a value is already set in the hidden field.
  $values = $form_state['values'];
  $key = array_shift($parents);
  $found = FALSE;
  while (isset($values[$key])) {
    if (isset($values[$key])) {
      $values = $values[$key];
      $found = TRUE;
    }
    else {
      $found = FALSE;
    }
    $key = array_shift($parents);
  }

  if (!$found || (empty($values['_fid']) && empty($values['_old']))) {
    if (empty($_FILES['files']['name'][$form_key]) && $component['mandatory']) {
      form_error($element, t('%field field is required.', array('%field' => $component['name'])));
    }
  }
}

/**
 * A Form API element validate function.
 *
 * Ensure that the uploaded file matches the specified file types.
 */
function _webform_validate_file($element, &$form_state) {
  $component = $element['#webform_component'];
  $form_key = implode('_', $element['#parents']);

  if (empty($_FILES['files']['name'][$form_key])) {
    return;
  }

  // Build a human readable list of extensions:
  $extensions = $component['extra']['filtering']['types'];
  $extension_list = '';
  if (count($extensions) > 1) {
    for ($n = 0; $n < count($extensions) - 1; $n++) {
      $extension_list .= $extensions[$n] . ', ';
    }
    $extension_list .= 'or ' . $extensions[count($extensions) - 1];
  }
  elseif (!empty($extensions)) {
    $extension_list = $extensions[0];
  }

  if (in_array('jpg', $extensions)) {
    $extensions[] = 'jpeg';
  }

  $dot = strrpos($_FILES['files']['name'][$form_key], '.');
  $extension = drupal_strtolower(substr($_FILES['files']['name'][$form_key], $dot+1));
  $file_error = FALSE;
  if (!empty($extensions) && !in_array($extension, $extensions)) {
    form_error($element, t("Files with the '%ext' extension are not allowed, please upload a file with a %exts extension.", array('%ext' => $extension, '%exts' => $extension_list)));
    $file_error = TRUE;
  }

  // Now let's check the file size (limit is set in KB).
  if ($_FILES['files']['size'][$form_key] > $component['extra']['filtering']['size'] * 1024) {
    form_error($element, t("The file '%filename' is too large (%filesize KB). Please upload a file %maxsize KB or smaller.", array('%filename' => $_FILES['files']['name'][$form_key], '%filesize' => (int) ($_FILES['files']['size'][$form_key]/1024), '%maxsize' => $component['extra']['filtering']['size'])));
    $file_error = TRUE;
  }

  // Save the file to a temporary location.
  if (!$file_error) {
    $upload_dir = file_directory_path() . '/webform/' . $component['extra']['savelocation'];
    if (file_check_directory($upload_dir, FILE_CREATE_DIRECTORY)) {
      $file = file_save_upload($form_key, array(), $upload_dir);
      if ($file) {
        @chmod($file->filepath, 0664);

        // Set the hidden field value.
        $parents = $element['#array_parents'];
        array_pop($parents);
        $parents[] = '_fid';
        form_set_value(array('#parents' => $parents), $file->fid, $form_state);
      }
      else {
        drupal_set_message(t('The uploaded file %filename was unable to be saved. The destination directory may not be writable.', array('%filename' => $file->filename)), 'error');
      }
    }
    else {
      drupal_set_message(t('The uploaded file was unable to be saved. The destination directory does not exist.'), 'error');
    }
  }
}

/**
 * Implementation of _webform_submit_component().
 */
function _webform_submit_file($component, $value) {

  if ($value['_fid'] && ($file = webform_get_file($value['_fid']))) {
    // Save any new files permanently.
    file_set_status($file, FILE_STATUS_PERMANENT);

    // Delete any previous files.
    if ($value['_old'] && ($existing = webform_get_file($value['_old']))) {
      file_delete($existing->filepath);
      db_query("DELETE FROM {files} WHERE fid = %d", $existing->fid);
    }

    $value = $file->fid;
  }
  else {
    $value = $value['_old'] ? $value['_old'] : NULL;
  }

  return $value;
}

/**
 * Implementation of _webform_display_component().
 */
function _webform_display_file($component, $value, $format = 'html') {
  $fid = isset($value[0]) ? $value[0] : NULL;
  return array(
     '#title' => $component['name'],
     '#value' => $fid ? webform_get_file($fid) : NULL,
     '#weight' => $component['weight'],
     '#theme' => 'webform_display_file',
     '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
     '#post_render' => array('webform_element_wrapper'),
     '#component' => $component,
     '#format' => $format,
  );
}

/**
 * Format the output of text data for this component
 */
function theme_webform_display_file($element) {
  $file = $element['#value'];
  $url = !empty($file) ? webform_file_url($file->filepath) : t('no upload');
  return !empty($file) ? ($element['#format'] == 'text' ? $url : l($file->filename, $url)) : ' ';
}

/**
 * Implementation of _webform_delete_component().
 */
function _webform_delete_file($component, $value) {
  // Delete an individual submission file.
  if (isset($value[0]) && ($file = webform_get_file($value[0]))) {
    unlink($file->filepath);
    db_query("DELETE FROM {files} WHERE fid = '%d'", $file->fid);
  }
}

/**
 * Implementation of _webform_analysis_component().
 */
function _webform_analysis_file($component, $sids = array()) {
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (" . implode(",", $placeholders) . ")" : "";
  $query = 'SELECT data ' .
    ' FROM {webform_submitted_data} ' .
    ' WHERE nid = %d ' .
    ' AND cid = %d' . $sidfilter;
  $nonblanks = 0;
  $sizetotal = 0;
  $submissions = 0;

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  while ($data = db_fetch_array($result)) {
    $file = webform_get_file($data['data']);
    if (isset($file->filesize)) {
      $nonblanks++;
      $sizetotal += $file->filesize;
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User uploaded file'), $nonblanks);
  $rows[2] = array(t('Average uploaded file size'), ($sizetotal != 0 ? (int) (($sizetotal/$nonblanks)/1024) . ' KB' : '0'));
  return $rows;
}

/**
 * Implementation of _webform_table_component().
 */
function _webform_table_file($component, $value) {
  $output = '';
  $file = webform_get_file($value[0]);
  if (!empty($file->fid)) {
    $output = '<a href="' . webform_file_url($file->filepath) . '">' . webform_file_name($file->filepath) . '</a>';
    $output .= ' (' . (int) ($file->filesize/1024) . ' KB)';
  }
  return $output;
}

/**
 * Implementation of _webform_csv_headers_component().
 */
function _webform_csv_headers_file($component, $export_options) {
  $header = array();
  // Two columns in header.
  $header[0] = array('', '');
  $header[1] = array($component['name'], '');
  $header[2] = array(t('Name'), t('Filesize (KB)'));
  return $header;
}

/**
 * Implementation of _webform_csv_data_component().
 */
function _webform_csv_data_file($component, $export_options, $value) {
  $file = webform_get_file($value[0]);
  return empty($file->filename) ? array('', '') : array(webform_file_url($file->filepath), (int) ($file->filesize/1024));
}

/**
 * Helper function to create proper file names for uploaded file.
 */
function webform_file_name($filepath) {
  if (!empty($filepath)) {
    $info = pathinfo($filepath);
    $file_name = $info['basename'];
  }
  return isset($file_name) ? $file_name : '';
}

/**
 * Helper function to create proper URLs for uploaded file.
 */
function webform_file_url($filepath) {
  if (!empty($filepath)) {
    $info = pathinfo($filepath);
    $file_url = file_create_url($info['dirname'] . '/' . rawurlencode($info['basename']));
  }
  return isset($file_url) ? $file_url : '';
}

/**
 * Helper function to load a file from the database.
 */
function webform_get_file($fid) {
  static $files;

  if (!isset($files[$fid])) {
    if (empty($fid)) {
      $files[$fid] = FALSE;
    }
    else {
      $files[$fid] = db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = %d", $fid));
    }
  }

  return $files[$fid];
}