<?php

/**
 * @file
 * Theme implementation to display a scald_atom.
 *
 * Available variables:
 * - $content: An array of items for the content of the scald_atom.
 *
 * Other variables:
 * - ?
 *
 * @see template_preprocess()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>

<div id="scald_atom-<?php print $sid; ?>" class="scald_atom">

<?php if ($view_mode != 'full'): ?>
  <h2><?php print l($title, 'atom/' . $sid); ?></h2>
<?php endif; ?>

  <?php print render($content); ?>
</div>
