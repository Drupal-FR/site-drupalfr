<?php // $Id: mimemail.tpl.php,v 1.2 2009/08/10 17:53:39 vauxia Exp $

/**
 * @file mimemail.tpl.php
 */
?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <?php if ($css): ?>
    <style type="text/css">
      <!--
      <?php print $css ?>
      -->
    </style>
    <?php endif; ?>
  </head>
  <body id="mimemail-body">
    <div id="center">
      <div id="main">
        <?php print $body ?>
      </div>
    </div>
  </body>
</html>
