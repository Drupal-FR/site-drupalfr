<?php

/**
 * @file
 * Defines a class for consuming the PayPal Checkout API.
 */

/**
 * Defines the PayPalCheckoutClient class.
 *
 * A modern PHP library would namespace its classes under a package name, which
 * in this case would mean using the PayPalCheckout namespace and instantiating
 * new objects of this class via:
 *
 * $client = new PayPalCheckout\Client(...);
 *
 * Unfortunately, Drupal 7 does not support namespaces in its autoloader, as it
 * maintains compatibility with previous versions of PHP that did not support
 * namespaces. Thus this library does not currently use a namespace.
 */
class PayPalCheckoutClient {

  // Determine the maximum number of retries after an authentication failure.
  const RETRY_LIMIT = 2;

  // THe config array containing the client_id & secret.
  protected $config;

  // An array of HTTP headers.
  protected $headers;

  // Reference the logger callable.
  protected $logger;

  // Track the number of retries after an authentication failure.
  protected $retryCount;

  /**
   * Constructs a new PayPalCheckoutClient object.
   *
   * @param array $config
   *   An associative array containing at least the following keys:
   *   - client_id: The client ID.
   *   - secret: The client secret.
   *   - server: ("sandbox"|"live").
   * @param string $logger
   *   A callable used to log API request / response messages. Leave empty if
   *   logging is not needed.
   */
  public function __construct($config, $logger = NULL) {
    $this->config = $config;
    $this->logger = $logger;
    $this->retryCount = 0;
    $this->headers = array(
      'Accept' => 'application/json',
      // Defaults the "Content-Type" header to application/json.
      'Content-Type' => 'application/json',
      'PayPal-Partner-Attribution-Id' => 'CommerceGuys_Cart_SPB',
    );
  }

