<?php

/**
 * @file
 * Controller layer of the BotchaRecipebook objects.
 */

interface IBotchaRecipebookController {
  public function getRecipebook($id = 'default', $create = TRUE);
  public function getRecipebooks($reset = FALSE);
  public function save($recipebook);
  public function delete($recipebook);
}


class BotchaRecipebookController extends Controller implements IBotchaRecipebookController {
  protected $app_name = 'Botcha';
  protected $controller_type = Botcha::CONTROLLER_TYPE_RECIPEBOOK;

  /**
   * Just for IDE autocomplete feature.
   * @return BotchaRecipebookModel
   */
  protected function getModel() {
    return parent::getModel();
  }

  public function getRecipebook($id = 'default', $create = TRUE) {
    $none = TRUE;
    if ($id != 'none') {
      $rb = $this->getModel()->getRecipebook($id);
      if ($rb || $create) {
        $none = FALSE;
      }
    }
    if ($none) {
      $recipebook = new BotchaRecipebookNone($id);
    }
    else {
      $recipebook = new BotchaRecipebook($id);
      if ($rb) {
        $recipebook->setTitle($rb->title)
          ->setDescription($rb->description);
      }
      $recipebook->getForms();
      $recipebook->getRecipes();
    }
    return $recipebook;
  }

  public function getRecipebooks($reset = FALSE, $withNone = FALSE) {
    // @todo Pass parameters accurately (add reset).
    $recipebooks = $this->getModel()->getRecipebooks();
    if ($withNone) {
      // @todo Remove hardcode.
      $recipebook_none = $this->getRecipebook('none');
      $recipebooks[$recipebook_none->id] = $recipebook_none;
    }
    return $recipebooks;
  }

  public function save($recipebook) {
    // Save recipe book to DB.
    $this->getModel()->save($recipebook);
    // Return updated object to check results if necessary.
    return $this->getRecipebook($recipebook->id, FALSE);
  }

  public function delete($recipebook) {
    // Delete recipe book from DB.
    $this->getModel()->delete($recipebook);
  }
}


class BotchaRecipebook {

  /**
   * Identifier of the recipe book.
   */
  public $id;

  /**
   * A title of the book.
   */
  public $title;

  /**
   * Description of the recipe book.
   */
  public $description;

  /**
   * List of recipe ids.
   * It is not set by default to let the application to determine whether it is
   * not set yet or it is set and empty.
   * @var BotchaRecipe
   */
  //protected $recipes = array();
  protected $recipes;

  /**
   * List of form ids.
   * It is not set by default to let the application to determine whether it is
   * not set yet or it is set and empty.
   * @var BotchaForm
   */
  //protected $forms = array();
  protected $forms;

  public function __construct($id) {
    $this->id = $id;
  }

  public function isApplicable($form, $form_state) {
    $form_id = $form['form_id']['#value'];
    $isApplicable = FALSE;
    if (!user_access('skip BOTCHA')) {
      $isApplicable = TRUE;
    }
    switch ($form_id) {
      case 'user_register':
        // Only change the registration form. There is also 'user_register' form
        // at /admin/config/people/user/create path, but we leave it alone.
        if (FALSE === strpos($form['#action'], 'user/register')) {
          if (!variable_get('botcha_allow_on_admin_pages', FALSE)) {
            $isApplicable = FALSE;
          }
        }
        break;
    }
    return $isApplicable;
  }

  public function setTitle($title) {
    $this->title = $title;
    return $this;
  }

  public function getTitle() {
    return $this->title;
  }

  public function setDescription($description) {
    $this->description = $description;
    return $this;
  }

  public function getDescription() {
    return $this->description;
  }

  public function setRecipe($recipe_id) {
    $this->recipes[$recipe_id] = $recipe_id;
    return $this;
  }

  public function unsetRecipe($recipe_id) {
    unset($this->recipes[$recipe_id]);
    return $this;
  }

  /**
   * @todo BotchaRecipebook getRecipes Description.
   * @return array
   */
  public function getRecipes() {
    if (!isset($this->recipes)) {
      $rs = BotchaModel::getRecipebooksRecipes(array(
        'mode' => 'recipe',
        'recipebooks' => $this->id,
      ));
      foreach ($rs as $recipe_id) {
        $this->setRecipe($recipe_id);
      }
    }
    return (array)$this->recipes;
  }

  public function setForm($form_id) {
    $this->forms[$form_id] = $form_id;
    return $this;
  }

  public function unsetForm($form_id) {
    unset($this->forms[$form_id]);
    return $this;
  }

  /**
   * @todo BotchaRecipebook getForms Description.
   * @return BotchaForm
   */
  public function getForms() {
    if (!isset($this->forms)) {
      $fs = BotchaModel::getRecipebooksForms(array(
        'mode' => 'form',
        'recipebooks' => $this->id,
      ));
      foreach ($fs as $form_id) {
        $this->setForm($form_id);
      }
    }
    return (array)$this->forms;
  }

