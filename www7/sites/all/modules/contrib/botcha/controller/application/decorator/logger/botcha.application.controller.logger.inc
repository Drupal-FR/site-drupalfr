<?php

/**
 * @file
 * Contains BotchaLogger class.
 *
 * Decorator implementation for Botcha application controller.
 */

/**
 * Since we can't use __call here because it can't work with passing by reference,
 * we have to implement each method directly.
 */
/**
 * Since we can't use __call here because it can't work with passing by reference,
 * we have to implement each method directly.
 * @todo Refactor it once we have an application of full value (not just an
 *   adapter for its controller).
 */
class BotchaLogger extends ApplicationLogger implements IBotcha {

  const ADMIN_PATH = 'admin/config/people/botcha';
  // Form ID of comment form on standard (page) node.
  const COMMENT_FORM_ID = 'comment_node_page_form';
  // Controllers.
  const CONTROLLER_TYPE_FORM = 'Form';
  const CONTROLLER_TYPE_RECIPE = 'Recipe';
  const CONTROLLER_TYPE_RECIPEBOOK = 'Recipebook';
  protected $ctrls = array(
    self::CONTROLLER_TYPE_FORM,
    self::CONTROLLER_TYPE_RECIPE,
    self::CONTROLLER_TYPE_RECIPEBOOK
  );
  protected $app_name = 'Botcha';
  /**
   * This implementation is just for IDE autocomplete feature.
   * @return BotchaFormControllerLogger|BotchaRecipeControllerLogger|BotchaRecipebookControllerLogger
   */
  public function getController($ctrl_name) {
    return parent::getController($ctrl_name);
  }

  // Hook implementations.
  // @todo Move logger settings to getAdminForm:general implementation.
  public function getAdminForm($form, &$form_state, $form_name) { return $this->logCall('getAdminForm', array($form, &$form_state, $form_name)); }
  public function submitAdminForm($form, &$form_state) { return $this->logCall('submitAdminForm', array($form, &$form_state)); }
  public function form_alter(&$form, &$form_state, $form_id) { return $this->logCall('form_alter', array(&$form, &$form_state, $form_id)); }
  //public function form_load($form_id) { return $this->logCall('form_load', array($form_id)); }
  public function help($path, $arg) { return $this->logCall('help', array($path, $arg)); }
  public function menu() { return $this->logCall('menu', array()); }
  // @todo Abstract it.
  //public function perm() { return $this->logCall('perm', array()); }
  public function permission() { return $this->logCall('permission', array()); }
  //public function recipebook_load($rbid) { return $this->logCall('recipebook_load', array($rbid)); }
  public function recipebook_title($recipebook) { return $this->logCall('recipebook_title', array($recipebook)); }
  public function theme() { return $this->logCall('theme', array()); }
  // Necessary Botcha functions.
  public function generateSecretKey() { return $this->logCall('generateSecretKey', array()); }
  //public function formAccess($botcha_form) { return $this->logCall('formAccess', array($botcha_form)); }
  public function formExists($value) { return $this->logCall('formExists', array($value)); }
  //public function recipebookAccess($recipebook) { return $this->logCall('recipebookAccess', array($recipebook)); }
  public function recipebookExists($value) { return $this->logCall('recipebookExists', array($value)); }
  public function formValidate($form, &$form_state) { return $this->logCall('formValidate', array($form, &$form_state)); }
}
