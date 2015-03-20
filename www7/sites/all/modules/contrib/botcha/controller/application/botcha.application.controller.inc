<?php

/**
 * @file
 * Contains Botcha class.
 *
 * Controller layer of the BOTCHA application.
 */

/**
 * Interface of the application includes all its hook implementations and public
 * functions. It is used by decorators that have to implement all of these methods.
 */
interface IBotcha {
  // Hook implementations.
  public function form_alter(&$form, &$form_state, $form_id);
  //public function form_load($form_id);
  public function help($path, $arg);
  public function menu();
  // @todo Abstract it.
  //public function perm();
  public function permission();
  //public function recipebook_load($rbid);
  public function recipebook_title($recipebook);
  //public function theme();
  // Necessary Botcha functions.
  public function generateSecretKey();
  //public function formAccess($botcha_form);
  public function formExists($value);
  //public function recipebookAccess($recipebook);
  public function recipebookExists($value);
  public function formValidate($form, &$form_state);
}

/**
 * Just a middleman for achieving purposes such as:
 * - provide a public interface to be used with IDEs (autocomplete and other stuff);
 * - make it possible to use Decorators which decorate each method calling (Logger, etc.)
 */
class Botcha extends Application implements IBotcha {
  const ADMIN_PATH = 'admin/config/people/botcha';
  // Form ID of comment form on standard (page) node.
  const COMMENT_FORM_ID = 'comment_node_page_form';
  // Controllers.
  const CONTROLLER_TYPE_FORM = 'Form';
  const CONTROLLER_TYPE_RECIPE = 'Recipe';
  const CONTROLLER_TYPE_RECIPEBOOK = 'Recipebook';
  protected $ctrls = array(
    self::CONTROLLER_TYPE_FORM,
    self::CONTROLLER_TYPE_RECIPE,
    self::CONTROLLER_TYPE_RECIPEBOOK
  );
  protected $app_name = 'Botcha';

  /**
   * This implementation is just for IDE autocomplete feature.
   * @return BotchaFormController|BotchaRecipeController|BotchaRecipebookController
   */
  public function getController($ctrl_name) {
    return parent::getController($ctrl_name);
  }

  /**
   * Implements hook_form_alter().
   *
   * This function adds BOTCHA protection to forms for untrusted users if needed and adds
   * BOTCHA administration links for site administrators if this option is enabled.
   */
  public function form_alter(&$form, &$form_state, $form_id) {
    // Get an instance of BOTCHA form controller.
    $form_controller = $this->getController(Botcha::CONTROLLER_TYPE_FORM);
    $botcha_form = $form_controller->getForm($form_id, FALSE);
    // Check if it is allowed to protect.
    if ($botcha_form->isEnabled()) {
      // Add admin links functionality.
      $botcha_form->addAdminLinks($form);
      // Get a recipe book and apply all applicable recipes to the form.
      $rbid = $botcha_form->getRecipebook();
      $recipebook = $this->getController(Botcha::CONTROLLER_TYPE_RECIPEBOOK)->getRecipebook($rbid, FALSE);
      if ($recipebook->isApplicable($form, $form_state)) {
        // Apply each of recipebook's recipes ...
        $recipe_controller = $this->getController(Botcha::CONTROLLER_TYPE_RECIPE);
        foreach ($recipebook->getRecipes() as $recipe_id) {
          $recipe = $recipe_controller->getRecipe($recipe_id, FALSE);
          $recipe->apply($form, $form_state);
          // CSS.
          if ($css = $recipe->getCss()) {
            $csss[] = $css;
          }
          // JS.
          if ($recipe instanceof BotchaRecipeUsingJsAbstract) {
            $jss[] = $recipe->getJsValue();
          }
        }
        // ... and the recipe book itself.
        $recipebook->apply($form, $form_state);
        // Logging.
        // @todo Refactor logging.
        if (BOTCHA_LOGLEVEL >= 4) {
          watchdog(
            BOTCHA_LOG,
            '%form_id form prepared by BOTCHA: added recipes - !botchas!more',
            array(
              '%form_id' => $form['form_id']['#value'],
              '!botchas' => implode(', ', $recipebook->getRecipes()),
              '!more' => ''
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'POST=<pre>' . print_r(_botcha_filter_value($_POST), 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'GET=<pre>' . print_r(_botcha_filter_value($_GET), 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'SERVER=<pre>' . print_r($_SERVER, 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5) ? '<br /><br />' . 'form=<pre>' . print_r(_botcha_filter_form_log($form), 1) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5 && count($jss)) ? '<br /><br />' . 'JS=<pre>' . join("\n", $jss) . '</pre>' : '')
                . ((BOTCHA_LOGLEVEL >= 5 && count($csss)) ? '<br /><br />' . 'CSS=<pre>' . join("\n", $csss) . '</pre>' : ''),
            ),
            WATCHDOG_NOTICE
          );
        }
      }
    }
  }

