<?php

/**
 * @file
 * Script to be executed with 'drush php:script'.
 */

declare(strict_types = 1);

use Drupal\user\Entity\User;
use Drush\Commands\DrushCommands;

if (!isset($this) || !($this instanceof DrushCommands)) {
  return;
}

if (!isset($extra)) {
  $extra = [];
}

if (count($extra) != 3) {
  \Drupal::messenger()->addError('This script expects the following arguments: Account name, Account mail, Account password.');
  \Drupal::messenger()->addError('The number of provided arguments is ' . count($extra) . '.');
  return;
}

$user = User::load(1);

if ($user == NULL) {
  \Drupal::messenger()->addError('There is no user with ID 1.');
  return;
}

$user->setUsername($extra[0])
  ->setEmail($extra[1])
  ->setPassword($extra[2])
  ->save();
