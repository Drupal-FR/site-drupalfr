<?php // $Id: mimemail_compress.inc,v 1.5 2009/07/16 02:12:24 jerdavis Exp $

/**
 * Code based on emogrifier by Pelago Design and licensed under the MIT license
 * http://www.pelagodesign.com/sidecar/emogrifier/
 *
 * http://www.opensource.org/licenses/mit-license.php
 *
 * Copyright (c) 2009 Pelago Design
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so.
 */

function mimemail_compress_clean_message($message) {
  $parts = array();
  preg_match('|(<style[^>]+)>(.*)</style>|mis', $message, $matches);
  $css  = str_replace('<!--', '', $matches[2]);
  $css  = str_replace('-->', '', $css);
  $css = preg_replace('|\{|',"\n{\n", $css);
  $css = preg_replace('|\}|',"\n}\n", $css);
  $html = str_replace($matches[0], '', $message);
  $parts = array('html' => $html, 'css' => $css);
  return $parts;
}

class mimemail_compress {
  private $html = '';
  private $css  = '';

  public function mimemail_compress($html = '', $css = '') {
    $this->html = $html;
    $this->css  = $css;
  }

  public function compress() {
    $err = error_reporting(0);
    $doc = new DOMDocument('1.0', 'utf8');
    $doc->strictErrorChecking = false;
    $doc->formatOutput = true;
    $doc->loadHTML($this->html);
    $doc->normalizeDocument();
    $xpath = new DOMXPath($doc);
    $css = preg_replace('/\/\*.*\*\//sU', '', $this->css);
    preg_match_all('/^\s*([^{]+){([^}]+)}/mis', $css, $matches);

    foreach ($matches[1] as $key => $selector_string) {
      if (!strlen(trim($matches[2][$key]))) continue;
      $selectors = explode(',',$selector_string);
      foreach ($selectors as $selector) {
        if (strpos($selector,':') !== false) continue;
        $nodes = $xpath->query($this->css_to_xpath(trim($selector)));
        foreach($nodes as $node) {
          if ($node->hasAttribute('style')) {
            $style = $node->getAttribute('style');
            $old_style = $this->css_style_to_array($node->getAttribute('style'));
            $new_style = $this->css_style_to_array($matches[2][$key]);
            $compressed = array_merge($old_style,$new_style);
            $style = '';
            foreach ($compressed as $k => $v) $style .= ($k . ':' . $v . ';');
          }
          else {
            $style = trim($matches[2][$key]);
          }
          $node->setAttribute('style',$style);
        }
      }
    }
    $nodes = $xpath->query('//*[contains(translate(@style," ",""),"display:none;")]');
    foreach ($nodes as $node) $node->parentNode->removeChild($node);
    error_reporting($err);
    return $doc->saveHTML();
  }

  private function css_to_xpath($selector) {
    $search = array(
      '/\s+>\s+/',
      '/(\w+)\s+\+\s+(\w+)/',
      '/\s+/',
      '/(\w+)?\#([\w\-]+)/e',
      '/(\w+)?\.([\w\-]+)/e',
    );
    $replace = array(
      '/',
      '\\1/following-sibling::*[1]/self::\\2',
      '//',
      "(strlen('\\1') ? '\\1' : '*').'[@id=\"\\2\"]'",
      "(strlen('\\1') ? '\\1' : '*').'[contains(concat(\" \",@class,\" \"),concat(\" \",\"\\2\",\" \"))]'",
    );
    return '//'.preg_replace($search, $replace, trim($selector));
  }

  private function css_style_to_array($style) {
    $definitions = explode(';',$style);
    $css_styles = array();
    foreach ($definitions as $def) {
      list($key,$value) = explode(':',$def);
      if (empty($key) || empty($value)) continue;
      $css_styles[trim($key)] = trim($value);
    }
    return $css_styles;
  }
}
