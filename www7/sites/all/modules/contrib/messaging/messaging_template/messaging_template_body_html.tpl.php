<?php
/**
 * @
 * Default template implementation for Mesage body -HTML format
 * 
 * Available variables:
 * - $header
 * - $content
 * - $footer
 * - $element, Original renderable array
 */
?>

<div id="message-body">
<?php if ($header): ?>
  <div id="message-header">
  <?php print $header;?>
  </div>
  <br />
<?php endif; ?>
<?php if ($content): ?>
  <div id="message-content">
  <?php print $content; ?>
  </div>
<?php endif; ?>
<?php if ($footer): ?>
  <br />
  <br />
  <div id="message-footer">
  <?php print $footer; ?>
  </div>
<?php endif; ?>
</div>