  /**
   * Sets the default cURL options.
   */
  public static function setDefaultCurlOptions($ch) {
    curl_setopt($ch, CURLOPT_HEADER, FALSE);
    curl_setopt($ch, CURLINFO_HEADER_OUT, TRUE);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, TRUE);
    curl_setopt($ch, CURLOPT_VERBOSE, FALSE);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 30);
    curl_setopt($ch, CURLOPT_TIMEOUT, 180);
  }

  /**
   * Send a message to the logger.
   *
   * @param string $message
   *   The message to log.
   */
  public function logMessage($message) {
    if (is_callable($this->logger)) {
      call_user_func($this->logger, $message);
    }
  }

  /**
   * Returns the base URL for the API, based on the specified mode.
   *
   * @return string
   *   The base URL for the API that query parameters will be appended to when
   *   submitting API requests.
   */
  public function baseUrl() {
    switch ($this->config['server']) {
      case 'live':
        return 'https://api.paypal.com';

      case 'sandbox':
      default:
        return 'https://api.sandbox.paypal.com';
    }
  }

  /**
   * Acquire an access token from PayPal.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   *
   * @throws PayPalCheckoutAuthenticationException
   */
  protected function acquireAccessToken() {
    $ch = curl_init();
    static::setDefaultCurlOptions($ch);
    $headers = array(
      'Authorization' => 'Basic ' .  base64_encode($this->config['client_id'] . ':' . $this->config['secret']),
      'Content-Type' => 'application/x-www-form-urlencoded',
    ) + $this->headers;
    $url = $this->baseUrl() . '/v1/oauth2/token';
    curl_setopt($ch, CURLOPT_HTTPHEADER, static::formatHeaders($headers));
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'POST');
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POSTFIELDS, 'grant_type=client_credentials');

    // Submit the request to the PayPal server.
    $response = curl_exec($ch);

    // Ensure we got a successful response by inspecting the HTTP status code.
    $response_info = curl_getinfo($ch);

    if (is_resource($ch)) {
      curl_close($ch);
    }

    $json = drupal_json_decode($response);

    if ($response_info['http_code'] == 401) {
      // Throw an exception indicating authentication failed.
      $message = 'Authentication failed.';
      if (isset($json['error_description'])) {
        $message = sprintf('Error description: %s.', $json['error_description']);
      }
      throw new PayPalCheckoutAuthenticationException($message);
    }

    return $json;
  }

  /**
   * Returns the access token stored locally if any, or get one from PayPal.
   *
   * @return string
   *   The Oauth2 access token.
   *
   * @throws PayPalCheckoutAuthenticationException
   */
  public function getAccessToken() {
    $access_token = variable_get('commerce_paypal_checkout_access_token', array());
    if (!empty($access_token['token']) && $access_token['expires'] > time()) {
      return $access_token['token'];
    }
    $response = $this->acquireAccessToken();
    variable_set('commerce_paypal_checkout_access_token', array(
      'token' => $response['access_token'],
      'expires' => time() + $response['expires_in'],
    ));
    return $response['access_token'];
  }

  /**
   * Creates an order in PayPal.
   *
   * @param array $parameters
   *   An array of parameters to include with the request.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function createOrder($parameters) {
    return $this->submitRequest('POST', 'v2/checkout/orders', $parameters);
  }

  /**
   * Get an existing order from PayPal.
   *
   * @param $remote_id
   *   The PayPal order ID.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function getOrder($remote_id) {
    return $this->submitRequest('GET', sprintf('v2/checkout/orders/%s', $remote_id));
  }

  /**
   * Updates an existing PayPal order.
   *
   * @param $remote_id
   *   The PayPal order ID.
   * @param array $parameters
   *   An array of parameters to include with the request.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function updateOrder($remote_id, $parameters) {
    return $this->submitRequest('PATCH', sprintf('v2/checkout/orders/%s', $remote_id), $parameters);
  }

  /**
   * Authorize payment for order.
   *
   * @param $remote_id
   *   The PayPal order ID.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function authorizeOrder($remote_id) {
    return $this->submitRequest('POST', sprintf('v2/checkout/orders/%s/authorize', $remote_id));

  }

  /**
   * Capture payment for order.
   *
   * @param $remote_id
   *   The PayPal order ID.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function captureOrder($remote_id) {
    return $this->submitRequest('POST', sprintf('v2/checkout/orders/%s/capture', $remote_id));
  }

  /**
   * Captures an authorized payment, by ID.
   *
   * @param $authorization_id
   *   The PayPal-generated ID for the authorized payment to capture.
   * @param array $parameters
   *   (optional An array of parameters to pass as the request body.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function capturePayment($authorization_id, array $parameters = array()) {
    return $this->submitRequest('POST', sprintf('v2/payments/authorizations/%s/capture', $authorization_id), $parameters);
  }

  /**
   * Reauthorizes an authorized PayPal account payment, by ID.
   *
   * @param $authorization_id
   *   The PayPal-generated ID of the authorized payment to reauthorize.
   * @param array $parameters
   *   (optional An array of parameters to pass as the request body.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function reAuthorizePayment($authorization_id, array $parameters = array()) {
    return $this->submitRequest('POST', sprintf('v2/payments/authorizations/%s/reauthorize', $authorization_id), $parameters);
  }

  /**
   * Refunds a captured payment, by ID.
   *
   * @param $capture_id
   *   The PayPal-generated ID for the captured payment to refund.
   * @param array $parameters
   *   (optional An array of parameters to pass as the request body.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function refundPayment($capture_id, array $parameters = array()) {
    return $this->submitRequest('POST', sprintf('v2/payments/captures/%s/refund', $capture_id), $parameters);
  }

  /**
   * Voids, or cancels, an authorized payment, by ID.
   *
   * @param $authorization_id
   *   The PayPal-generated ID of the authorized payment to void.
   */
  public function voidPayment($authorization_id) {
    return $this->submitRequest('POST', sprintf('v2/payments/authorizations/%s/void', $authorization_id));
  }

  /**
   * Submits an API request to the PayPal server.
   *
   * @param string $method
   *   The HTTP method to use. One of: 'GET', 'POST', 'PATCH', 'PUT', DELETE'.
   * @param string $path
   *   The remote path. The base URL will be automatically appended.
   * @param array $parameters
   *   An array of parameters to include with the request. Optional.
   *
   * @throws PayPalCheckoutAuthenticationException if the request fails authentication.
   * @throws PayPalCheckoutHttpServerErrorException if the response status code is 5xx.
   * @throws PayPalCheckoutHttpClientErrorException if the response status code is 4xx.
   * @throws PayPalCheckoutHttpRedirectionException if the response status code is 3xx.
   * @throws PayPalCheckoutInvalidResponseJsonException if the response is not valid JSON.
   *
   * @return string[]
   *   The API response JSON converted to an associative array.
   */
  public function submitRequest($method, $path, $parameters = array()) {
    $this->headers['Authorization'] = 'Bearer ' . $this->getAccessToken();
    $url = $this->baseUrl() . '/' . $path;
    $ch = curl_init();
    static::setDefaultCurlOptions($ch);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $method);
    if (!empty($parameters)) {
      if ($this->headers['Content-Type'] == 'application/json') {
        // JSON encode the fields and set them to the request body.
        curl_setopt($ch, CURLOPT_POSTFIELDS, drupal_json_encode($parameters));
      }
      else {
        curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($parameters, '', '&'));
      }
    }
    curl_setopt($ch, CURLOPT_HTTPHEADER, static::formatHeaders($this->headers));

    // Submit the request to the PayPal server.
    $this->logMessage(sprintf('Request URL: %s', $url));
    curl_setopt($ch, CURLOPT_URL, $url);
    $response = curl_exec($ch);

    // Ensure we got a successful response by inspecting the HTTP status code.
    $response_info = curl_getinfo($ch);

    if (is_resource($ch)) {
      curl_close($ch);
    }

    if ($response_info['http_code'] == 401) {
      // Attempt to get a new access token if the authentication failed.
      // This might happen if we're sending an expired access token to PayPal.
      if ($this->retryCount < static::RETRY_LIMIT) {
        $this->retryCount++;
        // Ensure we get a fresh access token next time.
        variable_del('commerce_paypal_checkout_access_token');
        return $this->submitRequest($method, $path, $parameters);
      }

      $json = drupal_json_decode($response);
      // Throw an exception indicating authentication failed.
      $message = 'Authentication failed.';
      if (isset($json['error_description'])) {
        $message = sprintf('Error description: %s.', $json['error_description']);
      }
      throw new PayPalCheckoutAuthenticationException($message);
    }
    elseif ($response_info['http_code'] >= 500) {
      // Throw an exception indicating a server error.
      throw new PayPalCheckoutHttpServerErrorException('', $response_info['http_code']);
    }
    elseif ($response_info['http_code'] >= 400) {
      // Throw an exception indicating a client error.
      throw new PayPalCheckoutHttpClientErrorException('', $response_info['http_code']);
    }
    elseif ($response_info['http_code'] >= 300) {
      // Throw an exception indicating a redirection that this library is not
      // going to automatically follow.
      throw new PayPalCheckoutHttpRedirectionException('', $response_info['http_code']);
    }

    // Attempt to convert the response body to an associative array.
    try {
      $json = drupal_json_decode($response);
    }
    catch (\Exception $e) {
      throw new PayPalCheckoutInvalidResponseJsonException('The API response string could not be parsed as JSON.');
    }

    return $json;
  }

  protected static function formatHeaders(array $headers) {
    $http_headers = array();
    foreach ($headers as $key => $value) {
      $http_headers[] = "$key: $value";
    }
    return $http_headers;
  }

}
