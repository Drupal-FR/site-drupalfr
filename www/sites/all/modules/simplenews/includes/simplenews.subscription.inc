<?php

/**
 * @file
 * (Un)subscription and (un)subscription confirmation
 *
 * FAPI subscription form cases:
 * - ACCOUNT
 *   self/admin action: authenticated user
 *   via hook_user form: category=newsletter
 *
 * - BLOCK
 *   self action: anonymous / authenticated user
 *   via hook_block: block
 *
 * - PAGE
 *   self action: anonymous / authenticated user
 *   callback: newsletter/subscriptions
 *
 * - MULTI BLOCK
 *   self action: anonymous / authenticated user
 *   authenticated user
 *   via hook_block: multi_block
 *   using PAGE handlers
 *
 * - ADMIN
 *   admin action: authenticated user
 *   via hook_menu: admin
 *
 * FAPI additional form cases:
 * - CONFIRM ADD
 * - CONFIRM REMOVAL
 *
 * @ingroup simplenews
 */

/**
 * FAPI ACCOUNT subscription form.
 *
 * Finally _account_ cases inject into hook_user and won't work on its own.
 * Note that our basis is:
 * drupal_get_form('user_profile_form', ...);
 * and NOT:
 * drupal_get_form('simplenews_subscriptions_account', ...);
 *
 * see also user/user.module and user/user.pages.inc
 *
 * @see simplenews_subscriptions_account_form_validate()
 * @see simplenews_subscriptions_account_form_submit()
 */
function simplenews_subscriptions_account_form(&$form_state, $account) {
  $subscription = simplenews_get_subscription($account);

  $form = array();
  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_get_newsletters(variable_get('simplenews_vid', '')) as $newsletter) {
    $options[$newsletter->tid] = check_plain($newsletter->name);
    $default_value[$newsletter->tid] = FALSE;
  }

  $form['subscriptions'] = array(
    '#type' => 'fieldset',
    '#description' => t('Select your newsletter subscriptions.'),
  );
  $form['subscriptions']['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array_merge($default_value, (array)$subscription->tids),
  );

  $form['subscriptions']['#title'] =  t('Current newsletter subscriptions');

  // if we don't override #validate, see user_profile_form_validate

  // adding an own #submit leads to the situation where drupal omits execution of user_profile_form_submit completely
  $form['#submit'][] = 'simplenews_subscriptions_account_form_submit';

  return $form;
}

/**
 * FAPI ACCOUNT subscription form_submit.
 */
function simplenews_subscriptions_account_form_submit($form, &$form_state) {
  global $user;
  // Get current subscriptions if any. Defined in user_profile_form
  $account = $form_state['values']['_account'];

  // We first subscribe, then unsubscribe. This prevents deletion of subscriptions
  // when unsubscribed from the
  arsort($form_state['values']['newsletters'], SORT_NUMERIC);
  foreach ($form_state['values']['newsletters'] as $tid => $checked) {
    if ($checked) {
      simplenews_subscribe_user($account->mail, $tid, FALSE, 'website');
    }
    else {
      simplenews_unsubscribe_user($account->mail, $tid, FALSE, 'website');
    }
  }
  if ($user->uid==$account->uid) {
    drupal_set_message(t('Your newsletter subscriptions have been updated.'));
  }
  else {
    drupal_set_message(t('The newsletter subscriptions for user %account have been updated.', array('%account' => $account->name)));
  }
}

/**
 * FAPI BLOCK subscription form.
 *
 * @param $tid term id of selected newsletter.
 *
 * @see simplenews_block_form_validate()
 * @see simplenews_block_form_submit()
 */
