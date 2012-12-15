<?php

/**
 * @file
 * MigratePhoneFieldHandler class definition.
 */

/**
 * Wrap Migrate's simple field handler for 'phone' fields.
 */
class MigratePhoneFieldHandler extends MigrateSimpleFieldHandler {

  public function __construct() {
    $this->registerTypes(array('phone'));
  }

}
