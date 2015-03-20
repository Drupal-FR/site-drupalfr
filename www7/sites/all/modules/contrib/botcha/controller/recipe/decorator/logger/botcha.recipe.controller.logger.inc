<?php

/**
 * @file
 * Contains BotchaRecipeControllerLogger class.
 *
 * Decorator implementation for BotchaRecipe objects controller.
 */

class BotchaRecipeControllerLogger extends ControllerLogger implements IBotchaRecipeController {
  protected $app_name = 'Botcha';
  protected $controller_type = Botcha::CONTROLLER_TYPE_RECIPE;

  public function getRecipe($id, $create = TRUE) { return $this->logCall('getRecipe', array($id, $create)); }
  public function getRecipes($reset = FALSE) { return $this->logCall('getRecipes', array($reset)); }
  public function save($recipe) { return $this->logCall('save', array($recipe)); }
  public function delete($recipe) { return $this->logCall('delete', array($recipe)); }
}