function simplenews_block_form(&$form_state, $tid) {
  global $user;
  $form = array();

  $submit_text = t('Subscribe');
  if ($user->uid) {
    if (simplenews_user_is_subscribed($user->mail, $tid)) {
      $submit_text = t('Unsubscribe');
      $form['action'] = array('#type' => 'value', '#value' => 'unsubscribe');
      $form['#attributes'] = array('class' => 'simplenews-unsubscribe');
    }
    else {
      $form['action'] = array('#type' => 'value', '#value' => 'subscribe');
      $form['#attributes'] = array('class' => 'simplenews-subscribe');
    }
    $form['mail'] = array('#type' => 'value', '#value' => $user->mail);
  }
  else {
    $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#size' => 20,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
    $form['action'] = array('#type' => 'value', '#value' => 'subscribe');
    $form['#attributes'] = array('class' => 'simplenews-subscribe');
  }

  // All block forms use the same validate and submit function.
  // #tid carries the tid for processing of the right newsletter issue term.
  $form['#tid'] = $tid;
  $form['#validate'][] = 'simplenews_block_form_validate';
  $form['#submit'][] = 'simplenews_block_form_submit';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_text,
  );
  return $form;
}

/*
 * FAPI BLOCK subscription form_validate.
 */
function simplenews_block_form_validate($form, &$form_state) {
  if (!simplenews_valid_email_address($form_state['values']['mail'])) {
    form_set_error('mail', t("The email address you supplied is not valid."));
  }
}

/*
 * FAPI BLOCK subscription form_submit.
 */
function simplenews_block_form_submit($form, &$form_state) {
  global $user;
  $tid = $form['#tid'];
  $account = _simplenews_user_load($form_state['values']['mail']);

  // If email belongs to the current registered user, don't send confirmation.
  // Other addresses receive a confirmation if double opt-in is selected.
  if ($account->uid && $account->uid == $user->uid) {
    $confirm = FALSE;
  }
  else {
    $confirm = variable_get('simplenews_opt_inout_'. $tid, 'double') == 'double';
  }

  switch ($form_state['values']['action']) {
    case 'subscribe':
      simplenews_subscribe_user($form_state['values']['mail'], $tid, $confirm, 'website');
      if ($confirm) {
        drupal_set_message(t('You will receive a confirmation email shortly containing further instructions on how to complete your subscription.'));
      }
      else {
        drupal_set_message(t('You have been subscribed.'));
      }
      break;
    case 'unsubscribe':
      simplenews_unsubscribe_user($form_state['values']['mail'], $tid, $confirm, 'website');
      if ($confirm) {
        drupal_set_message(t('You will receive a confirmation email shortly containing further instructions on how to cancel your subscription.'));
      }
      else {
        drupal_set_message(t('You have been unsubscribed.'));
      }
      break;
  }
}

/**
 * FAPI PAGE subscription form.
 *
 * @see simplenews_subscriptions_page_form_validate()
 * @see simplenews_subscriptions_page_form_submit()
 */
function simplenews_subscriptions_page_form(&$form_state) {
  global $user;

  $subscription = simplenews_get_subscription($user);

  $form = array();
  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_get_newsletters(variable_get('simplenews_vid', '')) as $newsletter) {
    $options[$newsletter->tid] = check_plain($newsletter->name);
    $default_value[$newsletter->tid] = FALSE;
  }

  $form['subscriptions'] = array(
    '#type' => 'fieldset',
    '#description' => t('Select the newsletter(s) to which you want to subscribe or unsubscribe.'),
  );
  $form['subscriptions']['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array_merge($default_value, (array)$subscription->tids),
  );

  // If current user is logged in, just display email.
  // Anonymous users see an email box and will receive confirmations
  if ($subscription->mail) {
    $form['subscriptions']['#title'] = t('Subscriptions for %mail', array('%mail' => $subscription->mail));
    $form['subscriptions']['mail'] = array('#type' => 'value', '#value' => $subscription->mail);
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
  }
  else {
    $form['subscriptions']['#title'] = t('Manage your newsletter subscriptions');
    $form['subscriptions']['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('email'),
      '#size' => 20,
      '#maxlength' => 128,
      '#weight' => 10,
      '#required' => TRUE,
    );
    $form['subscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Subscribe'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
    $form['unsubscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Unsubscribe'),
      '#weight' => 30,
      // @todo: add clean submit handler
    );
  }

  $form['#validate'][] = 'simplenews_subscriptions_page_form_validate';
  $form['#submit'][] = 'simplenews_subscriptions_page_form_submit';

  return $form;
}

/**
 * FAPI PAGE subscription form_validate.
 */
