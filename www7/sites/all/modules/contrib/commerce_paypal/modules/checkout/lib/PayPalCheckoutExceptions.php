<?php

/**
 * @file
 * Defines exception classes for use by the PayPalCheckoutClient class.
 */

class PayPalCheckoutAuthenticationException extends \Exception {}

/**
 * Defines a base class for HTTP response related exceptions.
 *
 * If an HTTP exception is thrown, the exception's code SHOULD correspond to the
 * status code of the response that generated the exception.
 */
class PayPalCheckoutHttpException extends \Exception {

  /**
   * Supplies a default message if none was supplied.
   */
  public function __construct($message = '', $code = 0, \Exception $previous = NULL) {
    if (empty($message)) {
      $message = '';

      if (!empty($code)) {
        $message .= $code . ' ';
      }

      $message .= 'HTTP response status code not OK.';
    }

    parent::__construct($message, $code, $previous);
  }
}

class PayPalCheckoutHttpServerErrorException extends PayPalCheckoutHttpException {}
class PayPalCheckoutHttpClientErrorException extends PayPalCheckoutHttpException {}
class PayPalCheckoutHttpRedirectionException extends PayPalCheckoutHttpException {}

class PayPalCheckoutInvalidResponseJsonException extends \Exception {}