  /**
   * Handle form depending on the result of spam check.
   *
   * @param string $result
   *   This parameter is string and not boolean to have a chance to easily implement
   *   new results of spam check (such as 'postponed', 'suspected' or other).
   * @param array $form
   * @param array $form_state
   * @param array $is_spam
   */
  public function handle($result, $form, $form_state, $is_spam) {
    $recipes_spam = array_intersect($is_spam, array_fill_keys(array_keys($is_spam), TRUE));
    $recipes_spam_count = count($recipes_spam);
    $recipes_success = array_intersect($is_spam, array_fill_keys(array_keys($is_spam), FALSE));
    $recipes_success_count = count($recipes_success);
    // !!~ @todo Recipebook handle Reduce code duplication.
    switch ($result) {
      case 'success':
        variable_set('botcha_form_passed_counter', $recipes_success_count);
        // Show good submissions in log.
        if (BOTCHA_LOGLEVEL >= 3) {
          watchdog(BOTCHA_LOG,
            '!form_id post approved by BOTCHA.!more',
            array(
              '!form_id' => $form['form_id']['#value'],
              '!more' => ''
                . ((BOTCHA_LOGLEVEL >= 3) ? ' Checked ' . count($this->getRecipes()) . ' botchas (' . join(', ', $this->getRecipes()) . ').' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'POST=<pre>' . print_r(_botcha_filter_value($_POST), 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'GET=<pre>' . print_r(_botcha_filter_value($_GET), 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'SERVER=<pre>' . print_r($_SERVER, 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . ' form=<pre>' . print_r(_botcha_filter_form_log($form), 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . ' values=<pre>' . print_r(_botcha_filter_value($form_state['values']), 1) . '</pre>' : ''),
            ),
            WATCHDOG_INFO
          );
        }
        $rules_event_name = 'botcha_form_approved';
        break;
      case 'spam':
      default:
        variable_set('botcha_form_blocked_counter', $recipes_spam_count);
        // Show blocked submissions in log.
        // @todo Turn logging into a Rules action.
        if (BOTCHA_LOGLEVEL >= 1) {
          watchdog(BOTCHA_LOG,
          '!form_id post blocked by BOTCHA: submission looks like from a spambot.!more',
          array(
            '!form_id' => $form['form_id']['#value'],
            '!more' => ''
              . ((BOTCHA_LOGLEVEL >= 2) ? '<br /><br />' . 'Failed ' . $recipes_spam_count . ' of ' . count($this->getRecipes()) . ' recipes [' . implode(', ', array_keys($recipes_spam)) . '] from "' . $this->id . '" recipe book.' : '')
              . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'POST=<pre>' . print_r(_botcha_filter_value($_POST), 1) . '</pre>' : '')
              . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'GET=<pre>' . print_r(_botcha_filter_value($_GET), 1) . '</pre>' : '')
              . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'SERVER=<pre>' . print_r($_SERVER, 1) . '</pre>' : '')
              . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . ' form=<pre>' . print_r(_botcha_filter_form_log($form), 1) . '</pre>' : '')
              . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . ' values=<pre>' . print_r(_botcha_filter_value($form_state['values']), 1) . '</pre>' : ''),
          ),
          WATCHDOG_WARNING
          );
        }
        $rules_event_name = 'botcha_form_rejected';
        break;
    }
    // Invoke rules event.
    if (module_exists('rules')) {
      $arguments = array(
  //      'form' => &$form,
  //      'form_state' => &$form_state,
        'form_id' => $form['form_id']['#value'],
        'total_recipes' => count($this->getRecipes()),
        'passed_recipes' => $recipes_success_count,
        'passed_recipes_names' => join(', ', array_keys($recipes_success)),
        // !!~ @todo Add last recipe name.
        //'last_recipe_name' => $recipe->name,
        // !!~ @todo Add a reason of fail to rules event invokation.
        //'fail' => $fail,
        'fail' => 'FAIL',
        'failed_field' => 'mail',
        // !!~ @todo Provide an error_text from Recipebook level.
        //'failed_error' => $recipe->error_text,
      );
      // !!? Do we need per recipe rules event invoking?
      rules_invoke_event_by_args($rules_event_name, $arguments);
    }
  }

  /**
   * Spam check.
   * Currently the logic is as follows: if we could find a recipe that failed
   * spam check - then we consider this form submission as spam and decline it.
   *
   * @param array $form
   * @param array $form_state
   * @param array $is_spam
   * @return boolean
   */
  public function isSpam($form, $form_state, $is_spam) {
    // Consider form submission as a spam if at least one recipe considered it
    // as a spam.
    return count($is_spam) ? (boolean)(array_diff($is_spam, array_fill(0, count($is_spam), FALSE))) : FALSE;
  }

  public function apply(&$form, &$form_state) {
    // @todo Abstract it.
    // '#input'=1 hacks FAPI to call #process handler on the form.
    //$form += array('#input' => TRUE);
    //$form['#process'][] = 'botcha_fprocess';
    $form_state['no_cache'] = TRUE;
    // User_login forms open session in validate hooks instead of submit,
    // we should be the first to validate - add our hook to the beginning
    if (is_array($form['#validate'])) {
      // Workaround since array_unshift'ing by reference was deprecated.
      // @link http://www.php.net/manual/en/function.array-unshift.php#40270 @endlink
      array_unshift($form['#validate'], '');
      $form['#validate'][0] = 'botcha_formValidate';
    }
    else {
      $form['#validate'] = array('botcha_formValidate');
    }
    // @todo ?Do we need it?
    $form_state['#botcha'] = $this->id;
  }
}


/**
 * Dummy class, created for data consistency and for interface unifying.
 * When there is no recipe book binded to form, this class is used as a handler.
 * It has no logic at all - by design.
 */
class BotchaRecipebookNone extends BotchaRecipebook {
  // @todo Refactor this since it is duplication.
  public function __construct($id = NULL) {
    $this->id = !empty($id) ? $id : 'none';
    $this->setTitle('None');
    $this->setDescription('Help class: "Null object" pattern.');
  }
}
