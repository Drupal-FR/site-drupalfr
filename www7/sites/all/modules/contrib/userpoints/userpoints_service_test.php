<?php

function print_error() {
  print_r(xmlrpc_errno());
  print_r(xmlrpc_error_msg());
}

  if ($_SERVER['SERVER_ADDR'] && $_SERVER['REMOTE_ADDR']) {
    print "This script should be run from the command line\n";
    exit(1);
  }

  require_once('./includes/bootstrap.inc');
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

  if ($argc != 4) {
    print "Usage: php $argv[0] uid server-URL API-key\n";
    exit(2);
  }

  $uid = (int)$argv[1];
  $s   = $argv[2];
  $k   = $argv[3];

  print "Querying points for user: $uid\n";
  $result = xmlrpc($s, 'userpoints.get', $k, $uid, 0);
  if ($result != FALSE) {
    print "Points for user $uid = ". $result['points'] ."\n";
  }
  else {
    print_error();
  }

  $points = 15;
  print "Adding $points points for user: $uid\n";
  $result = xmlrpc($s, 'userpoints.points', $k, $uid, $points, 0, NULL, NULL);
  if ($result != FALSE) {
    if ($result['status'] == TRUE) {
      print "Success\n";
    }
    else {
      print "Failed\n";
    }
  }
  else {
    print_error();
  }

  $result = xmlrpc($s, 'userpoints.get', $k, $uid, 0);
  if ($result != FALSE) {
    print "Points for user $uid = ". $result['points'] ."\n";
  }
  else {
    print_error();
  }

  $points = -5;
  print "Subtracting $points points for user: $uid\n";
  $result = xmlrpc($s, 'userpoints.points', $k, $uid, $points, 0, NULL, NULL);
  if ($result != FALSE) {
    if ($result['status'] == TRUE) {
      print "Success\n";
    }
    else {
      print "Failed\n";
    }
  }
  else {
    print_error();
  }
  
  $result = xmlrpc($s, 'userpoints.get', $k, $uid, 0);
  if ($result != FALSE) {
    print "Points for user $uid = ". $result['points'] ."\n";
  }
  else {
    print_error();
  }

