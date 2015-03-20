<?php

/**
 * @file
 * Contains BotchaRecipeModelLogger class.
 *
 * Decorator implementation for BotchaRecipe objects model.
 */

class BotchaRecipeModelLogger extends ModelLogger implements IBotchaRecipeModel {
  protected $rtlns = array(BotchaModel::RELATION_RECIPE_RECIPEBOOK);

  public function getRecipe($id) { return $this->logCall('getRecipe', array($id)); }
  public function getRecipes($parameters = array()) { return $this->logCall('getRecipes', array($parameters)); }
  public function save($recipe) { return $this->logCall('save', array($recipe)); }
}