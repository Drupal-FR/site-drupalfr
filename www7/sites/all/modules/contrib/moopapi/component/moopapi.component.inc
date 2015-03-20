<?php

/**
 * @todo Desc for Component.
 */
abstract class Component {
  // It is intended to store the name of the module (for example, Botcha).
  protected $app_name = '';
  protected $decorators_applied = array();
  // Identifier of a part of an application, each of which has
  // its own controller and model. The main part of each application
  // has id ID_APPLICATION - so we always know where to start.
  protected $id = '';
  const ID_APPLICATION = 'Application';
  // In fact there are only 2 types: controller and model, we
  // need a way to differ them.
  protected $type = 'Component';
  const TYPE_CONTROLLER = 'Controller';
  const TYPE_MODEL = 'Model';
  // For later use.
  // @todo Implement relations.
  // A list of relation names to be initialize at the first launch.
  protected $rltns = array();
  // Relations between parts of the application. Is intended to provide for
  // controllers a way to get and modify common data accordingly.
  protected $relations = array();

  public function __construct($decorators_applied = array(), &$relations = array()) {
    $this->decorators_applied = $decorators_applied;
    $this->relations = &$relations;
  }
}


/**
 * An Abstract Factory for Components.
 */
abstract class ComponentFactory {
  /**
   *
   * @param string $app_name
   * @param string $type
   * @param string $id
   * @param array $decorators
   * @return \decorator_class
   */
  public static function get($app_name, $type, $id, $decorators = array(), &$relations = array()) {
    // Template of classname for replacings.
    $class_pattern = '%app_name%id%type';
    $class_search = array('%app_name', '%id', '%type');
    // Switch to determine an object of which class to initialize.
    switch ($id) {
      case Component::ID_APPLICATION:
        $class_replace = array($app_name, '', '');
        break;
      default:
        $class_replace = array($app_name, $id, $type);
        break;
    }
    $class = str_replace($class_search, $class_replace, $class_pattern);
    $decorators_applied = $decorators;
    $component = new $class($decorators_applied, $relations);
    // Some preparations before building.
    $app_name_lower = strtolower($app_name);
    $type_lower = strtolower($type);
    $id_lower = strtolower($id);
    // Template of a file name with path for replacings.
    $file_pattern = '%type/%id/decorator/%decorator/%app_name.%id.%type.%decorator';
    $file_search = array('%type', '%id', '%decorator', '%app_name');
    foreach ($decorators as $did => $decorator) {
      $decorator_lower = strtolower($decorator);
      // Switch to determine where to find a necessary file.
      $file_replace = array($type_lower, $id_lower, $decorator_lower, $app_name_lower);
      $file = str_replace($file_search, $file_replace, $file_pattern);
      // Include necessary file.
      module_load_include('inc', $app_name_lower, $file);
      $decorator_class = $class . $decorator;
      $component = new $decorator_class($decorators_applied, $relations, $component);
      unset($decorators_applied[$did]);
    }
    return $component;
  }
}

interface IApplication {
  public function getControllers();
  public function getAdminForm($form, &$form_state, $form_name);
  public function submitAdminForm($form, &$form_state);
}


/**
 * @todo Desc for Application.
 */
abstract class Application extends Component implements IApplication {
  // The base path for all configurations.
  const ADMIN_PATH = 'admin';
  protected $type = 'Application';
  // A list of controller names to be initialize at the first launch.
  protected $ctrls = array();
  // Controllers to manipulate the parts of the application as a whole.
  protected $controllers = array();

  public function __construct($decorators_applied = array()) {
    parent::__construct($decorators_applied);
    $this->getControllers();
  }

  public function getControllers() {
    if (empty($this->controllers)) {
      foreach ($this->ctrls as $ctrl_name) {
        $this->controllers[$ctrl_name] = $this->getController($ctrl_name);
      }
    }
    return $this->controllers;
  }

  protected function getController($ctrl_name) {
    if (empty($this->controllers[$ctrl_name])) {
      $controller = ComponentFactory::get($this->app_name, 'Controller', $ctrl_name, $this->decorators_applied, $this->relations);
      $this->controllers[$ctrl_name] = $controller;
    }
    return $this->controllers[$ctrl_name];
  }

