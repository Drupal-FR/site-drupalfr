<?php
/**
 * @file
 * Drupal Messaging Framework - Send_Method class file
 */

/**
 * Base class for mail sending methods
 */
class Messaging_HTML_Mail_Method extends Messaging_Mail_Method {
  // Default group and address type
  public $method = 'mimemail';
  public $type = 'mail';
  public $anonymous = TRUE;
  public $format = MESSAGING_FORMAT_HTML;
  // Mail system object to create it only once and reuse
  protected static $mail_system;

  /**
   * Rebuild message in Drupal mail format, right before sending
   * 
   * Note that our 'body' element is a renderable array, not like drupal_mail()
   * This may clash with some module altering the mail
   * 
   * This mimics drupal_mail for finest access to properties
   * @param $address
   *   Email address to send to
   * @param $message
   *   Message object
   */
  protected function mail_build($address, $message) {
    $params = $this->message_params($message);
    $template = $message->get_template();
    $mail = array(
      'headers' => $this->mail_headers($message, $params),
      'id'       => $message->module . '_' . $message->key,
      'module'   => $message->module,
      'key'      => $message->key,
      'to'       => $address,
      'from'     => isset($params['from']) ? $params['from'] : $params['default_from'],
      'language' => $message->get_language(),
      'params'   => $params,
      'subject'  => $message->get_subject(),
      // Bundle up the variables into a structured array for altering.
      'body'     => $template->set_format(MESSAGING_FORMAT_HTML)->build('body'),
      'body_plain' => $template->set_format(MESSAGING_FORMAT_PLAIN)->render('body'),
      'attachments' => $message->get_files(),
    );
  
    // Build the e-mail (get subject and body, allow additional headers) by
    // invoking hook_mail() on this module. We cannot use module_invoke() as
    // we need to have $message by reference in hook_mail().
    if (function_exists($function = $message->module . '_mail')) {
      $function($message->key, $mail, $params);
    }
  
    // Invoke hook_mail_alter() to allow all modules to alter the resulting e-mail.
    drupal_alter('mail', $mail);
    
    // Invoke drupal_mail without sending, then override headers
    return $mail;
  }
  /**
   * Actually send mail through Drupal system
   * 
   * @param $mail
   *   Built Drupal mail array, not rendered
   * @param $message
   *   Original message object
   */
  protected static function mail_send($mail, $message) {
    // Retrieve the responsible implementation for this message.
    //$system = drupal_mail_system($mail['module'], $mail['key']);
    if (!isset(self::$mail_system)) {
      self::$mail_system = new Messaging_HTML_MailSystem();
    }
    // Format the message body, last chance for formatting
    $mail = self::$mail_system->format($mail);
    return self::$mail_system->mail($mail);    
  }

  
}

/**
 * Sendgrid mail system
 * 
 * We use HTML formatting + we need to take care of properly filtering the headers.
 * 
 * The e-mail is sent through default PHP mail though, Sendgrid config should be in the backend
 * 
 * For Exim configuration see http://wiki.sendgrid.com/doku.php?id=exim4
 * 
 * @todo Explore other options for sending, like PHPMailer or
 */
class Messaging_HTML_MailSystem extends DefaultMailSystem {
  /**
   * Concatenate and wrap the e-mail body for plain-text mails.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   The formatted $message.
   */
  public function format(array $mail) {
    // Note we use two templates here
    // - messaging_template_body_html, for the body content
    // - messaging_htmlmail, for formatting the mail body
    $body = drupal_render($mail['body']);
    $mail['body'] = theme('messaging_htmlmail', array('body' => $body, 'element' => $mail['body']));
    $mimemail = $this->mimemail_html_body($mail['body'], $mail['subject'], $mail['body_plain'], $mail['attachments']);
    // Merge mimemail headers on top of regular headers
    $mail['headers'] = array_merge($mail['headers'], $mimemail['headers']);
    $mail['body'] = $mimemail['body'];
    return $mail;
  }

