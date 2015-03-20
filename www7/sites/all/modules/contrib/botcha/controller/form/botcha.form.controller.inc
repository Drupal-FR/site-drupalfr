<?php

/**
 * @file
 * Controller layer of the BotchaForm objects.
 */

interface IBotchaFormController {
  public function getForm($form_id, $create = TRUE);
  public function getForms($reset = FALSE);
  public function save($form);
  public function delete($form);
}

/**
 * Special class to abstract operations with form_id.
 * It looks like additional layer of abstraction after DatabaseAbstractionLayer.
 * It helps us to get necessary data - while we don't have to repeat ourselves
 * in many places, writing queries to the database. All logic is hidden under
 * this simple interface: getForm, getRecipebook, getRecipe, etc.
 */
class BotchaFormController extends Controller implements IBotchaFormController {
  protected $app_name = 'Botcha';
  protected $controller_type = Botcha::CONTROLLER_TYPE_FORM;

  /**
   * Just for IDE autocomplete feature.
   * @return BotchaFormModel
   */
  protected function getModel() {
    return parent::getModel();
  }

  /**
   * Gets a form from cache. If it does not exists in cache - gets from
   * database. If it does not exists there also - returns new form or
   * BotchaFormNone depending on input parameter.
   * @param string $form_id
   * @param boolean $create
   *   Determines should we construct new form or return NULL if it does not exist.
   * @return BotchaForm
   */
  public function getForm($form_id, $create = TRUE) {
    $none = TRUE;
    // Respect form exceptions (done by forbidden_forms recipe book).
    //if (!in_array($form_id, array('user_login', 'user_login_block', 'update_script_selection_form'))) {
      $form = $this->getModel()->getForm($form_id);
      if ($form || $create) {
        $none = FALSE;
      }
    //}
    if ($none) {
      $botcha_form = new BotchaFormNone($form_id);
    }
    else {
      $botcha_form = new BotchaForm($form_id);
      $botcha_form->getRecipebook();
    }
    return $botcha_form;
  }

  /**
   * Get a list of available BOTCHA form objects.
   * @todo Respect reset (?via Cacher?)
   * @param boolean $reset
   * @return BotchaForm
   */
  public function getForms($reset = FALSE) {
    $forms = array();
    foreach ($this->getModel()->getForms() as $form) {
      $forms[$form->id] = $this->getForm($form->id, FALSE);
    }
    return $forms;
  }

  // @todo ?Should we separate BotchaForm and BotchaFormAbstract?
  public function save($form) {
    // Save form to DB.
    $this->getModel()->save($form);
    // Return updated object to check results if necessary.
    return $this->getForm($form->id, FALSE);
  }

  public function delete($form) {
    // Save form to DB.
    $this->getModel()->delete($form);
  }
}


class BotchaForm {
  protected $recipebook;

  public function __construct($form_id) {
    $this->id = $form_id;
    $this->form_id = $form_id;
  }

  public function setRecipebook($rbid) {
    $this->recipebook = $rbid;
    return $this;
  }

  function unsetRecipebook() {
    unset($this->recipebook);
    return $this;
  }

  /**
   * @todo BotchaForm getRecipebook Description.
   * @return BotchaRecipebook
   */
  public function getRecipebook() {
    if (!isset($this->recipebook)) {
      $rbs = BotchaModel::getRecipebooksForms(array(
        'mode' => 'recipebook',
        'forms' => $this->id,
      ));
      // In fact there is not more than 1 item.
      // @todo Remove hardcode.
      $this->recipebook = !empty($rbs) ? current($rbs) : 'none';
    }
    return $this->recipebook;
  }

  public function isEnabled() {
    $form_id = $this->id;
    $isEnabled = variable_get("botcha_enabled_{$form_id}", 0);
    return $isEnabled;
  }

  public function setEnabled($enabled) {
    $form_id = $this->id;
    // Cast to boolean first.
    $enabled = (bool)$enabled;
    // Cast to integer.
    $enabled = (int)$enabled;
    variable_set("botcha_enabled_{$form_id}", $enabled);
    return $this;
  }

