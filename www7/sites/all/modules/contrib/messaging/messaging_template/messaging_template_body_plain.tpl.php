<?php
/**
 * @
 * Default template implementation for Mesage body - Plaintext format
 * 
 * Available variables:
 * - $header
 * - $content
 * - $footer
 * - $element, Original renderable array (Properties #format, #method, etc..)
 */
?>
<?php if ($header): ?>
<?php print $header; ?>

<?php endif; ?>
<?php if ($content): ?>
<?php print $content; ?>
<?php endif; ?>
<?php if ($footer): ?>

--
<?php print $footer; ?>
<?php endif; ?>