  /**
   * Send an e-mail message, using Drupal variables and default settings.
   *
   * @see http://php.net/manual/en/function.mail.php
   * @see drupal_mail()
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   * @return
   *   TRUE if the mail was successfully accepted, otherwise FALSE.
   */
  public function mail(array $message) {
    // If 'Return-Path' isn't already set in php.ini, we pass it separately
    // as an additional parameter instead of in the header.
    // However, if PHP's 'safe_mode' is on, this is not allowed.
    if (isset($message['headers']['Return-Path']) && !ini_get('safe_mode')) {
      $return_path_set = strpos(ini_get('sendmail_path'), ' -f');
      if (!$return_path_set) {
        $message['Return-Path'] = $message['headers']['Return-Path'];
        unset($message['headers']['Return-Path']);
      }
    }
    /*
    $mail_subject = mime_header_encode($message['subject']);
    $line_endings = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
    $mail_body = preg_replace('@\r?\n@', $line_endings, $message['body']);
    $mail_headers = mimemail_rfc_headers($message['headers']);
    */

    $mimeheaders = array();
    foreach ($message['headers'] as $name => $value) {
      $mimeheaders[] = $name . ': ' . mime_header_encode($value);
    }
    $line_endings = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
    // Prepare mail commands.
    $mail_subject = mime_header_encode($message['subject']);
    // Note: e-mail uses CRLF for line-endings. PHP's API requires LF
    // on Unix and CRLF on Windows. Drupal automatically guesses the
    // line-ending format appropriate for your system. If you need to
    // override this, adjust $conf['mail_line_endings'] in settings.php.
    $mail_body = preg_replace('@\r?\n@', $line_endings, $message['body']);
    // For headers, PHP's API suggests that we use CRLF normally,
    // but some MTAs incorrectly replace LF with CRLF. See #234403.
    $mail_headers = join("\n", $mimeheaders);
    
    if (isset($message['Return-Path']) && !ini_get('safe_mode')) {
      $mail_result = mail(
        $message['to'],
        $mail_subject,
        $mail_body,
        $mail_headers,
        // Pass the Return-Path via sendmail's -f command.
        '-f ' . $message['Return-Path']
      );
    }
    else {
      // The optional $additional_parameters argument to mail() is not allowed
      // if safe_mode is enabled. Passing any value throws a PHP warning and
      // makes mail() return FALSE.
      $mail_result = mail(
        $message['to'],
        $mail_subject,
        $mail_body,
        $mail_headers
      );
    }
    return $mail_result;
  }
  /**
   * Generate a multipart message body with a text alternative for some html text
   * @param $body An HTML message body
   * @param $subject The message subject
   *
   * @return
   *     an array containing the elements 'header' and 'body'.
   *     'body' is the mime encoded multipart body of a mail.
   *     'headers' is an array that includes some headers for the mail to be sent.
   *
   * The first mime part is a multipart/alternative containing mime-encoded
   * sub-parts for HTML and plaintext.  Each subsequent part is the required
   * image/attachment
   */
  function mimemail_html_body($body, $subject, $text = NULL, $attachments = array()) {
    if (empty($text)) {
      // todo: remove this preg_replace once filter_xss() is properly handling
      // direct descendant css selectors '>' in inline CSS. For now this cleans
      // up our plain text part. See mimemail #364198, drupal #370903
      $text = preg_replace('|<style.*?</style>|mis', '', $body);
      $text = drupal_html_to_text($text);
    }
  
    $content_type = 'multipart/related; type="multipart/alternative"';
  
    $text_part = array('Content-Type' => 'text/plain; charset=utf-8', 'content' => $text);
    
    /**
     * @todo When replacing links we break the ones that are yet to be replaced (tokens)
    //expand all local links
    $pattern = '/(<a[^>]+href=")([^"]*)/mi';
    $body = preg_replace_callback($pattern, '_mimemail_expand_links', $body);
    **/
    
    $mime_parts = $this->mimemail_extract_files($body);
    
    $content = array($text_part, array_shift($mime_parts));
    $content = $this->mimemail_multipart_body($content, 'multipart/alternative', TRUE);
    $parts = array(array('Content-Type' => $content['headers']['Content-Type'], 'content' => $content['body']));
  
    if ($mime_parts) {
      $parts = array_merge($parts, $mime_parts);
    }

    foreach ($attachments as $a) {
      $a = (object) $a;
      // Check the list parameter if its set or ignore it (Upload module support).
      if(!isset($a->list) || $a->list) {
        _mimemail_file($a->filepath, $a->filename, $a->filemime, 'attachment');
        $parts = array_merge($parts, _mimemail_file());
      }
    }
  
    return $this->mimemail_multipart_body($parts, $content_type);
  }
  /**
   *
   * @param $parts
   *        an array of parts to be included
   *        each part is itself an array:
   *        array(
   *          'name' => $name the name of the attachement
   *          'content' => $content textual content
   *          'file' => $file a file
   *          'Content-Type' => Content type of either file or content.
   *                            Mandatory for content, optional for file.
   *                            If not present, it will be derived from
   *                            file the file if mime_content_type is available.
   *                            If not, application/octet-stream is used.
   *          'Content-Disposition' => optional, inline is assumed
   *          'Content-Transfer-Encoding' => optional,
   *                                         base64 is assumed for files
   *                                         8bit for other content.
   *          'Content-ID' => optional, for in-mail references to attachements.
   *        )
   *        name is mandatory, one of content and file is required,
   *        they are mutually exclusive.
   *
   * @param $content_type
   *        Content-Type for the combined message, optional, default: multipart/mixed
   *
   * @return
   *     an array containing the elements 'header' and 'body'.
   *     'body' is the mime encoded multipart body of a mail.
   *     'headers' is an array that includes some headers for the mail to be sent.
   */
  function mimemail_multipart_body($parts, $content_type = 'multipart/mixed; charset=utf-8', $sub_part = FALSE) {
    $boundary = md5(uniqid(time()));
    $body = '';
    $headers = array(
      'Content-Type' => "$content_type; boundary=\"$boundary\"",
    );
    if (!$sub_part) {
      $headers['MIME-Version'] = '1.0';
      $body = "This is a multi-part message in MIME format.\n";
    }
  
    foreach ($parts as $part) {
      $part_headers = array();
  
      if (isset($part['Content-ID'])) {
        $part_headers['Content-ID'] = '<'. $part['Content-ID'] .'>';
      }
  
      if (isset($part['Content-Type'])) {
        $part_headers['Content-Type'] = $part['Content-Type'];
      }
  
      if (isset($part['Content-Disposition'])) {
        $part_headers['Content-Disposition'] = $part['Content-Disposition'];
      }
      else {
        $part_headers['Content-Disposition'] = 'inline';
      }
  
      if (isset($part['Content-Transfer-Encoding'])) {
        $part_headers['Content-Transfer-Encoding'] = $part['Content-Transfer-Encoding'];
      }
  
      // mail content provided as a string
      if (isset($part['content']) && $part['content']) {
        if (!isset($part['Content-Transfer-Encoding'])) {
          $part_headers['Content-Transfer-Encoding'] = '8bit';
        }
        $part_body = $part['content'];
        if (isset($part['name'])) {
          $part_headers['Content-Type'] .= '; name="'. $part['name'] .'"';
          $part_headers['Content-Disposition'] .= '; filename="'. $part['name'] .'"';
        }
  
      // mail content references in a filename
      }
      else {
        if (!isset($part['Content-Transfer-Encoding'])) {
          $part_headers['Content-Transfer-Encoding'] = 'base64';
        }
  
        if (!isset($part['Content-Type'])) {
          $part['Content-Type'] = file_get_mimetype($part['file']);
        }
  
        if (isset($part['name'])) {
          $part_headers['Content-Type'] .= '; name="'. $part['name'] .'"';
          $part_headers['Content-Disposition'] .= '; filename="'. $part['name'] .'"';
        }
  
        if (isset($part['file'])) {
          $part_body = chunk_split(base64_encode(file_get_contents($part['file'])));
        }
      }
  
      $body .= "\n--$boundary\n";
      $body .= $this->mimemail_rfc_headers($part_headers) ."\n\n";
      $body .= $part_body;
    }
    $body .= "\n--$boundary--\n";
  
    return array('headers' => $headers, 'body' => $body);
  }

