<?php
/**
 * @
 * Default template implementation for Mesage body -HTML format
 * 
 * Available variables:
 * - $body
 * - $element, Original renderable array
 * - $css, Custom css, to be injected maybe with some pre-processing function
 */
?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <?php if (!empty($css)): ?>
    <style type="text/css">
      <!--
      <?php print $css ?>
      -->
    </style>
    <?php endif; ?>
  </head>
  <body id="message-body">
  <?php if ($body): ?>
    <?php print $body; ?>
  <?php endif; ?>
  </body>
</html>