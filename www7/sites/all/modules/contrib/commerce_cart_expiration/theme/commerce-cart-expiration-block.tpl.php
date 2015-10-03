<?php

/**
 * @file
 * Default implementation of the cart expiration block template.
 *
 * Available variables:
 * - $content: Rendered block content.
 *
 * Helper variables:
 * - $order: The full order object for the shopping cart.
 * - $expire_in: Seconds left until cart expires.
 *
 * @see template_preprocess()
 * @see template_process()
 */
?>
<div class="cart-expiration">
  <?php print $content; ?>
</div>
