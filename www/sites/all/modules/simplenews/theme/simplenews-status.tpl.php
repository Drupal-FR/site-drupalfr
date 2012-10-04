<?php

/**
 * @file
 * Default theme implementation to display the simplenews newsletter status.
 *
 * Available variables:
 * - $image: status image
 * - $alt: 'alt' message
 * - $title: 'title' message
 *
 * @see template_preprocess_simplenews_status()
 */
?>
  <?php if (isset($image)): ?>
    <img src="<?php print $image; ?>" width="15" height="15" alt="<?php print $alt; ?>" border="0" title="<?php print $title; ?>" />
    <?php if (isset($trailer)): ?>
      <span id="simplenews_status_sent"> (<?php print $trailer; ?>) </span>
    <?php endif; ?>
  <?php else: ?>
    <?php print $title; ?>
  <?php endif; ?>
