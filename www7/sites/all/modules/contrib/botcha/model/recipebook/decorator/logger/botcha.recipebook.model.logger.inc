<?php

/**
 * @file
 * Contains BotchaRecipebookModelLogger class.
 *
 * Decorator implementation for BotchaRecipebook objects model.
 */

class BotchaRecipebookModelLogger extends ModelLogger implements IBotchaRecipebookModel {
  protected $rtlns = array(BotchaModel::RELATION_FORM_RECIPEBOOK, BotchaModel::RELATION_RECIPE_RECIPEBOOK);

  public function getRecipebooks($parameters = array()) { return $this->logCall('getRecipebooks', array($parameters)); }
  public function getRecipebook($id = 'default') { return $this->logCall('getRecipebook', array($id)); }
  public function save($recipebook) { return $this->logCall('save', array($recipebook)); }
  public function delete($recipebook) { return $this->logCall('delete', array($recipebook)); }
}