function simplenews_subscriptions_page_form_validate($form, &$form_state) {
  global $user;
  $valid_email = simplenews_valid_email_address($form_state['values']['mail']);
  if (!$valid_email) {
    form_set_error('mail', t('The email address you supplied is not valid.'));
  }
  if (isset($user->mail) && $user->mail == $form_state['values']['mail']) {
    $account = $user;
  }
  else {
    $account = (object) array('mail' => $form_state['values']['mail']);
  }
  $subscription = simplenews_get_subscription($account);

  $checked_newsletters = array_filter($form_state['values']['newsletters']);
  if (!count($checked_newsletters) && !$subscription->uid) {
    form_set_error('newsletters', t('You must select at least one newsletter.'));
  }
}

/**
 * FAPI PAGE subscription form_submit.
 */
function simplenews_subscriptions_page_form_submit($form, &$form_state) {
  global $user;
  if (isset($user->mail) && $user->mail == $form_state['values']['mail']) {
    $account = $user;
  }
  else {
    $account = (object) array('mail' => $form_state['values']['mail']);
  }

  switch ($form_state['values']['op']) {
    case t('Update'):
      // We first subscribe, then unsubscribe. This prevents deletion of subscriptions
      // when unsubscribed from the
      arsort($form_state['values']['newsletters'], SORT_NUMERIC);
      foreach ($form_state['values']['newsletters'] as $tid => $checked) {
        if ($checked) {
          simplenews_subscribe_user($form_state['values']['mail'], $tid, FALSE, 'website');
        }
        else {
          simplenews_unsubscribe_user($form_state['values']['mail'], $tid, FALSE, 'website');
        }
      }
      drupal_set_message(t('The newsletter subscriptions for %mail have been updated.', array('%mail' => $form_state['values']['mail'])));
      break;
    case t('Subscribe'):
      $confirm_any = FALSE;
      foreach ($form_state['values']['newsletters'] as $tid => $checked) {
        $confirm = FALSE;
        if ($checked) {
          if (empty($account->uid)) {
            $confirm = variable_get('simplenews_opt_inout_'. $tid, 'double') == 'double';
            $confirm_any |= $confirm;
          }
          simplenews_subscribe_user($form_state['values']['mail'], $tid, $confirm, 'website');
        }
      }
      if ($confirm_any) {
        drupal_set_message(t('You will receive a confirmation email shortly containing further instructions on how to complete your subscription.'));
      }
      else {
        drupal_set_message(t('You were added to the chosen mailing list.'));
      }
      break;
    case t('Unsubscribe'):
      $confirm_any = FALSE;
      foreach ($form_state['values']['newsletters'] as $tid => $checked) {
        $confirm = FALSE;
        if ($checked) {
          if (!$account->uid) {
            $confirm = variable_get('simplenews_opt_inout_'. $tid, 'double') == 'double';
            $confirm_any |= $confirm;
          }
          simplenews_unsubscribe_user($form_state['values']['mail'], $tid, $confirm, 'website');
        }
      }
      if ($confirm_any) {
        drupal_set_message(t('You will receive a confirmation email shortly containing further instructions on how to cancel your subscription.'));
      }
      else {
        drupal_set_message(t('You were added to the chosen mailing list.'));
      }
      break;
  }
}

/**
 * FAPI MULTI BLOCK subscription form.
 *
 * Menu callback: Generates the subscription form for users for the multisignup block.
 *
 * @see simplenews_subscriptions_multi_block_form_validate()
 * @see simplenews_subscriptions_multi_block_form_submit()
 */
function simplenews_subscriptions_multi_block_form(&$form_state) {
  global $user;

  $subscription = simplenews_get_subscription($user);

  // If someone not authorized to edit their subscription, return empty form.
  if (!user_access('subscribe to newsletters')) {
    return;
  }

  $form = array();
  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_get_newsletters(variable_get('simplenews_vid', '')) as $newsletter) {
    $options[$newsletter->tid] = check_plain($newsletter->name);
    $default_value[$newsletter->tid] = FALSE;
  }

  $form['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array_merge($default_value, (array)$subscription->tids),
  );

  // If current user is logged in, just display email.
  // Anonymous users see an email box and will receive confirmations
  if ($subscription->mail) {
    // @todo why not simply Manage your subscriptions?
    $form['mail'] = array('#type' => 'value', '#value' => $subscription->mail);
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
  }
  else {
    $form['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#size' => 20,
      '#maxlength' => 128,
      '#weight' => 10,
      '#required' => TRUE,
    );
    $form['subscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Subscribe'),
      '#weight' => 20,
      // @todo: add clean submit handler
    );
    $form['unsubscribe'] = array(
      '#type' => 'submit',
      '#value' => t('Unsubscribe'),
      '#weight' => 30,
      // @todo: add clean submit handler
    );
  }

  $form['#validate'][] = 'simplenews_subscriptions_page_form_validate';
  $form['#submit'][] = 'simplenews_subscriptions_page_form_submit';

  return $form;
}

