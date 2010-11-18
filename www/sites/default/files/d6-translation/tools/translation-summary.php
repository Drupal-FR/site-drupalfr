#!/usr/bin/php
<?php

$VERSION = "0.1";
$AUTHOR = "damz";

if (php_sapi_name() !== 'cli')
  die("This script is designed to be run from the command-line.\n");

function show_usage() {  
  echo <<< EOF
This is summary v$VERSION by $AUTHOR.

Usage: $argv[0] FOLDER TYPE

Where:
 - FOLDER is the folder with all the .po files
 - TYPE is the type of output:
    text: output is a tabbed text
    html: output is an html snippet


EOF;
  die();
}

if (count($argv) <> 3) {
  show_usage();
}
else {
  $folder = $argv[1];
  $type = strtolower($argv[2]);
  if ($type != 'text' && $type != 'html')
    show_usage();
}

$list = glob("$folder/*.po");

$total = array('translated' => 0, 'total' => 0, 'fuzzy' => 0);
$data = array();
$sort = array();
foreach($list as $file) {
  $item = array(
    'file' => basename($file),
    'translated' => (int) `msgattrib --translated $file | grep -c "^$"`,
    'total' => (int) `msgattrib $file | grep -c "^$"`,
    'fuzzy' => (int) `msgattrib --fuzzy $file | grep -c "^$"`
  );
  $sort[$item['file']] = $item['total'] - $item['translated'];
  $data[$item['file']] = $item;
  $total['translated'] += $item['translated'];
  $total['total'] += $item['total'];
  $total['fuzzy'] += $item['fuzzy'];
}

arsort($sort);
$sorted_data = array();
foreach($sort as $file => $sort) {
  $sorted_data[$file] = $data[$file];
}

if ($type == 'text')
  echo output_text($sorted_data, $total);
else
  echo output_html($sorted_data, $total);

die();

function output_html($data, $total) {
  $output  = "";
  $output .= '<table class="msgattrib">'."\n";
  $output .= "<thead><tr><th>File</th>";
  $output .= "<th>Status</th>";
  $output .= "<th>%</th></tr></thead>\n";
  $output .= "<tbody>\n";
  $odd = false;
  foreach($data as $item) {
    $output .= "  <tr class=\"".($odd ? "odd" : "even")."\"><td><a href=\"/sites/drupalfr.org/files/d6-translation/$item[file]\">$item[file]</a></td><td>$item[translated] / $item[total]</td><td>" . ($item['total'] > 0 ? round($item['translated'] / $item['total'] * 100)."&nbsp;%" : "") . "</td></tr>\n";
  }
  $output .= "  <tr class=\"summary\"><td>Total</td><td>$total[translated] / $total[total]</td><td>" . ($total['total'] > 0 ? round($total['translated'] / $total['total'] * 100)."&nbsp;%" : "")."</td></tr>\n";
  $output .= "</tbody>\n";
  $output .= "</table>\n";
  return $output;
}

function output_text($data, $total) {
  $output  = "";
  foreach($data as $item) {
    $output .= sprintf("%-30s %8s / %4d  %3s %%\n",
      $item['file'],
      "$item[translated] - $item[fuzzy]",
      $item['total'],
      ($item['total'] > 0 ? round($item['translated']/$item['total']*100) : 'na')
    );
  }
  $output .= "---\n";
  $output .= sprintf("%-30s %4d / %4d  %3s %%\n",
      "Summary",
      $total['translated'],
      $total['total'],
      ($total['total'] > 0 ? round($total['translated']/$total['total']*100) : 'na')
    );
  return $output;
}

