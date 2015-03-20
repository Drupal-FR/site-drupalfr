<?php

/**
 * @file
 * Contains BotchaModel class.
 *
 * Model layer of the BOTCHA application.
 */

/* @todo Refactor: replace static calls to BotchaModel with these sexy relations.
// We have not done it yet, because we need something like "View" in MVC-paradigm:
// an array of relations of each type must have an interface to switch from one
// representation to another. For example, BotchaFormRecipebookRelation could be
// used either by Form endpoint or Recipebook: in any case it has to provide an
// appropriate array, keyed either with form_id or rbid.
interface IBotchaFormRelation {
  public function setForm($form);
  public function unsetForm($form);
}

interface IBotchaRecipeRelation {
  public function setRecipe($recipe);
  public function unsetRecipe($recipe);
}

interface IBotchaRecipebookRelation {
  public function setRecipebook($recipebook);
  public function unsetRecipebook($recipebook);
}

class BotchaFormRecipebookRelation extends Relation implements IBotchaFormRelation, IBotchaRecipebookRelation {
  // @todo Remove hardcode.
  public $table = 'botcha_recipebook_form';
  public $table_alias = 'brbf';

  // @todo Refactor relation making it depending on schema definition: if each non-PK field is empty then delete, else - merge. It will let move it to Relation class - so no need to redeclare it per concrete relation.
  public function save() {
    if (empty($this->row->rbid)) {
      // Delete row.
      db_delete($this->table)
        // @todo Remove hardcode.
        ->condition('form_id', $this->row->form_id)
        ->execute();
    }
    else {
      // Merge (update if exists else insert).
      db_merge($this->table)
        ->key(array('form_id' => $this->row->form_id))
        ->fields(array('rbid' => $this->row->rbid))
        ->execute();
    }
  }

  public function setForm($form) {
    $this->form = $form;
  }

  public function unsetForm($form) {
    unset($this->form);
  }

  public function setRecipebook($recipebook) {
    $this->recipebook = $recipebook;
  }

  public function unsetRecipebook($recipebook) {
    unset($this->recipebook);
  }
}

class BotchaRecipeRecipebookRelation extends Relation implements IBotchaRecipeRelation, IBotchaRecipebookRelation {
  // @todo Remove hardcode.
  public $table = 'botcha_recipebook_recipe';
  public $table_alias = 'brbr';

  // @todo Refactor relation making it depending on schema definition: if each non-PK field is empty then delete, else - merge. It will let move it to Relation class - so no need to redeclare it per concrete relation.
  public function save() {
    if (empty($this->row->rbid)) {
      // Delete row.
      db_delete($this->table)
        // @todo Remove hardcode.
        ->condition('form_id', $this->row->form_id)
        ->execute();
    }
    else {
      // Merge (update if exists else insert).
      db_merge($this->table)
        ->key(array('form_id' => $this->row->form_id))
        ->fields(array('rbid' => $this->row->rbid))
        ->execute();
    }
  }

  public function setRecipe($recipe) {
    $this->recipe = $recipe;
  }

  public function unsetRecipe($recipe) {
    unset($this->recipe);
  }

  public function setRecipebook($recipebook) {
    $this->recipebook = $recipebook;
  }

  public function unsetRecipebook($recipebook) {
    unset($this->recipebook);
  }
}
 *
 */

interface IBotchaModel {
  public static function getFormsRecipebooks($parameters = array());
  public static function getRecipebooksForms($parameters = array());
  public static function getRecipesRecipebooks($parameters = array());
  public static function getRecipebooksRecipes($parameters = array());
}

class BotchaModel extends Model implements IBotchaModel {
  const RELATION_FORM_RECIPEBOOK = 'BotchaFormRecipebookRelation';
  const RELATION_RECIPE_RECIPEBOOK = 'BotchaRecipeRecipebookRelation';
  protected $app_name = 'Botcha';

  /**
   * Just an alias for getRecipebooksForms.
   * @param array $parameters
   * @return array
   */
  public static function getFormsRecipebooks($parameters = array()) {
    // @todo Remove hardcode.
    $parameters['mode'] = !empty($parameters['mode']) ? $parameters['mode'] : 'recipebook';
    return self::getRecipebooksForms($parameters);
  }

  public static function getRecipebooksForms($parameters = array()) {
    $fields = array();
    switch ($parameters['mode']) {
      case 'form':
        $fields[] = 'form_id';
        break;
      case 'recipebook':
      default:
        $fields[] = 'rbid';
        break;
    }
    $rbf = db_select('botcha_recipebook_form', 'brf')
      ->fields('brf', $fields);
    if (!empty($parameters['recipebooks'])) {
      $rbf->condition('rbid', (array)$parameters['recipebooks'], 'IN');
    }
    if (!empty($parameters['forms'])) {
      $rbf->condition('form_id', (array)$parameters['forms'], 'IN');
    }
    // Catching of PDOException helps to avoid WSOD during update use case. The
    // reason is in that form_alter is called before performing an update.
    // @see http://drupal.org/node/1828710
    try {
      $result = $rbf->execute()->fetchCol();
    } catch (Exception $e) {
      if ($e instanceof PDOException) {
        watchdog_exception('BOTCHA', $e, 'Please perform an update via update.php or reinstall the BOTCHA module to fix the reason of this warning! %type: !message in %function (line %line of %file).', array(), WATCHDOG_WARNING);
        $result = array();
      }
    }
    return $result;
  }

  public static function getRecipesRecipebooks($parameters = array()) {
    // @todo Remove hardcode.
    $parameters['mode'] = !empty($parameters['mode']) ? $parameters['mode'] : 'recipebook';
    return self::getRecipebooksRecipes($parameters);
  }

  public static function getRecipebooksRecipes($parameters = array()) {
    $fields = array();
    switch ($parameters['mode']) {
      case 'recipe':
        $fields[] = 'recipe_id';
        break;
      case 'recipebook':
      default:
        $fields[] = 'rbid';
        break;
    }
    $rbr = db_select('botcha_recipebook_recipe', 'brr')
      ->fields('brr', $fields);
    if (!empty($parameters['recipebooks'])) {
      $rbr->condition('rbid', (array)$parameters['recipebooks'], 'IN');
    }
    if (!empty($parameters['recipes'])) {
      $rbr->condition('recipe_id', (array)$parameters['recipes'], 'IN');
    }
    // Catching of PDOException helps to avoid WSOD during update use case. The
    // reason is in that form_alter is called before performing an update.
    // @see http://drupal.org/node/1828710
    try {
      $result = $rbr->execute()->fetchCol();
    } catch (Exception $e) {
      if ($e instanceof PDOException) {
        watchdog_exception('BOTCHA', $e, 'Please perform an update via update.php or reinstall the BOTCHA module to fix the reason of this warning! %type: !message in %function (line %line of %file).', array(), WATCHDOG_WARNING);
        $result = array();
      }
    }
    return $result;
  }
}
