<?php

/**
 * @file
 * Contains BotchaFormModelLogger class.
 *
 * Decorator implementation for BotchaForm objects model.
 */

class BotchaFormModelLogger extends ModelLogger implements IBotchaFormModel {
  protected $rtlns = array(BotchaModel::RELATION_FORM_RECIPEBOOK);

  public function getForm($form_id) { return $this->logCall('getForm', array($form_id)); }
  public function getForms($parameters = array()) { return $this->logCall('getForms', array($parameters)); }
  public function save($form) { return $this->logCall('save', array($form)); }
  public function delete($form) { return $this->logCall('delete', array($form)); }
}