/**
 * Menu callback: confirm the user's (un)subscription request
 *
 * This function is called by clicking the confirm link in the confirmation
 * email or the unsubscribe link in the footer of the newsletter. It handles
 * both subscription addition and subscription removal.
 *
 * Calling URLs are:
 * newsletter/confirm/add
 * newsletter/confirm/add/$HASH
 * newsletter/confirm/remove
 * newsletter/confirm/remove/$HASH
 *
 * @see simplenews_confirm_add_form()
 * @see simplenews_confirm_removal_form()
 */
function simplenews_confirm_subscription() {
  $arguments = func_get_args();
  // add or remove
  $op1 = array_shift($arguments);
  // private key hash
  $op2 = array_shift($arguments);

  $md5 = drupal_substr($op2, 0, 10);
  list($snid, $tid) = explode('t', drupal_substr($op2, 10));

  $result = db_query('
    SELECT snid, mail
    FROM {simplenews_subscriptions}
    WHERE snid = %d',
    $snid);
  if (!($subs = db_fetch_object($result))) {
    drupal_not_found();
    return;
  }

  // Prevent search engines from indexing this page.
  drupal_set_html_head('<meta name="robots" content="noindex" />');

  if ($md5 == drupal_substr(md5($subs->mail . simplenews_private_key()), 0, 10)) {
    $newsletter = taxonomy_get_term($tid);
    // The confirmation page called with two arguments will display a confirmation question.
    // When called with three of more arguments the user will be directed to the
    // (un)subscribe confirmation page. The additional arguments will be passed on
    // to the confirmation page.
    if (empty($arguments)) {
      if ($op1 == 'remove') {
        return drupal_get_form('simplenews_confirm_removal_form', $subs->mail, $newsletter);
      }
      elseif ($op1 == 'add') {
        return drupal_get_form('simplenews_confirm_add_form', $subs->mail, $newsletter);
      }
    }
    else {
      if ($op1 == 'remove') {
        simplenews_unsubscribe_user($subs->mail, $tid, FALSE, 'website');
        if ($path = variable_get('simplenews_confirm_unsubscribe_page', '')) {
          $path = $path .'/'. implode('/', $arguments);
          drupal_goto($path);
        }
        drupal_set_message(t('%user was unsubscribed from the %newsletter mailing list.', array('%user' => $subs->mail, '%newsletter' => $newsletter->name)));
        drupal_goto(variable_get('site_frontpage', 'node'));
      }
      elseif ($op1 == 'add') {
        simplenews_subscribe_user($subs->mail, $tid, FALSE, 'website');
        if ($path = variable_get('simplenews_confirm_subscribe_page', '')) {
          $path = $path .'/'. implode('/', $arguments);
          drupal_goto($path);
        }
        drupal_set_message(t('%user was added to the %newsletter mailing list.', array('%user' => $subs->mail, '%newsletter' => $newsletter->name)));
        drupal_goto(variable_get('site_frontpage', 'node'));
      }
    }
  }

  // If md5 didn't match, do a not found.
  drupal_not_found();
  return;
}

/**
 * FAPI CONFIRM ADD form.
 *
 * @see simplenews_confirm_add_form_submit()
 */
