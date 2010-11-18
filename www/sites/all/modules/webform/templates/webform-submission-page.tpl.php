<?php
// $Id: webform-submission-page.tpl.php,v 1.2.2.1 2010/04/10 22:02:09 quicksketch Exp $

/**
 * @file
 * Customize the navigation shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The Webform submission array.
 * - $submission_content: The contents of the webform submission.
 * - $submission_navigation: The previous submission ID.
 * - $submission_information: The next submission ID.
 */

drupal_add_css(drupal_get_path('module', 'webform') .'/css/webform-admin.css', 'theme', 'all', FALSE);
?>

<?php print $submission_navigation; ?>
<?php print $submission_information; ?>

<?php print $submission_content; ?>

<?php print $submission_navigation; ?>