  /**
   * Extracts links to local images from html documents.
   *
   * @param $html html text
   * @param $name document name
   *
   * @return an array of arrays
   *            array(array(
   *                     'name' => document name
   *                     'content' => html text, local image urls replaced by Content-IDs,
   *                     'Content-Type' => 'text/html; charset=utf-8')
   *                  array(
   *                     'name' => file name,
   *                     'file' => reference to local file,
   *                     'Content-ID' => generated Content-ID,
   *                     'Content-Type' => derived using mime_content_type
   *                                       if available, educated guess otherwise
   *                     )
   *                  )
   */
  function mimemail_extract_files($html) {
    $pattern = '/(<link[^>]+href=[\'"]?|<object[^>]+codebase=[\'"]?|@import |[\s]src=[\'"]?)([^\'>"]+)([\'"]?)/mis';
    $html = preg_replace_callback($pattern, '_mimemail_replace_files', $html);
  
    $document = array(array(
      'Content-Type' => "text/html; charset=utf-8",
      'Content-Transfer-Encoding' => '8bit',
      'content' => $html,
    ));
  
    $files = _mimemail_file();
  
    return array_merge($document, $files);
  }

  /**
   * Attempts to RFC822-compliant headers for the mail message or its MIME parts
   * TODO could use some enhancement and stress testing
   *
   * @param $headers An array of headers
   * @return header string
   */
  function mimemail_rfc_headers($headers) {
    $header = '';
    $crlf = variable_get('mimemail_crlf', "\n");
    foreach ($headers as $key => $value) {
      $key = trim($key);
      // collapse spaces and get rid of newline characters
      $value = preg_replace('/(\s+|\n|\r|^\s|\s$)/', ' ', $value);
  
      //fold headers if they're too long
      if (strlen($value) > 60) {
        //if there's a semicolon, use that to separate
        if (count($array = preg_split('/;\s*/', $value)) > 1) {
          $value = trim(join(";$crlf    ", $array));
        }
        else {
          $value = wordwrap($value, 50, "$crlf    ", FALSE);
        }
      }
      $header .= "$key: $value$crlf";
    }
    return trim($header);
  }
}
/**
 * Common mail functions for sending e-mail, taken from Drupal's mime-mail module
 * 
 *   Allie Micka < allie at pajunas dot com >
 *   Originally written by Gerhard.
 */

