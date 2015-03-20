<?php

/**
 * @file
 * Contains BotchaFormModel class.
 *
 * Model layer of the BotchaForm objects.
 */

interface IBotchaFormModel {
  // @todo ? Do we need $create parameter?
  public function getForm($form_id);
  public function getForms($parameters = array());
  public function save($form);
  public function delete($form);
}

class BotchaFormModel extends Model implements IBotchaFormModel {
  protected $rtlns = array(BotchaModel::RELATION_FORM_RECIPEBOOK);

  public function getForms($parameters = array()) {
    $forms = db_select('botcha_form', 'bf')
      ->fields('bf');
    if (!empty($parameters['forms'])) {
      foreach ((array) $parameters['forms'] as $form_id) {
        $forms->condition('id', $form_id, 'IN');
      }
    }
    // Catching of PDOException helps to avoid WSOD during update use case. The
    // reason is in that form_alter is called before performing an update.
    // @see http://drupal.org/node/1828710
    try {
      $result = $forms->execute()->fetchAllAssoc('id');
    } catch (Exception $e) {
      if ($e instanceof PDOException) {
        watchdog_exception('BOTCHA', $e, 'Please perform an update via update.php or reinstall the BOTCHA module to fix the reason of this warning! %type: !message in %function (line %line of %file).', array(), WATCHDOG_WARNING);
        $result = array();
      }
    }
    return $result;
  }

  public function getForm($id) {
    $forms = $this->getForms();
    return !empty($forms[$id]) ? ($forms[$id]) : NULL;
  }

  /**
   * Save form to the database.
   * @param BotchaForm $form
   */
  public function save($form) {
    // Catching of PDOException helps to avoid WSOD during update use case. The
    // reason is in that form_alter is called before performing an update.
    // @see http://drupal.org/node/1828710
    try {
      // Save form.
      $forms = db_select('botcha_form', 'bf')
        ->fields('bf')
        ->condition('id', $form->id)
        ->execute()
        ->fetchCol();
      if (!count($forms)) {
        db_insert('botcha_form')
          ->fields(array('id' => $form->id))
          ->execute();
      }
      // Delete all data related to this form.
      db_delete('botcha_recipebook_form')
        ->condition('form_id', $form->id)
        ->execute();
      $rbid = $form->getRecipebook();
      // @todo Remove hardcode.
      if ($rbid != 'none') {
        // Save form-recipe book relationship.
        db_merge('botcha_recipebook_form')
          ->key(array('form_id' => $form->id))
          ->fields(array(
            'rbid' => $rbid,
          ))->execute();
      }
    } catch (Exception $e) {
      if ($e instanceof PDOException) {
        watchdog_exception('BOTCHA', $e, 'Please perform an update via update.php or reinstall the BOTCHA module to fix the reason of this warning! %type: !message in %function (line %line of %file).', array(), WATCHDOG_WARNING);
      }
    }
  }

  public function delete($form) {
    // Catching of PDOException helps to avoid WSOD during update use case. The
    // reason is in that form_alter is called before performing an update.
    // @see http://drupal.org/node/1828710
    try {
      // Delete all data related to this form.
      db_delete('botcha_recipebook_form')
        ->condition('form_id', $form->id)
        ->execute();
      db_delete('botcha_form')
        ->condition('id', $form->id)
        ->execute();
    } catch (Exception $e) {
      if ($e instanceof PDOException) {
        watchdog_exception('BOTCHA', $e, 'Please perform an update via update.php or reinstall the BOTCHA module to fix the reason of this warning! %type: !message in %function (line %line of %file).', array(), WATCHDOG_WARNING);
      }
    }
  }
}
