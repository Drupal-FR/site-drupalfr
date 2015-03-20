<?php

// @todo Make it configurable.
define('LOGGER_DESTINATION', DRUPAL_ROOT . '/sites/default/files/botcha_logger.txt');

/**
 * Logger.
 */
abstract class Logger extends Decorator {
  const LOGGER_DATE_FORMAT = 'Y/m/d H:i:s';
  // A bitmask.
  const LOGGER_DISABLED = 0;
  const LOGGER_FILE = 1;  // 1 << 0 001
  const LOGGER_DB = 2;    // 1 << 1 010
  const LOGGER_EMAIL = 4; // 1 << 2 100

  protected $date_format;
  protected $destination;
  protected $level;

  public function __construct($decorators_applied = array(), &$relations = array(), $app) {
    parent::__construct($decorators_applied, $relations, $app);
    $this->setLevel(variable_get('botcha_logger_level', self::LOGGER_DISABLED));
    $this->setDestination(variable_get('botcha_logger_destination', LOGGER_DESTINATION));
    $this->setDateFormat(variable_get('botcha_logger_date_format', self::LOGGER_DATE_FORMAT));
  }

  public function setDateFormat($date_format) {
    $this->date_format = $date_format;
  }

  public function getDateFormat() {
    return $this->date_format;
  }

  public function setDestination($destination) {
    $this->destination = $destination;
  }

  public function getDestination() {
    return $this->destination;
  }

  public function setLevel($level) {
    $this->level = $level;
  }

  public function getLevel() {
    return $this->level;
  }

  protected function logCall($method, $arguments) {
    $app = $this->original;
    $class = get_class($app);
    $this->log("!class::!method(!args) <=", array('!class' => $class, '!method' => $method, '!args' => print_r($arguments, TRUE)));
    $return = call_user_func_array(array($app, $method), $arguments);
    $this->log("!class::!method(!args) return !return", array('!class' => $class, '!method' => $method, '!args' => print_r($arguments, TRUE), '!return' => print_r($return, TRUE)));
    return $return;
  }

  public function log($message, $placeholders = array()) {
    $levels = self::getLevels();
    foreach ($levels as $level) {
      if ($this->level & $level) {
        $this->put2log($message, $placeholders);
      }
    }
  }

  protected static function getLevels() {
    return array(
      self::LOGGER_DISABLED,
      self::LOGGER_FILE,
      self::LOGGER_DB,
      self::LOGGER_EMAIL,
    );
  }

  protected function put2log($message, $placeholders = array()) {
    $destination = NULL;
    $extra_headers = NULL;
    // Match corresponding log levels of error_log function.
    $message_type = 0;
    switch ($this->level) {
      case self::LOGGER_FILE:
        $message_type = 3;
        $destination = $this->getDestination();
        break;
      case self::LOGGER_EMAIL:
        $message_type = 1;
        // @todo Real headers.
        $extra_headers = NULL;
      case self::LOGGER_DB:
      default:
        // Just use default logging behavior.
        break;
    }
    error_log(date($this->date_format, time()) . ' ' .  t($message, $placeholders) . "\n", $message_type, $destination, $extra_headers);
  }
}


abstract class ApplicationLogger extends Logger implements IApplication {
  const ADMIN_PATH = 'admin';
  protected $type = 'Application';
  protected $ctrls = array();
  protected $controllers = array();

  public function __construct($decorators_applied = array(), &$relations = array(), $app) {
    parent::__construct($decorators_applied, $relations, $app);
    $this->getControllers();
  }

  public function getControllers() {
    $controllers = array();
    foreach ($this->ctrls as $ctrl_name) {
      $controllers[$ctrl_name] = $this->getController($ctrl_name);
    }
    return $controllers;
  }

  protected function getController($ctrl_name) {
    $this->controllers[$ctrl_name] = ComponentFactory::get($this->app_name, 'Controller', $ctrl_name, $this->decorators_applied, $this->relations);
    return $this->controllers[$ctrl_name];
  }
}


abstract class ControllerLogger extends Logger {
  protected $type = 'Controller';
  protected $model;

  public function __construct($decorators_applied = array(), &$relations = array(), $app) {
    parent::__construct($decorators_applied, $relations, $app);
    $this->getModel();
  }

  protected function getModel() {
    $this->model = ComponentFactory::get($this->app_name, 'Model', $this->controller_type, $this->decorators_applied, $this->relations);
    return $this->model;
  }
}


abstract class ModelLogger extends Logger {
  protected $type = 'Model';
}