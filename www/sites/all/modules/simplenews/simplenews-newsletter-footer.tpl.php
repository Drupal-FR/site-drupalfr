<?php
// $Id: simplenews-newsletter-footer.tpl.php,v 1.1.2.4 2009/01/02 11:59:33 sutharsan Exp $

/**
 * @file
 * Default theme implementation to format the simplenews newsletter footer.
 * 
 * Copy this file in your theme directory to create a custom themed footer.
 * Rename it to simplenews-newsletter-footer--<tid>.tpl.php to override it for a 
 * newsletter using the newsletter term's id.
 *
 * Available variables:
 * - $node: newsletter node object
 * - $language: language object
 * - $key: email key [node|test]
 * - $format: newsletter format [plain|html]
 * - $unsubscribe_text: unsubscribe text
 * - $test_message: test message warning message
 *
 * Available tokens:
 * - !confirm_unsubscribe_url: unsubscribe url to be used as link
 * for more tokens: see simplenews_mail_tokens()
 *
 * @see template_preprocess_simplenews_newsletter_footer()
 */
?>
<?php if ($format == 'html'): ?>
  <p class="newsletter-footer"><a href="!confirm_unsubscribe_url"><?php print $unsubscribe_text ?></a></p>
<?php else: ?>
-- <?php print $unsubscribe_text ?>: !confirm_unsubscribe_url
<?php endif ?>

<?php if ($key == 'test'): ?>
- - - <?php print $test_message ?> - - -
<?php endif ?>