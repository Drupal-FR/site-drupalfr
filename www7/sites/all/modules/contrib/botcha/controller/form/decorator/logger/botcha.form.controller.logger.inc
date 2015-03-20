<?php

/**
 * @file
 * Contains BotchaFormControllerLogger class.
 *
 * Decorator implementation for BotchaForm objects controller.
 */

class BotchaFormControllerLogger extends ControllerLogger implements IBotchaFormController {
  protected $app_name = 'Botcha';
  protected $controller_type = Botcha::CONTROLLER_TYPE_FORM;

  public function getForm($form_id, $create = TRUE) { return $this->logCall('getForm', array($form_id, $create)); }
  public function getForms($reset = FALSE) { return $this->logCall('getForms', array($reset)); }
  public function save($form) { return $this->logCall('save', array($form)); }
  public function delete($form) { return $this->logCall('delete', array($form)); }
}