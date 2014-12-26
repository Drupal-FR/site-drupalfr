<?php

/**
 * @file
 * Template for invoiced orders.
 */

?>

<div class="invoice-invoiced">
  <div class="header">
    <img src="<?php print $content['invoice_logo']['#value']; ?>"/>
    <div class="invoice-header">
        <p><?php print render($content['invoice_header']); ?></p>
    </div>
  </div>

  <hr/>

  <div class="invoice-header-date"><?php print render($content['invoice_header_date']); ?></div>
  <div class="customer"><?php print render($content['commerce_customer_billing']); ?></div>
  <div class="invoice-number"><?php print render($content['order_number']); ?></div>
  <div class="order-id"><?php print render($content['order_id']); ?></div>

  <div class="line-items">
    <div class="line-items-view"><?php print render($content['commerce_line_items']); ?></div>
    <div class="order-total"><?php print render($content['commerce_order_total']); ?></div>
  </div>
  <div class="invoice-text"><?php print render($content['invoice_text']); ?></div>

  <div class="invoice-footer"><?php print render($content['invoice_footer']); ?></div>
</div>
