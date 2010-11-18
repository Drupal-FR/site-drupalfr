<?php
// $Id: webform-mail.tpl.php,v 1.3.2.2 2010/03/25 02:07:29 quicksketch Exp $

/**
 * @file
 * Customize the e-mails sent by Webform after successful submission.
 *
 * This file may be renamed "webform-mail-[nid].tpl.php" to target a
 * specific webform e-mail on your site. Or you can leave it
 * "webform-mail.tpl.php" to affect all webform e-mails on your site.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The webform submission.
 * - $email: The entire e-mail configuration settings.
 * - $user: The current user submitting the form.
 * - $ip_address: The IP address of the user submitting the form.
 *
 * The $email['email'] variable can be used to send different e-mails to different users
 * when using the "default" e-mail template.
 */
?>
<?php print t('Submitted on %date'); ?>

<?php if ($user->uid): ?>
<?php print t('Submitted by user: %username'); ?>
<?php else: ?>
<?php print t('Submitted by anonymous user: [%ip_address]'); ?>
<?php endif; ?>


<?php print t('Submitted values are') ?>:

%email_values

<?php print t('The results of this submission may be viewed at:') ?>

%submission_url