/**
 * Callback for preg_replace_callback()
 */
function _mimemail_replace_files($matches) {
  return stripslashes($matches[1]) . _mimemail_file($matches[2]) . stripslashes($matches[3]);
}

/**
 * Helper function to extract local files
 *
 * @param $url a URL to a file
 *
 * @return an absolute :
 */
function _mimemail_file($url = NULL, $name = '', $type = '', $disposition = 'related') {
  static $files = array();
  static $filenames = array();

  if ($url) {
    $url = _mimemail_url($url, 'TRUE');

    // If the $url is absolute, we're done here.
    if (strpos($url, '://') !== FALSE || preg_match('!mailto:!', $url)) {
      return $url;
    }
    // The $url is a relative file path, continue processing.
    else {
      // Download method is private, and the $url needs conversion.
      if (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PRIVATE && strpos($url, 'system/files/') !== FALSE) {
        $file = file_create_path(drupal_substr($url, (strpos($url, 'system/files/') + drupal_strlen('system/files/'))));
      }
      // Download method is public.
      else {
        $file = $url;
      }
    }
  }

  if (isset($file) && @file_exists($file)) {
    // Prevent duplicate items.
    if (isset($filenames[$file])) return 'cid:'. $filenames[$file];

    $content_id = md5($file) .'@'. $_SERVER['HTTP_HOST'];

    if (!$name) $name = drupal_substr($file, strrpos($file, '/') + 1);

    $new_file = array(
      'name' => $name,
      'file' => $file,
      'Content-ID' => $content_id,
      'Content-Disposition' => $disposition,
    );

    $new_file['Content-Type'] = (!empty($name)) ? file_get_mimetype($name) : file_get_mimetype($file);

    $files[] = $new_file;
    $filenames[$file] = $content_id;

    return 'cid:'. $content_id;
  }
  // The $file is not exist, return the $url if possible.
  elseif ($url) {
    return $url;
  }

  $ret = $files;
  $files = array();
  $filenames = array();
  return $ret;
}

/**
 * Callback for preg_replace_callback()
 */
