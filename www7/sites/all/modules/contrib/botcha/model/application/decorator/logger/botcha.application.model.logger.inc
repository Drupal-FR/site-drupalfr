<?php

/**
 * @file
 * Contains BotchaModelLogger class.
 *
 * Decorator implementation for Botcha application model.
 */

class BotchaModelLogger extends ModelLogger implements IBotchaModel {

  protected $app_name = 'Botcha';

  public function getFormsRecipebooks($parameters = array()) { return $this->logCall('getFormsRecipebooks', $parameters); }
  public function getRecipebooksForms($parameters = array()) { return $this->logCall('getRecipebooksForms', $parameters); }
  public function getRecipesRecipebooks($parameters = array()) { return $this->logCall('getRecipesRecipebooks', $parameters); }
  public function getRecipebooksRecipes($parameters = array()) { return $this->logCall('getRecipebooksRecipes', $parameters); }
}