  public function getAdminForm($form, &$form_state, $form_name) {
    $app_name = strtolower($this->app_name);
    // @todo Refactor this switch stuff with classes since they are similar a lot.
    switch ($form_name) {
      case 'general':
        // Development fieldset.
        $form['development'] = array(
          '#type' => 'fieldset',
          '#title' => t('Development'),
          '#description' => t('In fact you should change the state of these settings only for development purposes.'),
        );
        // Decorator fieldset.
        $form['development']['decorators'] = array(
          '#type' => 'fieldset',
          '#title' => t('Decorators'),
          '#description' => t('Features that extend usual behavior of the module. For more details see <a href="@decorator_link">Decorator pattern</a> article.',
            array(
              '@decorator_link' => url("http://en.wikipedia.org/wiki/Decorator_pattern"),
            )
          ),
        );
        // Logger decorator.
        $form['development']['decorators']['logger'] = array(
          '#type' => 'checkbox',
          '#title' => t('Logger'),
          '#description' => t('Enable to get full log of all application method calls as a file.'),
          '#default_value' => in_array('Logger', (array) unserialize(variable_get("{$app_name}_decorators"))),
        );
        // Cacher decorator.
        $form['development']['decorators']['cacher'] = array(
          '#disabled' => TRUE,
          '#type' => 'checkbox',
          '#title' => t('Cacher'),
          '#description' => t('Speeds up each method call by using multi-level caching.') . ' ' . $this->getStubText(1870060, 'moopapi'),
          '#default_value' => in_array('Cacher', (array) unserialize(variable_get("{$app_name}_decorators"))),
        );
        break;
    }
    return $form;
  }

  public function submitAdminForm($form, &$form_state) {
    $form_name = $form['#form_name']['#value'];
    // Decorators handling.
    switch ($form_name) {
      case 'general':
        // @todo Refactor it.
        $decs = array(
          'logger' => 'Logger',
          'cacher' => 'Cacher',
        );
        $app_name = strtolower($this->app_name);
        $decorators = array();
        foreach ($decs as $field_name => $decorator_id) {
          if ($form_state['values'][$field_name]) {
            $decorators[$field_name] = $decorator_id;
          }
        }
        variable_set("{$app_name}_decorators", serialize($decorators));
    }
  }

  protected function getStubText($drupal_nid, $app_name = NULL) {
    $app_name = !empty($app_name) ? $app_name : strtolower($this->app_name);
    return t('This functionality is currently in development. See <a href="@issue_link">related issue</a>. Please consider participating in <a href="@patchranger_link">patch crowd funding of this issue</a>. Read more about patch crowd funding on <a href="@project_link">the module\'s project page</a>.',
      array(
        '@issue_link' => url("http://drupal.org/node/{$drupal_nid}"),
        '@patchranger_link' => url("http://www.patchranger.com/?do_nid={$drupal_nid}"),
        '@project_link' => url("http://drupal.org/project/{$app_name}#how-much-does-it-cost"),
      ));
  }
}


abstract class Controller extends Component {
  protected $type = 'Controller';
  protected $model;

  public function __construct($decorators_applied = array(), &$relations = array()) {
    parent::__construct($decorators_applied, $relations);
    $this->getModel();
  }

  protected function getModel() {
    $this->model = ComponentFactory::get($this->app_name, 'Model', $this->controller_type, $this->decorators_applied, $this->relations);
    return $this->model;
  }
}


// @todo Real Model interface.
interface IModel {}

abstract class Model extends Component implements IModel {
  protected $type = 'Model';

  public function __construct($decorators_applied = array(), &$relations = array()) {
    parent::__construct($decorators_applied, $relations);
  }
}


/**
 * Decorator.
 * @see http://en.wikipedia.org/wiki/Decorator_pattern
 */
abstract class Decorator extends Component {
  /**
   * Here the original value of decorated application is stored.
   * @var IApplication 
   */
  protected $original;

  /**
   * @param IApplication $app
   */
  public function __construct($decorators_applied = array(), &$relations = array(), $app) {
    parent::__construct($decorators_applied, $relations);
    $this->original = $app;
  }
}

/* @todo Review.
interface IRelationController {
  public function getRelations();
  public function getRelation($row);
  public function setRelation($row);
  public function save($relation);
  public function delete($relation);
}

abstract class RelationController implements IRelationController {
  public $table;
  public $table_alias;
  public $relations = array();

  public function getRelations($reset = FALSE) {
    if ($reset) {
      $rtlns = db_select($this->table, $this->table_alias)
        ->fields($this->table_alias)
        ->execute();
      while ($row = $rtlns->fetchObject()) {
        $this->setRelation($row);
      }
    }
    return $this->relations;
  }

  public function getRelation($row) {
    $rowId = $this->getRowId($row);
    if (empty($this->relations[$rowId])) {
      $this->getRelations(TRUE);
    }
    return !empty($this->relations[$rowId]) ? $this->relations[$rowId] : NULL;
  }

  public function setRelation($row) {
    $rowId = $this->getRowId($row);
    $this->relations[$rowId] = $row;
  }
}

interface IRelation {
  public function __construct($row);
}

abstract class Relation implements IRelation {

  public function __construct($row) {}
}
 *
 */