function _mimemail_expand_links($matches) {
  return $matches[1] . _mimemail_url($matches[2]);
}

/*
 * Split a multi-part message using mime boundaries
 */
function mimemail_parse_boundary($part) {
  $m = array();
  if (preg_match('/.*boundary="?([^";]+)"?.*/', $part['headers']['Content-Type'], $m)) {
    $boundary = "\n--". $m[1];
    $body     = str_replace("$boundary--", '', $part['body']);
    return array_slice(explode($boundary, $body), 1);
  }
  return array($part['body']);
}

/*
 * Split a message (or message part) into its headers and body section
 */
function mimemail_parse_headers($message) {
  // Split out body and headers
  if (preg_match("/^(.*?)\r?\n\r?\n(.*)/s", $message, $match)) {
    list($hdr, $body) = array($match[1], $match[2]);
  }

  // Un-fold the headers.
  $hdr = preg_replace(array("/\r/", "/\n(\t| )+/"), array('', ' '), $hdr);

  $headers = array();
  foreach (explode("\n", trim($hdr)) as $row) {
    $split = strpos( $row, ':' );
    $name = trim(substr($row, 0, $split));
    $val  = trim(substr($row, $split+1));
    $headers[$name] = $val;
  }

  $type = (preg_replace('/\s*([^;]+).*/', '\1', $headers['Content-Type']));

  return array('headers' => $headers, 'body' => $body, 'content-type' => $type);
}

/*
 * Return a decoded mime part in UTF8
 */
function mimemail_parse_content($part) {
  $content = $part['body'];

  // Decode this part
  if ($encoding = strtolower($part['headers']['Content-Transfer-Encoding'])) {
    switch ($encoding) {

      case 'base64':
        $content = base64_decode($content);
        break;

      case 'quoted-printable':
        $content = quoted_printable_decode($content);
        break;

      case '7bit':  // 7bit is the RFC default
        break;
    }
  }

  // Try to convert character set to UTF-8.
  if (preg_match('/.*charset="?([^";]+)"?.*/', $part['headers']['Content-Type'], $m)) {
    $content = drupal_convert_to_utf8($content, $m[1]);
    //$content = iconv($m[1], 'utf-8', $content);
  }

  return $content;
}

/*
 * Convert a mime part into a file array
 */
function mimemail_parse_attachment($part) {
  $m = array();
  if (preg_match('/.*filename="?([^";])"?.*/', $part['headers']['Content-Disposition'], $m)) {
    $name = $m[1];
  }
  elseif (preg_match('/.*name="?([^";])"?.*/', $part['headers']['Content-Type'], $m)) {
    $name = $m[1];
  }

  return array(
    'filename' => $name,
    'filemime' => $part['content-type'],
    'content'  => mimemail_parse_content($part),
  );
}

/**
 * Helper function to format urls
 *
 * @param $url an url
 *
 * @return an absolute url, sans mailto:
 */
function _mimemail_url($url, $embed_file = NULL) {
  global $base_url;
  $url = urldecode($url);

  // If the URL is absolute or a mailto, return it as-is.
  if (strpos($url, '://') !== FALSE || preg_match('!mailto:!', $url)) {
    $url = str_replace(' ', '%20', $url);
    return $url;
  }

  $url = preg_replace( '!^'. base_path() .'!', '', $url, 1);

  // If we're processing to embed the file, we're done here so return.
  if ($embed_file) return $url;

  if (!preg_match('!^\?q=*!', $url)) {
    $strip_clean = TRUE;
  }

  $url = str_replace('?q=', '', $url);
  list($url, $fragment) = explode('#', $url, 2);
  list($path, $query) = explode('?', $url, 2);

  // If we're dealing with an intra-document reference, return it.
  if (empty($path) && !empty($fragment)) {
    return '#'. $fragment;
  }

  // If we have not yet returned, then let's clean things up and leave.
  $url = url($path, array('query' => $query, 'fragment' => $fragment, 'absolute' => TRUE));


  // If url() added a ?q= where there should not be one, remove it.
  if ($strip_clean) $url = preg_replace('!\?q=!', '', $url);

  $url = str_replace('+', '%20', $url);
  return $url;
}