  public function addAdminLinks(&$form) {
    $form_id = $form['form_id']['#value'];
    if (variable_get('botcha_administration_mode', FALSE)
      && user_access('administer BOTCHA settings')
      && (arg(0) != 'admin'
        || variable_get('botcha_allow_on_admin_pages', FALSE)
        || ($form_id == 'user_register'))
      && $form_id != 'update_script_selection_form') {
        // Add BOTCHA administration tools.
        $botcha_element = $this->createAdminLinksFieldset($form_id);
        // Get placement in form and insert in form.
        // @todo Make away with a dependency from botcha.inc.
        $botcha_placement = _botcha_get_botcha_placement($form_id, $form);
        _botcha_insert_botcha_element($form, $botcha_placement, $botcha_element);
    }
  }

  protected function createAdminLinksFieldset($form_id) {
    // For administrators: show BOTCHA info and offer link to configure it.
    $botcha_element = array(
      '#type' => 'fieldset',
      '#title' => t('BOTCHA'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      // @todo Abstract it.
      //'#attributes' => array('class' => 'botcha-admin-links'),
      '#attributes' => array('class' => array('botcha-admin-links')),
    );
    $rbid = $this->getRecipebook();
    // @todo Remove hardcode.
    if ($rbid === 'none') {
      $botcha_element['#title'] = t('BOTCHA: no protection enabled');
      $botcha_element['add_botcha'] = array(
        // @todo Abstract it.
        //'#value' => l(t('Add BOTCHA protection on form'), Botcha::ADMIN_PATH . "/form/add", array('query' => drupal_get_destination() . "&botcha_form_id=$form_id", 'html' => TRUE)),
        '#markup' => l(t('Add BOTCHA protection on form'), Botcha::ADMIN_PATH . "/form/add", array('query' => array_merge(drupal_get_destination(), array('botcha_form_id' => $form_id)), 'html' => TRUE)),
      );
    }
    else {
      $botcha_element['#title'] = t('BOTCHA: protection enabled (@recipebook recipe book)', array('@recipebook' => $rbid));
      $botcha_element['#description'] = t('Untrusted users will have form %form_id protected by BOTCHA (!recipebook_settings, !general_settings).',
        array(
          '%form_id' => $form_id,
          '!recipebook_settings' => l(t('Recipe book settings'), Botcha::ADMIN_PATH . "/recipebook/{$rbid}"),
          '!general_settings' => l(t('General BOTCHA settings'), Botcha::ADMIN_PATH),
        )
      );
      $botcha_element['protection'] = array(
        '#type' => 'item',
        '#title' => t('Enabled protection'),
        // @todo Abstract it.
        //'#value' => t('Form is protected by "@recipebook" recipe book (!edit, !disable)', array(
        '#markup' => t('Form is protected by "@recipebook" recipe book (!edit, !disable)', array(
          '@recipebook' => $rbid,
          '!edit' => l(t('edit'), Botcha::ADMIN_PATH . "/form/{$form_id}", array('query' => drupal_get_destination(), 'html' => TRUE)),
          '!disable' => l(t('disable'), Botcha::ADMIN_PATH . "/form/{$form_id}/disable", array('query' => drupal_get_destination(), 'html' => TRUE)),
        )),
      );
    }
    return $botcha_element;
  }
}


/**
 * Dummy class, created for data consistency and for interface unifying.
 * When there is no form, this class is used as a handler.
 * It has no logic at all - by design.
 */
class BotchaFormNone extends BotchaForm {
  // @todo Refactor this since it is duplication.
  public function __construct($form_id = NULL) {
    $this->id = !empty($form_id) ? $form_id : 'none';
  }

  public function getRecipebook() {
    // @todo Remove hardcode.
    return 'none';
  }

  public function addAdminLinks(&$form) {
    $form_id = $form['form_id']['#value'];
    // Apply only to allowed forms.
    if (!in_array($form_id, array('update_script_selection_form', 'user_login', 'user_login_block'))) {
      parent::addAdminLinks($form);
    }
  }
}