  /**
   * Implements hook_help().
   */
  public function help($path, $arg) {
    switch ($path) {
      case 'admin/help#botcha':
        $output = '<p>' . t('"BOTCHA" is an acronym for "BOT Computerized Heuristic Analysis". It is a method of protection from automated form submissions by performing analysis of submitted data that determines whether the user is a bot. The BOTCHA module is a tool to fight automated submission by malicious users that utilize automated form submission (e.g. for spamming) of for example comments forms, user registration forms, guestbook forms, etc. BOTCHA inserts elements into the desired forms that will not be shown to normal users. These elements have no impact on humans and require no puzzles to solve, but they are easy enough for automated scripts and spam bots to trip on.') . '</p>';
        return $output;
      case Botcha::ADMIN_PATH:
        $output = t('A BOTCHA protection consists of these parts:
  <ul>
  <li><b>Forms</b>: By default BOTCHA protection is enabled for concrete list of forms. Those forms that are not in the list are not protected. You could manage the list of protected forms on <a href="@forms_page">Forms</a> page.</li>
  <li><b>Recipe books</b>: Recipe books are containers for recipes. You could use them to organize your defense lines against spam. To control your recipe books go to <a href="@recipebooks_page">Recipe books</a> page.</li>
  <li><b>Recipes</b>: Each recipe has its own method to differ a human visitor and a spam bot. Some of them are flexible enough to provide UI for controlling its behavior. See the list of available recipes on <a href="@recipes_page">Recipes</a> page.</li>
  </ul>', array(
            '@forms_page' => url(Botcha::ADMIN_PATH . '/form'),
            '@recipebooks_page' => url(Botcha::ADMIN_PATH . '/recipebook'),
            '@recipes_page' => url(Botcha::ADMIN_PATH . '/recipe')
          )
        );
        return $output;
      case Botcha::ADMIN_PATH . '/form':
        $output = '<p>' . t('A BOTCHA protection can be added to virtually each Drupal form. Some default forms are already provided in the form list and more can be added using form internal name.') . '</p>';
        $output .= '<p>' . t('All existing forms can be easily added and managed when the option "%adminlinks" is enabled.',
          array('%adminlinks' => t('Add BOTCHA administration links to forms'))) . '</p>';
        if (module_exists('captcha')) {
          $output .= '<p>' . t('Other forms will be added automatically based on CAPTCHA settings when the option "%usecaptcha" is enabled.',
            array('%usecaptcha' => t('Add BOTCHA to forms selected for CAPTCHA'))) . '</p>';
        }
        $output .= '<p>' . t('Forms served to users with the "%skipbotcha" <a href="@perm">permission</a> won\'t be protected. Be sure to grant this permission to the trusted users (e.g. site administrators). If you want to test a protected form, be sure to do it as a user without the "%skipbotcha" permission (e.g. as anonymous user).',
          // @todo Abstract it.
          //array('%skipbotcha' => t('skip BOTCHA'), '@perm' => url('admin/user/permissions', array('fragment' => 'module-' . 'botcha')))) . '</p>';
          array('%skipbotcha' => t('skip BOTCHA'), '@perm' => url('admin/people/permissions/list', array('fragment' => 'module-' . 'botcha')))) . '</p>';
        $output .= '<p>' . t('Select which forms to protect with BOTCHA.');
        return $output;
      case Botcha::ADMIN_PATH . '/recipebook':
        $output = t('Recipe book is a glue that connects all the parts. Each recipe book is binded to the forms in one hand and to the recipes - in another.');
        return $output;
    }
  }

  /**
   * Implements hook_menu().
   */
  public function menu() {
    $items = array();
    // Main configuration page of BOTCHA module.
    $items[Botcha::ADMIN_PATH] = array(
      'title' => 'BOTCHA',
      'description' => 'Administer how and where BOTCHAs are used.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('botcha_getAdminForm', 'general'),
      'access arguments' => array('administer BOTCHA settings'),
      'type' => MENU_NORMAL_ITEM,
    );
    $items[Botcha::ADMIN_PATH . '/general'] = array(
      'title' => 'General',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items[Botcha::ADMIN_PATH . '/form'] = array(
      'title' => 'Forms',
      'description' => 'Administer BOTCHA forms.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('botcha_getAdminForm', 'form_list'),
      'access arguments' => array('administer BOTCHA settings'),
      // @todo Abstract it.
      //'type' => MENU_LOCAL_TASK + MENU_NORMAL_ITEM,
      'type' => MENU_LOCAL_TASK,
    );
    $items[Botcha::ADMIN_PATH . '/form/add'] = array(
      'title' => 'Add BOTCHA protection to form',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('botcha_getAdminForm', 'form_edit'),
      'access arguments' => array('administer BOTCHA settings'),
      // @todo Abstract it.
      //'type' => MENU_CALLBACK,
      'type' => MENU_LOCAL_ACTION,
    );
    $items[Botcha::ADMIN_PATH . '/form/%botcha_form'] = array(
      'title' => 'BOTCHA form administration',
      'page callback' => 'drupal_get_form',
      // @todo Abstract it.
      //'page arguments' => array('botcha_getAdminForm', 'form_edit', 4),
      'page arguments' => array('botcha_getAdminForm', 'form_edit', 5),
      'access callback' => 'botcha_form_access',
      // @todo Abstract it.
      //'access arguments' => array(4),
      'access arguments' => array(5),
      'type' => MENU_CALLBACK,
    );
    $items[Botcha::ADMIN_PATH . '/form/%botcha_form/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => 1,
    );
    $items[Botcha::ADMIN_PATH . '/form/%botcha_form/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'drupal_get_form',
      // @todo Abstract it.
      //'page arguments' => array('botcha_getAdminForm', 'form_delete', 4),
      'page arguments' => array('botcha_getAdminForm', 'form_delete', 5),
      'access callback' => 'botcha_form_access',
      // @todo Abstract it.
      //'access arguments' => array(4),
      'access arguments' => array(5),
      'type' => MENU_LOCAL_TASK,
      'weight' => 4,
    );
    $items[Botcha::ADMIN_PATH . '/recipebook'] = array(
      'title' => 'Recipe books',
      'description' => 'Administer BOTCHA recipe books.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('botcha_getAdminForm', 'recipebook_list'),
      'access arguments' => array('administer BOTCHA settings'),
      // @todo Abstract it.
      //'type' => MENU_LOCAL_TASK + MENU_NORMAL_ITEM,
      'type' => MENU_LOCAL_TASK,
    );
    $items[Botcha::ADMIN_PATH . '/recipebook/add'] = array(
      'title' => 'Add recipe book',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('botcha_getAdminForm', 'recipebook_edit'),
      'access arguments' => array('administer BOTCHA settings'),
      // @todo Abstract it.
      //'type' => MENU_CALLBACK,
      'type' => MENU_LOCAL_ACTION,
    );
    $items[Botcha::ADMIN_PATH . '/recipebook/%botcha_recipebook'] = array(
      'title callback' => 'botcha_recipebook_title',
      // @todo Abstract it.
      //'title arguments' => array(4),
      'title arguments' => array(5),
      'page callback' => 'drupal_get_form',
      // @todo Abstract it.
      //'page arguments' => array('botcha_getAdminForm', 'recipebook_edit', 4),
      'page arguments' => array('botcha_getAdminForm', 'recipebook_edit', 5),
      'access callback' => 'botcha_recipebook_access',
      // @todo Abstract it.
      //'access arguments' => array(4),
      'access arguments' => array(5),
      // @todo Abstract it.
      //'type' => MENU_CALLBACK,
      'type' => MENU_NORMAL_ITEM,
    );
    $items[Botcha::ADMIN_PATH . '/recipebook/%botcha_recipebook/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => 1,
    );
    $items[Botcha::ADMIN_PATH . '/recipebook/%botcha_recipebook/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'drupal_get_form',
      // @todo Abstract it.
      //'page arguments' => array('botcha_getAdminForm', 'recipebook_delete', 4),
      'page arguments' => array('botcha_getAdminForm', 'recipebook_delete', 5),
      'access callback' => 'botcha_recipebook_access',
      // @todo Abstract it.
      //'access arguments' => array(4),
      'access arguments' => array(5),
      'type' => MENU_LOCAL_TASK,
      'weight' => 4,
    );
    $items[Botcha::ADMIN_PATH . '/recipe'] = array(
      'title' => 'Recipes',
      'description' => 'Administer BOTCHA recipes.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('botcha_getAdminForm', 'recipe_list'),
      'access arguments' => array('administer BOTCHA settings'),
      // @todo Abstract it.
      //'type' => MENU_LOCAL_TASK + MENU_NORMAL_ITEM,
      'type' => MENU_LOCAL_TASK,
    );
    return $items;
  }

  /**
   * Implements hook_permission().
   */
  public function permission() {
    return array(
      // @todo Abstract it.
      //'administer BOTCHA settings',
      //'skip BOTCHA',
      'administer BOTCHA settings' => array(
        'title' => t('Administer BOTCHA settings'),
      ),
      'skip BOTCHA' => array(
        'title' => t('Skip BOTCHA'),
        'description' => t('Users with this permission will not be subjected to BOTCHA.'),
      ),
    );
  }

  /**
   * Implements hook_theme().
   */
  /* @todo Find a way to move it here, since it needs to be placed separately for #theme magic to work.
   * @see botcha_theme()
  public function theme() {
    return array(
      'botcha_forms_form_botcha_forms' => array(
        'file' => 'botcha.module',
        'render element' => 'form',
      ),
      'botcha_recipebooks_form' => array(
        'file' => 'botcha.module',
        'render element' => 'form',
      ),
    );
  }
   *
   */

  /**
   * Generate a random secret key.
   */
  public function generateSecretKey() {
    return md5(uniqid(mt_rand(), TRUE));
  }

  // @todo Refactor operations: make them just an individual implementation of available operations (access, load, view, edit, delete, etc.)
  public function formExists($value) {
    return !($this->getController(Botcha::CONTROLLER_TYPE_FORM)->getForm($value, FALSE) instanceof BotchaFormNone);
  }

  // @todo Refactor operations: make them just an individual implementation of available operations (access, load, view, edit, delete, etc.)
  public function recipebookExists($value) {
    return !($this->getController(Botcha::CONTROLLER_TYPE_RECIPEBOOK)->getRecipebook($value, FALSE) instanceof BotchaRecipebookNone);
  }

  // @todo Refactor operations: make them just an individual implementation of available operations (access, load, view, edit, delete, etc.)
  /**
   * Title callback for importers.
   */
  public function recipebook_title($recipebook) {
    return "Edit recipe book \"$recipebook->title\"";
  }

  public function formValidate($form, &$form_state) {
    // FIXME: where does this empty value come from ? happens with comments
    unset($form_state['values']['']);
    // Get an instance of BOTCHA form controller.
    $form_controller = $this->getController(Botcha::CONTROLLER_TYPE_FORM);
    $botcha_form = $form_controller->getForm($form['form_id']['#value'], FALSE);
    // Check if it is allowed to protect.
    if ($botcha_form->isEnabled()) {
      // Fetch a recipe book and handle spam protection.
      $recipebook_controller = $this->getController(Botcha::CONTROLLER_TYPE_RECIPEBOOK);
      $rbid = $botcha_form->getRecipebook();
      $recipebook = $recipebook_controller->getRecipebook($rbid, FALSE);
      if ($recipebook->isApplicable($form, $form_state)) {
        // Array to store results of spam checks.
        $is_spam = array();
        $recipe_controller = $this->getController(Botcha::CONTROLLER_TYPE_RECIPE);
        foreach ($recipebook->getRecipes() as $recipe_id) {
          $recipe = $recipe_controller->getRecipe($recipe_id, FALSE);
          $is_spam[$recipe_id] = $recipe->isSpam($form, $form_state);
          $recipe->handle($is_spam[$recipe_id] ? 'spam' : 'success', $form, $form_state);
        }
        // To block or not to block we decide using recipe book logic.
        $recipebook->handle($recipebook->isSpam($form, $form_state, $is_spam) ? 'spam' : 'success', $form, $form_state, $is_spam);
      }
    }
  }

  /**
   * Get admin form.
   */
  // @todo Abstract it.
  //public function getAdminForm(&$form_state, $form_name) {
  public function getAdminForm($form, &$form_state, $form_name) {
    // @todo Abstract it.
    //$form = parent::getAdminForm($form_state, $form_name);
    $form = parent::getAdminForm($form, $form_state, $form_name);
    // It is used on submit stage.
    $form['#form_name'] = array(
      '#type' => 'hidden',
      '#value' => $form_name,
    );
    // @todo Refactor this switch stuff with classes since they are similar a lot.
    switch ($form_name) {
      case 'general':
        // Module settings form.
        // We can't use system_settings_form() here because it will put all extra
        // stuff into variables, that we want to avoid.
        $form['botcha_secret'] = array(
          '#type' => 'textfield',
          '#title' => t('Secret key'),
          '#default_value' => variable_get('botcha_secret', $this->generateSecretKey() ),
          '#description' => t('It is recommended to enter some random text into the secret key. This setting makes your site\'s BOTCHA challenges unique and harder to break.')
            . '<br />' . t('If you leave this field empty and save configuration, a new random key will be generated for you.'),
        );
        // BOTCHA Statistics & Logging
        $form['botcha_statistics'] = array(
          '#type' => 'fieldset',
          '#title' => t('Statistics & logging'),
          '#description' => t('BOTCHA collects statistics of form submissions and it can report different events into the system log.'),
        );
        $dblog_link = l(t('log'), 'admin/reports/dblog');
        // @todo Refactor logging.
        $form['botcha_statistics']['botcha_loglevel'] = array(
          '#type' => 'select',
          '#title' => t('Log level'),
          '#default_value' => variable_get('botcha_loglevel', 2),
          '#options' => array(
            0 => t('0: no log'),
            1 => t('1: blocked/bad submissions only'),
            2 => t('2: ... and why blocked'),
            3 => t('3: ... and good submissions'),
            4 => t('4: ... and protected forms'),
            5 => t('5: ... and extra submission details'),
            6 => t('6: ... and misc development items'),
          ),
          '#description' => t('Select what information to report into the !log.'
            . ' Please note!: Using BOTCHA logging setting could cause at high'
            . ' levels putting vulnerable data into logs. We have some basic'
            . ' escaping (e.g., for password field) - but any other data could'
            . ' be found in raw format. Please be careful with logging level'
            . ' setting!', array('!log' => $dblog_link)),
        );
        // Button for resetting the BOTCHA statistics.
        $form['botcha_statistics']['botcha_statistics_group'] = array(
          '#type' => 'item',
          '#title' => t('BOTCHA statistics'),
          '#description' => t('Reset all accumulated statistics of form submissions.'),
        );
        // Handle the button for resetting the BOTCHA statistics.
        // This is done here instead of in a submit handler because the button is
        // not a submitting button.
        $form['botcha_statistics']['botcha_statistics_group']['botcha_statistics_reset'] = array(
          '#type' => 'button',
          '#value' => t('Reset BOTCHA statistics'),
          '#submit' => array('botcha_statistics_reset'),
          // Pull it down.
          '#weight' => 100,
        );
        // @todo Abstract it.
        //if (isset($form_state['post']['op']) && $form_state['post']['op']
        if (isset($form_state['input']['op']) && $form_state['input']['op']
          == $form['botcha_statistics']['botcha_statistics_group']['botcha_statistics_reset']['#value']) {
          variable_set('botcha_form_passed_counter', 0);
          variable_set('botcha_form_blocked_counter', 0);
          drupal_set_message(t('BOTCHA statistics have been reset.'));
        }
        // Show statistic counters.
        $block_cnt = variable_get('botcha_form_blocked_counter', 0);
        $build_cnt = variable_get('botcha_form_passed_counter', 0) + $block_cnt;
        $form['botcha_statistics']['botcha_statistics_group']['botcha_statistics'] = array(
          '#type' => 'item',
          // @todo Abstarct it.
          //'#value' => format_plural(
          '#markup' => format_plural(
            $block_cnt,
            'Already 1 blocked form submission.',
            'Already @count blocked form submissions.'
          ) . ($build_cnt > 0 ? ' ' . t('(!percent% of total !build_cnt processed)',
            array('!percent' => sprintf("%0.3f", 100 * $block_cnt / $build_cnt), '!build_cnt' => $build_cnt)) : ''),
        );
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Save configuration'),
        );
        $form['#theme'] = 'system_settings_form';
        break;
      case 'form_delete':
        // @todo Abstract it.
        //$botcha_form = func_get_arg(2);
        $botcha_form = func_get_arg(3);
        $form['botcha_form_id'] = array(
          '#type' => 'value',
          '#value' => $botcha_form->id,
        );
        $message = t('Are you sure you want to delete the BOTCHA protection for form_id %form_id?', array('%form_id' => $botcha_form->id));
        // @todo Refactor: remove code duplication (filling in #submit is intended to be done below).
        $form = confirm_form($form, $message, Botcha::ADMIN_PATH, NULL, t('Delete'));
        $form['actions']['submit']['#submit'] = array('Botcha_submitAdminForm');
        return $form;
        break;
      case 'form_edit':
        $args = func_get_args();
        // @todo Abstract it.
        //$botcha_form = (!empty($args[2]) ? $args[2] : NULL);
        $botcha_form = (!empty($args[3]) ? $args[3] : NULL);
        $recipebook_controller = $this->getController(Botcha::CONTROLLER_TYPE_RECIPEBOOK);
        // Determine default values depending on whether we add or edit form.
        $edit = !empty($botcha_form);
        if ($edit) {
          if ($botcha_form instanceof BotchaFormNone) {
            // Redirect in case we are trying to edit unexisting item.
            drupal_goto(Botcha::ADMIN_PATH . '/form/add', array('query' => array('botcha_form_id' => $botcha_form->id)));
          }
          $operation = 'edit';
          $isEnabled = $botcha_form->isEnabled();
          $recipebook = $recipebook_controller->getRecipebook($botcha_form->getRecipebook(), FALSE);
          $botcha_form_id_default = check_plain($botcha_form->id);
          // @todo Abstract it.
          //$validate = array();
          $button = t('Save');
        }
        else {
          $operation = 'add';
          $isEnabled = TRUE;
          // 'default' will be selected.
          $recipebook = $recipebook_controller->getRecipebook();
          // @todo Abstract it.
          $query_params = drupal_get_query_parameters();
          $botcha_form_id_default = !empty($query_params['botcha_form_id']) ? $query_params['botcha_form_id'] : NULL;
          // @todo Abstract it.
          //$validate = array('botcha_formIdValidate');
          $button = t('Add');
        }
        // Insert a field that allows to understand whether it is creation or edition form submission.
        $form['botcha_operation'] = array(
          '#type' => 'value',
          '#value' => $operation,
        );
        // Determine whether the form enabled or not.
        $form['botcha_enabled'] = array(
          '#type' => 'checkbox',
          '#title' => t('Enabled'),
          '#description' => t('Enable BOTCHA protection for the form.'),
          '#default_value' => $isEnabled,
        );
        // Use passed as a parameter form id.
        $form['botcha_form_id'] = array(
          // @todo Abstract it. Perhaps use Elements for adapter to it.
          // @see http://drupal.org/node/995784
          //'#type' => 'textfield',
          //'#element_validate' => $validate,
          //'#required' => TRUE,
          '#title' => t('Form ID'),
          '#description' => t('The Drupal form_id of the form to add the BOTCHA protection to.'),
          '#maxlength' => 128,
        );
        if ($edit) {
          // Change type to textfield to avoid unique checking.
          $form['botcha_form_id']['#type'] = 'textfield';
          $form['botcha_form_id']['#disabled'] =  TRUE;
          // We use #value instead of #default_value as a workaround.
          // @see http://drupal.org/node/1837306
          $form['botcha_form_id']['#value'] = $botcha_form_id_default;
        }
        else {
          $form['botcha_form_id']['#type'] = 'machine_name';
          $form['botcha_form_id']['#default_value'] = $botcha_form_id_default;
          $form['botcha_form_id']['#machine_name']['exists'] = 'Botcha_formExists';
          $form['botcha_form_id']['#required'] = TRUE;
        }
        $options = array();
        $rbs = $recipebook_controller->getRecipebooks(TRUE, TRUE);
        foreach ($rbs as $rb) {
          $options[$rb->id] = $rb->title;
        }
        $form['botcha_form_recipebook'] = array(
          '#type' => 'select',
          '#title' => t('Recipe book'),
          '#description' => t('Recipe book to apply to the form.'),
          '#default_value' => $recipebook->id,
          '#options' => $options,
        );
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => $button,
        );
        break;
      case 'form_list':
        $form['botcha_form_protection'] = array();
        // List known form_ids.
        $form['botcha_form_protection']['botcha_form_id_overview'] = array(
          '#theme' => 'botcha_forms_form_botcha_forms',
          '#tree' => TRUE,
        );
        $form['botcha_form_protection']['botcha_form_id_overview']['#header'] = array(t('Enabled'), 'form_id', t('Recipe book'));
        $form['botcha_form_protection']['botcha_form_id_overview']['botcha_forms'] = array();
        $forms = $this->getController(Botcha::CONTROLLER_TYPE_FORM)->getForms(TRUE);
        foreach ($forms as $botcha_form) {
          // Get individual admin form as a row of our overview table.
          // @todo Abstract it.
          $fake_form = array();
          $fake_form_state = array();
          // @todo Abstract it.
          //$item_form = $this->getAdminForm($fake_form_state, 'form_edit', $botcha_form);
          $item_form = $this->getAdminForm($fake_form, $fake_form_state, 'form_edit', $botcha_form);
          // Remove submit button ...
          unset($item_form['submit']);
          // ... and botcha_operation field ...
          unset($item_form['botcha_operation']);
          // ... and also additional properties of some fields.
          unset($item_form['botcha_enabled']['#title']);
          unset($item_form['botcha_enabled']['#description']);
          $form['botcha_form_protection']['botcha_form_id_overview']['botcha_forms'][$botcha_form->id] = $item_form;
        }
        if (module_exists('captcha')) {
          // Checkbox to put BOTCHA on same forms as CAPTCHA.
          $form['botcha_form_protection']['botcha_on_captcha_forms'] = array(
            '#type' => 'checkbox',
            '#title' => t('Add BOTCHA to forms selected for CAPTCHA'),
            '#default_value' => variable_get('botcha_on_captcha_forms', TRUE),
            '#description' => t('This option makes it easy to manage BOTCHA settings on forms. When enabled, all forms that are configured to have CAPTCHA on them (<a href="@captcha">see configuration</a>) will also be selected for BOTCHA protection.!more',
              array(
                // @todo Abstract it.
                //'@captcha' => url('admin/user/captcha'),
                '@captcha' => url('admin/config/people/captcha'),
                '!more' => module_exists('captcha') ? '' : '<br />' . t('<b>Note:</b> <a href="@captcha_home">CAPTCHA module</a> is not installed. This setting will have no effect.',
                  array('@captcha_home' => 'http://drupal.org/project/captcha')
                ),
              )),
          );
        }
        // Field for the BOTCHA administration mode.
        $form['botcha_form_protection']['botcha_administration_mode'] = array(
          '#type' => 'checkbox',
          '#title' => t('Add BOTCHA administration links to forms'),
          '#default_value' => variable_get('botcha_administration_mode', FALSE),
          '#description' => t('This option makes it easy to manage BOTCHA settings on forms. When enabled, users with the "%adminbotcha" permission will see a fieldset with BOTCHA administration links on all forms, except on administrative pages.', array('%adminbotcha' => t('administer BOTCHA settings'))),
        );
        // Field for the BOTCHAs on admin pages.
        $form['botcha_form_protection']['botcha_allow_on_admin_pages'] = array(
          '#type' => 'checkbox',
          '#title' => t('Allow BOTCHAs and BOTCHA administration links on administrative pages'),
          '#default_value' => variable_get('botcha_allow_on_admin_pages', FALSE),
          '#description' => t('This option makes it possible to add BOTCHAs to forms on administrative pages. BOTCHAs are disabled by default on administrative pages (which shouldn\'t be accessible to untrusted users normally) to avoid the related overhead. In some situations, e.g. in the case of demo sites, it can be usefull to allow BOTCHAs on administrative pages.'),
        );
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Save configuration'),
        );
        // @todo Abstract it.
        // @todo ?Do we need it?
        //$form['#theme'] = 'system_settings_form';
        break;
      case 'recipe_delete':
        // Unused yet.
        // @todo Abstract it.
        //$recipe = func_get_arg(2);
        $recipe = func_get_arg(3);
        break;
      case 'recipe_edit':
        // Unused yet.
        $args = func_get_args();
        // @todo Abstract it.
        //$recipe = (!empty($args[2]) ? $args[2] : NULL);
        $recipe = (!empty($args[3]) ? $args[3] : NULL);
        break;
      case 'recipe_list':
        // @todo Implement Recipe UI.
        // @see https://drupal.org/node/1815080
        $form['stub'] = array(
          // @todo Abstract it.
          //'#value' => $this->getStubText(1815080),
          '#markup' => $this->getStubText(1815080),
        );
        break;
      case 'recipebook_delete':
        // @todo Abstract it.
        //$recipebook = func_get_arg(2);
        $recipebook = func_get_arg(3);
        // @todo Pass just rbid, not the whole object.
        $form['#botcha_recipebook'] = array(
          '#type' => 'value',
          '#value' => $recipebook,
        );
        // @todo Refactor: remove code duplication (filling in #submit is intended to be done below).
        $form = confirm_form(
          $form,
          t('Are you sure you want to delete the recipe book @recipebook?', array('@recipebook' => $recipebook->title)),
          Botcha::ADMIN_PATH . '/recipebook',
          t('This action cannot be undone.'),
          t('Delete')
        );
        $form['actions']['submit']['#submit'] = array('Botcha_submitAdminForm');
        return $form;
        break;
      case 'recipebook_edit':
        $args = func_get_args();
        // @todo Abstract it.
        //$recipebook = (!empty($args[2]) ? $args[2] : NULL);
        $recipebook = (!empty($args[3]) ? $args[3] : NULL);
        // Determine default values depending on whether we add or edit recipe book.
        // Form a list of recipes.
        $options_recipes = array();
        $recipe_controller = $this->getController(Botcha::CONTROLLER_TYPE_RECIPE);
        foreach ($recipe_controller->getRecipes() as $recipe_id) {
          $recipe = $recipe_controller->getRecipe($recipe_id, FALSE);
          $options_recipes[$recipe->id] = $recipe->title;
        }
        $edit = !empty($recipebook);
        if ($edit) {
          if ($recipebook instanceof BotchaRecipebookNone) {
            // Redirect in case we are trying to edit unexisting item.
            drupal_goto(Botcha::ADMIN_PATH . '/recipebook/add', array('query' => array('rbid' => $recipebook->id)));
          }
          $default_id = $recipebook->id;
          $default_title = $recipebook->title;
          $default_description = $recipebook->description;
          $default_recipes = array_keys($recipebook->getRecipes());
          // @todo Abstract it.
          //$validate = array();
          $button = t('Save');
        }
        else {
          $default_id = '';
          $default_title = '';
          $default_description = '';
          $default_recipes = array();
          // @todo Abstract it.
          //$validate = array('botcha_recipebookIdValidate');
          $button = t('Add');
        }
        $form['id'] = array(
          // @todo Abstract it. Perhaps use Elements for adapter to it.
          // @see http://drupal.org/node/995784
          //'#type' => 'textfield',
          //'#element_validate' => $validate,
          //'#required' => TRUE,
          //'#title' => t('Machine-readable name'),
          //'#description' => t('A unique machine-readable name. Can only contain lowercase letters, numbers, and underscores.'),
          '#maxlength' => 128,
        );
        if ($edit) {
          // Change type to textfield to avoid unique checking.
          $form['id']['#type'] = 'textfield';
          $form['id']['#disabled'] =  TRUE;
          // We use #value instead of #default_value as a workaround.
          // @see http://drupal.org/node/1837306
          $form['id']['#value'] = $default_id;
        }
        else {
          $form['id']['#type'] = 'machine_name';
          $form['id']['#default_value'] = $default_id;
          $form['id']['#machine_name']['exists'] = 'Botcha_recipebookExists';
          $form['id']['#required'] = TRUE;
        }
        $form['title'] = array(
          '#type' => 'textfield',
          '#title' => t('Title'),
          '#description' => t('A title for this recipe book. You can always change this name later.'),
          '#default_value' => $default_title,
          '#required' => TRUE,
          '#maxlength' => 128,
        );
        $form['description'] = array(
          '#type' => 'textarea',
          '#rows' => 5,
          '#title' => t('Description'),
          '#description' => t('A description of the recipe book.'),
          '#default_value' => $default_description,
        );
        $form['recipes'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Recipes'),
          '#description' => t('Choose what recipes are included in recipe book.'),
          '#options' => $options_recipes,
          '#default_value' => $default_recipes,
        );
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => $button,
        );
        break;
      case 'recipebook_list':
        $form['#theme'] = 'botcha_recipebooks_form';
        // @todo Refactor it: turn list into multiple single forms.
        $form['#header'] = array(
          t('Title'),
          t('Description'),
          t('Operations'),
        );
        // Get all recipe books from database.
        $recipebooks = $this->getController(Botcha::CONTROLLER_TYPE_RECIPEBOOK)->getRecipebooks(TRUE);
        // Protect default recipebook from being deleted.
        foreach ($recipebooks as $recipebook) {
          // @todo Abstract it.
          //$form['recipebooks'][$recipebook->id]['title']['#value'] = $recipebook->title;
          $form['recipebooks'][$recipebook->id]['title']['#markup'] = $recipebook->title;
          // @todo Abstract it.
          //$form['recipebooks'][$recipebook->id]['description']['#value'] = $recipebook->description;
          $form['recipebooks'][$recipebook->id]['description']['#markup'] = $recipebook->description;
          // @todo Abstract it.
          //$form['recipebooks'][$recipebook->id]['operations']['#value'] =
          $form['recipebooks'][$recipebook->id]['operations']['#markup'] =
            // @todo Remove hardcode.
            in_array($recipebook->id, array('forbidden_forms')) ? '' : l(t('Edit'), Botcha::ADMIN_PATH . "/recipebook/{$recipebook->id}")
            // @todo Remove hardcode.
            . (in_array($recipebook->id, array('default', 'forbidden_forms')) ? '' : ' | ' . l(t('Delete'), Botcha::ADMIN_PATH . "/recipebook/{$recipebook->id}/delete"));
        }
        break;
    }
    // Turn on submit magic.
    // @todo Remove it after RecipeUI implementation.
    if ($form_name != 'recipe_list') {
      $form['submit']['#submit'] = array('Botcha_submitAdminForm');
    }
    return $form;
  }

  /**
   * Unified submit handler for admin forms.
   */
  public function submitAdminForm($form, &$form_state) {
    parent::submitAdminForm($form, $form_state);
    $form_name = $form['#form_name']['#value'];
    switch ($form_name) {
      // @todo Refactor switch stuff with classes since they are similar a lot.
      case 'general':
        // Generate the secret key.
        // @todo Move secret key generation to validate phase.
        if (empty($form_state['values']['botcha_secret'])) {
          // Generate unique secret for this site
          $secret = $this->generateSecretKey();
          $form_state['values']['botcha_secret'] = $secret;
          drupal_set_message(t('New BOTCHA secret key have been generated.'));
        }
        // Do what system_settings_form() would do with regular variable fields
        variable_set('botcha_secret', $form_state['values']['botcha_secret']);
        variable_set('botcha_loglevel', $form_state['values']['botcha_loglevel']);
        drupal_set_message(t('The BOTCHA settings were saved.'), 'status');
        break;
      case 'form_delete':
        $form_id = $form_state['values']['botcha_form_id'];
        $form_controller = $this->getController(Botcha::CONTROLLER_TYPE_FORM);
        $botcha_form = $form_controller->getForm($form_id, FALSE);
        $form_controller->delete($botcha_form);
        drupal_set_message(t('Deleted BOTCHA protection for form %form_id.', array('%form_id' => $form_id)));
        $form_state['redirect'] = Botcha::ADMIN_PATH . '/form';
        break;
      case 'form_edit':
        $form_id = $form_state['values']['botcha_form_id'];
        $enabled = $form_state['values']['botcha_enabled'];
        $rbid = $form_state['values']['botcha_form_recipebook'];
        $operation = $form_state['values']['botcha_operation'];
        $add = ($operation == 'add');
        $form_controller = $this->getController(Botcha::CONTROLLER_TYPE_FORM);
        $botcha_form = $form_controller->getForm($form_id, $add)
          ->setEnabled($enabled)
          ->setRecipebook($rbid);
        $form_controller->save($botcha_form);
        switch ($operation) {
          case 'edit':
            // Check if the form was really modified.
            // @todo Refactor in a more general manner.
            $edited = ($form['botcha_enabled']['#default_value'] != $form['botcha_enabled']['#value']
              || $form['botcha_form_recipebook']['#default_value'] != $form['botcha_form_recipebook']['#value']);
            if ($edited) {
              drupal_set_message(t('Saved BOTCHA form settings for %form_id.', array('%form_id' => $form_id)), 'status');
            }
            break;
          case 'add':
          default:
            // Check if the form was really modified.
            // @todo Refactor in a more general manner.
            $added = ($form['botcha_form_id']['#default_value'] != $form['botcha_form_id']['#value']
              || $form['botcha_form_recipebook']['#default_value'] != $form['botcha_form_id']['#value']);
            if ($added) {
              drupal_set_message(t('Added BOTCHA form %form_id.', array('%form_id' => $form_id)), 'status');
            }
            break;
        }
        $form_state['redirect'] = Botcha::ADMIN_PATH . '/form';
        break;
      case 'form_list':
        $forms = $form['botcha_form_protection']['botcha_form_id_overview']['botcha_forms'];
        $form_ids = element_children($forms);
        foreach ($form_ids as $form_id) {
          // @todo Replace this workaround with more beautiful solution.
          $fake_form_state['values'] = array(
            'botcha_form_id' => $form_state['values']['botcha_form_id_overview']['botcha_forms'][$form_id]['botcha_form_id'],
            'botcha_enabled' => $form_state['values']['botcha_form_id_overview']['botcha_forms'][$form_id]['botcha_enabled'],
            'botcha_form_recipebook' => $form_state['values']['botcha_form_id_overview']['botcha_forms'][$form_id]['botcha_form_recipebook'],
            'botcha_operation' => 'edit',
          );
          $this->submitAdminForm($forms[$form_id], $fake_form_state);
        }
        // Do what system_settings_form() would do with regular variable fields
        variable_set('botcha_on_captcha_forms', !empty($form_state['values']['botcha_on_captcha_forms']) ? $form_state['values']['botcha_on_captcha_forms'] : FALSE);
        variable_set('botcha_administration_mode', $form_state['values']['botcha_administration_mode']);
        variable_set('botcha_allow_on_admin_pages', $form_state['values']['botcha_allow_on_admin_pages']);
        drupal_set_message(t('The BOTCHA settings were saved.'), 'status');
        break;
      case 'recipe_delete':
        // Unused yet.
        break;
      case 'recipe_edit':
        // Unused yet.
        break;
      case 'recipe_list':
        break;
      case 'recipebook_delete':
        $form_state['redirect'] = Botcha::ADMIN_PATH . '/recipebook';
        // Remove recipe book.
        $this->getController(Botcha::CONTROLLER_TYPE_RECIPEBOOK)->delete($form['#botcha_recipebook']);
        break;
      case 'recipebook_edit':
        $values = $form_state['values'];
        $recipebook_controller = $this->getController(Botcha::CONTROLLER_TYPE_RECIPEBOOK);
        $recipebook = $recipebook_controller->getRecipebook($values['id'])
          ->setTitle($values['title'])
          ->setDescription($values['description']);
        foreach ($values['recipes'] as $recipe_id => $value) {
          if ($value) {
            $recipebook->setRecipe($recipe_id);
          }
          else {
            $recipebook->unsetRecipe($recipe_id);
          }
        }
        $recipebook_controller->save($recipebook);
        $form_state['redirect'] = Botcha::ADMIN_PATH . '/recipebook';
        drupal_set_message(t('Settings for recipe book "%recipebook" are successfully saved.', array('%recipebook' => $recipebook->id)), 'status');
        break;
      case 'recipebook_list':
        break;
    }
  }
}