function simplenews_confirm_add_form(&$form_state, $mail, $newsletter) {
  $form = array();
  $form['question'] = array('#value' => '<p>'. t('Are you sure you want to add %user to the %newsletter mailing list?', array('%user' => simplenews_mask_mail($mail), '%newsletter' => $newsletter->name)) ."<p>\n");
  $form['mail'] = array('#type' => 'value', '#value' => $mail);
  $form['newsletter'] = array('#type' => 'value', '#value' => $newsletter);
  $form['#redirect'] = variable_get('simplenews_confirm_subscribe_page', '');

  return confirm_form($form,
    t('Confirm subscription'),
    '',
    t('You will receive %newsletter mails.', array('%newsletter' => $newsletter->name)),
    t('Subscribe'),
    t('Cancel')
  );
}

/**
 * FAPI CONFIRM ADD form_submit.
 */
function simplenews_confirm_add_form_submit($form, &$form_state) {
  simplenews_subscribe_user($form_state['values']['mail'], $form_state['values']['newsletter']->tid, FALSE, 'website');

  // Display message if user is directed to the front page.
  if (!$form['#redirect']) {
    drupal_set_message(t('%user was added to the %newsletter mailing list.', array('%user' => $form_state['values']['mail'], '%newsletter' => $form_state['values']['newsletter']->name)));
  }
}

/**
 * Mask a mail address.
 *
 * For example, name@example.org will be masked as n*****@e*****.org.
 *
 * @param $mail
 *   A valid mail address to mask.
 *
 * @return
 *   The masked mail address.
 */
function simplenews_mask_mail($mail) {
  if (preg_match('/^(.).*@(.).*(\..+)$/', $mail)) {
    return preg_replace('/^(.).*@(.).*(\..+)$/', '$1*****@$2*****$3', $mail);
  }
  else {
    // Missing top-level domain.
    return preg_replace('/^(.).*@(.).*$/', '$1*****@$2*****', $mail);
  }
}

/**
 * FAPI CONFIRM REMOVAL form.
 *
 * @see simplenews_confirm_removal_form_submit()
 */
function simplenews_confirm_removal_form(&$form_state, $mail, $newsletter) {
  $form = array();
  $form['question'] = array('#value' => '<p>'. t('Are you sure you want to remove %user from the %newsletter mailing list?', array('%user' => simplenews_mask_mail($mail), '%newsletter' => $newsletter->name)) ."<p>\n");
  $form['mail'] = array('#type' => 'value', '#value' => $mail);
  $form['newsletter'] = array('#type' => 'value', '#value' => $newsletter);
  $form['#redirect'] = variable_get('simplenews_confirm_unsubscribe_page', '');

  return confirm_form($form,
    t('Confirm remove subscription'),
    '',
    t('You will no longer receive %newsletter mails.', array('%newsletter' => $newsletter->name)),
    t('Unsubscribe'),
    t('Cancel')
  );
}

/**
 * FAPI CONFIRM REMOVAL form_submit.
 */
function simplenews_confirm_removal_form_submit($form, &$form_state) {
  simplenews_unsubscribe_user($form_state['values']['mail'], $form_state['values']['newsletter']->tid, FALSE, 'website');
  // Display message if user is directed to the front page.
  if (!$form['#redirect']) {
    drupal_set_message(t('%user was unsubscribed from the %newsletter mailing list.', array('%user' => $form_state['values']['mail'], '%newsletter' => $form_state['values']['newsletter']->name)));
  }
}

/**
 * FAPI ADMIN subscription form.
 *
 * Menu callback: handle the edit subscription page and a subscription
 * page for anonymous users.
 *
 * @see simplenews_subscriptions_admin_form_validate()
 * @see simplenews_subscriptions_admin_form_submit()
 */
