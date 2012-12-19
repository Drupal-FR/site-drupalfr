<?php

/**
 * @file
 * Contains the SearchApiSolrHttpTransport class.
 */

/**
 * Drupal-based implementation of the HTTP transport interface.
 *
 * Uses drupal_http_request() for sending the request.
 */
class SearchApiSolrHttpTransport extends Apache_Solr_HttpTransport_Abstract {

  /**
   * If set, an HTTP authentification string to use.
   *
   * @var string
   */
  protected $http_auth;

  /**
   * Constructor.
   *
   * @param $http_auth
   *   If set, an HTTP authentification string to use.
   */
  public function __construct($http_auth = NULL) {
    $this->http_auth = $http_auth;
  }

  /**
   * Perform a GET HTTP operation with an optional timeout and return the response
   * contents, use getLastResponseHeaders to retrieve HTTP headers
   *
   * @param string $url
   * @param float $timeout
   * @return Apache_Solr_HttpTransport_Response HTTP response
   */
  public function performGetRequest($url, $timeout = false) {
    return $this->performHttpRequest('GET', $url, $timeout);
  }

  /**
   * Perform a HEAD HTTP operation with an optional timeout and return the response
   * headers - NOTE: head requests have no response body
   *
   * @param string $url
   * @param float $timeout
   * @return Apache_Solr_HttpTransport_Response HTTP response
   */
  public function performHeadRequest($url, $timeout = false) {
    return $this->performHttpRequest('HEAD', $url, $timeout);
  }

  /**
   * Perform a POST HTTP operation with an optional timeout and return the response
   * contents, use getLastResponseHeaders to retrieve HTTP headers
   *
   * @param string $url
   * @param string $rawPost
   * @param string $contentType
   * @param float $timeout
   * @return Apache_Solr_HttpTransport_Response HTTP response
   */
  public function performPostRequest($url, $rawPost, $contentType, $timeout = false) {
    return $this->performHttpRequest('POST', $url, $timeout, $rawPost, $contentType);
  }

  /**
   * Helper method for making an HTTP request.
   */
  protected function performHttpRequest($method, $url, $timeout, $rawPost = NULL, $contentType = NULL) {
    $options = array(
      'method' => $method,
      'timeout' => $timeout && $timeout > 0 ? $timeout : $this->getDefaultTimeout(),
      'headers' => array(),
    );

    if ($this->http_auth) {
      $options['headers']['Authorization'] = $this->http_auth;
    }
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    if ($rawPost) {
      $options['data'] = $rawPost;
    }
    if ($contentType) {
      $options['headers']['Content-Type'] = $contentType;
    }

    $response = drupal_http_request($url, $options);

    $type = isset($response->headers['content-type']) ? $response->headers['content-type'] : 'text/xml';
    $body = isset($response->data) ? $response->data : NULL;
    return new Apache_Solr_HttpTransport_Response($response->code, $type, $body);
  }

}
