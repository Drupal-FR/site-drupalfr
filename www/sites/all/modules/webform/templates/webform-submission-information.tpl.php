<?php
// $Id: webform-submission-information.tpl.php,v 1.1 2010/01/14 06:12:47 quicksketch Exp $

/**
 * @file
 * Customize the header information shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The contents of the webform submission.
 * - $account: The user that submitted the form.
 */
?>
<fieldset>
  <legend><?php print t('Submission information'); ?></legend>
  <?php print theme('user_picture', $account); ?>
  <div><?php print t('Form: !form', array('!form' => l($node->title, 'node/' . $node->nid))); ?></div>
  <div><?php print t('Submitted by !name', array('!name' => theme('username', $account))); ?></div>
  <div><?php print format_date($submission->submitted, 'large'); ?></div>
  <div><?php print $submission->remote_addr; ?></div>
</fieldset>
