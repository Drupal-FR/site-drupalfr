<?php

/**
 * @file
 * Template for PDFs.
 */

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">
<html lang="en" dir="ltr" version="HTML+RDFa 1.1" xmlns:xsd="http://www.w3.org/2001/XMLSchema#">
<head profile="http://www.w3.org/1999/xhtml/vocab">
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <style type="text/css">
    <?php print $inline_css; ?>
  </style>
</head>
<body>

<?php
  foreach ($viewed_orders as $key => $viewed_order):
    print '<div class="invoice">';
    print render($viewed_order);
    print '</div>';

    // Force a page break if we have a credit memo.
    if (count($viewed_orders) > ($key+1)) {
      print '<div style="page-break-before: always;" />';
    }
  endforeach;
?>
</body></html>
