<?php

/**
 * A few custom rewrites to the Apache_Solr_Service class, to allow providing
 * HTTP authentication and using this module without turning "allow_url_fopen"
 * on.
 *
 * Stolen from the apachesolr module for the most part.
 */
class SearchApiSolrConnection extends Apache_Solr_Service {

  /**
   * Authentication string (username + password) for HTTP authentication.
   */
  protected $http_auth;

  /**
   * Additional servlet mapping. Allows us to use the LukeRequestHandler Solr
   * service.
   */
  const LUKE_SERVLET = 'admin/luke';

  /**
   * Lucene index schema information.
   *
   * @var Apache_Solr_Response
   */
  protected $luke;

  /**
   * Identifies which version of the SolrPhpClient this uses, "old" or "new".
   *
   * @var bool
   */
  protected $newClient = FALSE;

  /**
   * Constructs a Solr connection with an optional HTTP user and password.
   *
   * @param array $options
   *   An array containing construction arguments.
   */
  public function __construct(array $options) {
    $options += array(
      'host' => 'localhost',
      'port' => 8983,
      'path' => '',
      'http_user' => NULL,
      'http_pass' => NULL,
      'default_field'=>'id',
    );
    parent::__construct($options['host'], $options['port'], $options['path']);
    if ($options['http_user'] && $options['http_pass']) {
      $this->http_auth = 'Basic ' . base64_encode($options['http_user'] . ':' . $options['http_pass']);
    }
    // Since /ping otherwise complains about missing default field.
    $this->_pingUrl .= '?q=' . $options['default_field'] . ':1';

    // As of July 2011, the newest release is r60, with Service.php having
    // revision 59. Revision 40 is just anything between 22 (old) and that.
    $this->newClient = trim(parent::SVN_REVISION, '$ :A..Za..z') > 40;
    if ($this->newClient) {
      $this->_httpTransport = new SearchApiSolrHttpTransport($this->http_auth);
    }
  }

  /**
   * Central method for making a get operation against this Solr Server.
   *
   * @see Apache_Solr_Service::_sendRawGet()
   */
  protected function _sendRawGet($url, $timeout = FALSE) {
    // Little "hack" to allow filter-only queries
    // Since "*:*" doesn't work with the dismax query handler, we mustn't set
    // "q", to let "q.alt" kick in. However, Apache_Solr_Service::search() will
    // always add "q", even if it is empty. Therefore, we delete empty "q"
    // parameters here.
    $url = preg_replace('/([?&])q=(&|$)/', '$1', $url);

    if ($this->newClient) {
      return parent::_sendRawGet($url, $timeout);
    }

    list($data, $headers) = $this->_makeHttpRequest($url, 'GET', array(), '', $timeout);
    $response = new Apache_Solr_Response($data, $headers, $this->_createDocuments, $this->_collapseSingleValueArrays);
    $code = (int) $response->getHttpStatus();
    if ($code != 200) {
      $message = $response->getHttpStatusMessage();
      if ($code >= 400 && $code != 403 && $code != 404) {
        // Add details, like Solr's exception message.
        $message .= $response->getRawResponse();
      }
      throw new Exception('"' . $code . '" Status: ' . $message);
    }
    return $response;
  }

  /**
   * Central method for making a post operation against this Solr Server.
   *
   * @see Apache_Solr_Service::_sendRawPost()
   */
  protected function _sendRawPost($url, $rawPost, $timeout = FALSE, $contentType = 'text/xml; charset=UTF-8') {
    if ($this->newClient) {
      return parent::_sendRawPost($url, $rawPost, $timeout, $contentType);
    }

    $request_headers = array('Content-Type' => $contentType);
    list($data, $headers) = $this->_makeHttpRequest($url, 'POST', $request_headers, $rawPost, $timeout);
    $response = new Apache_Solr_Response($data, $headers, $this->_createDocuments, $this->_collapseSingleValueArrays);
    $code = (int) $response->getHttpStatus();
    if ($code != 200) {
      $message = $response->getHttpStatusMessage();
      if ($code >= 400 && $code != 403 && $code != 404) {
        // Add details, like Solr's exception message.
        $message .= $response->getRawResponse();
      }
      throw new Exception('"' . $code . '" Status: ' . $message);
    }
    return $response;
  }