function simplenews_subscriptions_admin_form(&$form_state, $snid) {
  $account = (object) array('snid' => $snid);
  $subscription = simplenews_get_subscription($account);

  $form = array();
  $options = array();
  $default_value = array();

  // Get newsletters for subscription form checkboxes.
  // Newsletters with opt-in/out method 'hidden' will not be listed.
  foreach (simplenews_get_newsletters(variable_get('simplenews_vid', '')) as $newsletter) {
    $options[$newsletter->tid] = check_plain($newsletter->name);
    $default_value[$newsletter->tid] = FALSE;
  }

  $form['subscriptions'] = array(
    '#title' => t('Subscriptions for %mail', array('%mail' => $subscription->mail)),
    '#type' => 'fieldset',
    '#description' => t('Select the newsletter(s) to add/remove from subscription.'),
  );
  $form['subscriptions']['newsletters'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => array_merge($default_value, (array)$subscription->tids),
  );

  $form['activated'] = array(
    '#title' => t('Activation'),
    '#type' => 'fieldset',
    '#description' => t('Activate or inactivate account.'),
  );
  $form['activated']['activated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activated'),
    '#default_value' => $subscription->activated,
  );

  if ((variable_get('language_count', 1) > 1)) {
    // TODO we could allow to switch back to "default", but user_load
    //$language_options[''] = t('Site default language');
    $languages = language_list('enabled');
    foreach ($languages[1] as $langcode => $item) {
      $name = t($item->name);
      $language_options[$langcode] = $name . ($item->native != $name ? ' ('. $item->native .')' : '');
    }
    // std users have language in profile. disable
    $disabled = $subscription->uid ? TRUE : FALSE;
    $form['language'] = array(
      '#type' => 'fieldset',
      '#title' => 'Preferred language',
      '#description' => t('The emails will be localized in language chosen. Real users have their preference in account settings.'),
      '#disabled' => FALSE,
    );
    if ($subscription->uid) {
      // fapi error: disabled not supported for select type. workaround: output markup
      $form['language']['language'] = array(
        '#type' => 'markup',
        '#value' => $language_options[$subscription->language->language],
      );
    }
    else {
      $form['language']['language'] = array(
        '#type' => 'select',
        '#default_value' => $subscription->language->language,
        '#options' => $language_options,
      );
    }
  }

  $form['subscriptions']['mail'] = array('#type' => 'value', '#value' => $subscription->mail);
  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 20,
  );

  $form['#validate'][] = 'simplenews_subscriptions_admin_form_validate';
  $form['#submit'][] = 'simplenews_subscriptions_admin_form_submit';
  $form['#redirect'] = 'admin/content/simplenews/users';

  return $form;
}

/**
 * FAPI ADMIN subscription form_validate.
 */
function simplenews_subscriptions_admin_form_validate($form, &$form_state) {
  global $user;
  if (isset($user->mail) && $user->mail == $form_state['values']['mail']) {
    $account = $user;
  }
  else {
    $account = (object) array('mail' => $form_state['values']['mail']);
  }
  $subscription = simplenews_get_subscription($account);

  $valid_email = simplenews_valid_email_address($form_state['values']['mail']);
  if (!$valid_email) {
    form_set_error('mail', t('The email address you supplied is not valid.'));
  }
  $checked_newsletters = array_filter($form_state['values']['newsletters']);
  if (!count($checked_newsletters) && $subscription->uid) {
    form_set_error('newsletters', t('You must select at least one newsletter.'));
  }
  $languages = language_list('enabled');
  if (!empty($form_state['values']['language'])
    && !isset($languages[1][$form_state['values']['language']])) {
    form_set_error('language', t('Please choose a language from the list.'));
  }
}

/**
 * FAPI ADMIN subscription form_submit.
 */
function simplenews_subscriptions_admin_form_submit($form, &$form_state) {
  global $user;
  if (isset($user->mail) && $user->mail == $form_state['values']['mail']) {
    $account = $user;
  }
  else {
    $account = (object) array('mail' => $form_state['values']['mail']);
  }
  $subscription = simplenews_get_subscription($account);

  arsort($form_state['values']['newsletters'], SORT_NUMERIC);
  foreach ($form_state['values']['newsletters'] as $tid => $checked) {
    if ($checked) {
      simplenews_subscribe_user($form_state['values']['mail'], $tid, FALSE, 'website');
    }
    else {
      simplenews_unsubscribe_user($form_state['values']['mail'], $tid, FALSE, 'website');
    }
  }

  $data = array();
  $data['activated'] = $form_state['values']['activated'];
  if (!$subscription->uid) {
    if (isset($form_state['values']['language'])) {
      $data['language'] = $form_state['values']['language'];
    }
  }
  simplenews_subscriber_update($subscription, $data);

  drupal_set_message(t('The newsletter subscriptions for %mail have been updated.', array('%mail' => $form_state['values']['mail'])));
}

