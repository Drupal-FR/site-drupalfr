<?php // $Id: mimemail.theme.inc,v 1.2 2009/08/10 17:53:39 vauxia Exp $

function mimemail_theme_theme() {
  $path = drupal_get_path('module', 'mimemail') .'/theme';

  return array(
    'mimemail_message' => array(
      'arguments' => array('body' => NULL, 'mailkey' => NULL),
      'template' => 'mimemail',
      'pattern' => 'mimemail__',
      'file' => 'mimemail.theme.inc',
      'path' => $path,
    )
  );
}

/**
 * A preprocess function for theme('mimemail_message').
 *
 * The $variables array initially contains the following arguments:
 * - $body:  The message body
 * - $mailkey:  The mailkey associated with the message
 *
 * See includes/mimemail.tpl.php for additional variables
 */
function template_preprocess_mimemail_message(&$variables) {
  $theme = variable_get('theme_default', NULL);

  // Check for the existence of a mail.css file in the current theme folder
  if (!file_exists($styles)) {
    $styles = drupal_get_path('theme', $theme) .'/mail.css';
  }

  // If no mail.css was found, gather all style sheets
  if (!file_exists($styles)) {
    // embed a version of all style definitions
    $styles = preg_replace('|<link.*href="'. base_path()
                           .'([^"?]*)[?"].*|', '\1', drupal_get_css());
  }

  // Process each style sheet
  foreach (explode("\n", $styles) as $style) {
    if (file_exists($style)) $css .= file_get_contents($style);
  }

  $variables['css'] = str_replace(' ','', str_replace("\n", '', $css));
}