  /**
   * Call the /admin/ping servlet, to test the connection to the server.
   *
   * @param $timeout
   *   maximum time to wait for ping in seconds, -1 for unlimited (default 2).
   * @return
   *   (float) seconds taken to ping the server, FALSE if timeout occurs.
   */
  public function ping($timeout = 2) {
    if ($this->newClient) {
      return parent::ping($timeout);
    }
    $start = microtime(TRUE);

    if ($timeout <= 0.0) {
      $timeout = -1;
    }
    // Attempt a HEAD request to the solr ping url.
    list($data, $headers) = $this->_makeHttpRequest($this->_pingUrl, 'HEAD', array(), NULL, $timeout);
    $response = new Apache_Solr_Response($data, $headers);

    if ($response->getHttpStatus() == 200) {
      // Add 0.1 ms to the ping time so we never return 0.0.
      return microtime(TRUE) - $start + 0.0001;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Helper method for making an HTTP request, without using stupid stuff like
   * file_get_contents().
   */
  protected function _makeHttpRequest($url, $method = 'GET', $headers = array(), $content = '', $timeout = FALSE) {
    $options = array(
      'headers' => $headers,
      'method' => $method,
      'data' => $content,
    );

    if ($this->http_auth) {
      $options['headers']['Authorization'] = $this->http_auth;
    }
    if ($timeout) {
      $options['timeout'] = $timeout;
    }

    $result = drupal_http_request($url, $options);

    if (!isset($result->code) || $result->code < 0) {
      $result->code = 0;
      $result->status_message = 'Request failed';
      $result->protocol = 'HTTP/1.0';
    }
    // Additional information may be in the error property.
    if (isset($result->error)) {
      $result->status_message .= ': ' . check_plain($result->error);
    }

    if (!isset($result->data)) {
      $result->data = '';
    }
    // The headers have to be reformatted for the response class.
    $headers[] = "{$result->protocol} {$result->code} {$result->status_message}";
    if (isset($result->headers)) {
      foreach ($result->headers as $name => $value) {
        $headers[] = "$name: $value";
      }
    }
    return array($result->data, $headers);
  }

  /**
   * Convenience function for escaping a field name.
   *
   * Since field names can only contain one special character, ":", there is no
   * need to use the complete escape() method.
   *
   * @param string $value
   *   The field name to escape.
   *
   * @return string
   *   An escaped string suitable for passing to Solr.
   */
  static public function escapeFieldName($value) {
    $value = str_replace(':', '\:', $value);
    return $value;
  }

  /**
   * Convenience function for creating phrase syntax from a value.
   *
   * @param string $value
   *   The string to convert into a Solr phrase value.
   *
   * @return string
   *   A quoted string suitable for passing to Solr.
   */
  static public function phrase($value) {
    $value = str_replace("\\", "\\\\", $value);
    $value = str_replace('"', '\"', $value);
    return '"' . $value . '"';
  }

  /**
   * Get metadata about the Lucene index.
   *
   * @param int $num_terms
   *   Number of 'top terms' to return.
   *
   * @return Apache_Solr_Response
   *   A response object containing schema information.
   */
  public function getLuke($num_terms = 0) {
    if (!isset($this->luke[$num_terms])) {
      $url = $this->_constructUrl(self::LUKE_SERVLET, array('numTerms' => "$num_terms", 'wt' => self::SOLR_WRITER));
      $this->luke[$num_terms] = $this->_sendRawGet($url);
    }
    return $this->luke[$num_terms];
  }

  /**
   * Get metadata about fields in the Lucene index.
   *
   * @return array
   *   An array of objects, keyed by field name, describing fields on the index.
   *
   * @see SearchApiSolrConnection::getLuke()
   * @see http://wiki.apache.org/solr/LukeRequestHandler
   */
  public function getFields() {
    return (array) $this->getLuke()->fields;
  }

}
