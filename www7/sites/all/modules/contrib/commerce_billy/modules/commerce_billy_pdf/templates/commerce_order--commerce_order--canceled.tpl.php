<?php

/**
 * @file
 * Template for credit memos (=canceled orders).
 */

?>

<div class="invoice-canceled">
  <div class="header">
    <img src="<?php print $content['invoice_logo']['#value']; ?>"/>
    <div class="invoice-header">
      <p><?php print render($content['invoice_header']); ?></p>
    </div>
  </div>

  <hr/>

  <div class="invoice-header-date"><?php print render($content['invoice_header_date']); ?></div>
  <div class="customer"><?php print render($content['commerce_customer_billing']); ?></div>
  <h1 class="credit-memo"><?php print t('Credit Memo'); ?></h1>
  <div class="invoice-number"><?php print render($content['order_number']); ?></div>
  <div class="order-id"><?php print render($content['order_id']); ?></div>

  <div class="order-total"><?php print render($content['commerce_order_total']); ?></div>

  <div class="invoice-footer"><?php print render($content['invoice_footer']); ?></div>
</div>
