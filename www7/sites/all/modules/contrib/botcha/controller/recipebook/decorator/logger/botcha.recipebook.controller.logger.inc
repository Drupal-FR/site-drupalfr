<?php

/**
 * @file
 * Contains BotchaRecipebookControllerLogger class.
 *
 * Decorator implementation for BotchaRecipebook objects controller.
 */

class BotchaRecipebookControllerLogger extends ControllerLogger implements IBotchaRecipebookController {
  protected $app_name = 'Botcha';
  protected $controller_type = Botcha::CONTROLLER_TYPE_RECIPEBOOK;

  public function getRecipebooks($reset = FALSE) { return $this->logCall('getRecipebooks', array($reset)); }
  public function getRecipebook($id = 'default', $create = TRUE) { return $this->logCall('getRecipebook', array($id, $create)); }
  public function save($recipebook) { return $this->logCall('save', array($recipebook)); }
  public function delete($recipebook) { return $this->logCall('delete', array($recipebook)); }
}