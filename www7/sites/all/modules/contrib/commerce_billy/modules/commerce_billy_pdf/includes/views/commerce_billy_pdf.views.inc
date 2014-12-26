<?php

/**
 * @file
 * Views integration for Commerce Billy PDF.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_billy_pdf_views_data_alter(&$data) {
  $data['views_entity_commerce_order']['commerce_billy_pdf_invoice_link'] = array(
    'field' => array(
      'title' => t('Invoice PDF'),
      'handler' => 'commerce_billy_pdf_field_handler',
      'help' => t('Link to invoice PDF file.'),
    ),
  );
}
