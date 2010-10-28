<?php
// $Id: globalredirect.admin.inc,v 1.1.2.5 2008/12/22 10:42:06 njt1982 Exp $

/**
 * @file
 *  This is the GlobalRedirect admin include which provides an interface to global redirect to change some of the default settings
 */

/**
 *  Function to generate the form setting array
 */
function globalredirect_settings() {
  $form = array();

  $form['globalredirect_deslash'] = array(
    '#type' => 'radios',
    '#title' => t('Deslash'),
    '#description' => t('If enabled, this option will remove the trailing slash from requests. This stops requests such as <code>example.com/node/1/</code> failing to match the corresponding alias and can cause duplicate content. On the other hand, if you require certain requests to have a trailing slash, this feature can cause problems so may need to be disabled.'),
    '#options' => array(
      GLOBALREDIRECT_FEATURE_DISABLED => t('Off'),
      GLOBALREDIRECT_DESLASH_ENABLED  => t('On'),
    ),
    '#default_value' => variable_get('globalredirect_deslash', GLOBALREDIRECT_DESLASH_ENABLED),
  );


  $form['globalredirect_nonclean2clean'] = array(
    '#type' => 'radios',
    '#title' => t('Non-clean to Clean'),
    '#description' => t('If enabled, this option will redirect from non-clean to clean URL (if Clean URL\'s are enabled). This will stop, for example, node 1  existing on both <code>example.com/node/1</code> AND <code>example.com?q=node/1</code>.'),
    '#options' => array(
      GLOBALREDIRECT_FEATURE_DISABLED => t('Off'),
      GLOBALREDIRECT_NONCLEAN2CLEAN_ENABLED  => t('On'),
    ),
    '#default_value' => variable_get('globalredirect_nonclean2clean', GLOBALREDIRECT_NONCLEAN2CLEAN_ENABLED),
  );

  $form['globalredirect_trailingzero'] = array(
    '#type' => 'radios',
    '#title' => t('Remove Trailing Zero Argument'),
    '#description' => t('If enabled, any instance of "/0" will be trimmed from the right of the URL. This stops duplicate pages such as "taxonomy/term/1" and "taxonomy/term/1/0" where 0 is the default depth. There is an option of limiting this feature to taxonomy term pages ONLY or allowing it to effect any page. <strong>By default this feature is disabled to avoid any unexpected behaviour</strong>'),
    '#options' => array(
      GLOBALREDIRECT_FEATURE_DISABLED => t('Disabled'),
      GLOBALREDIRECT_TRAILINGZERO_TAXTERM => t('Enabled for taxonomy term pages only'),
      GLOBALREDIRECT_TRAILINGZERO_ALL => t('Enabled for all pages'),
    ),
    '#default_value' => variable_get('globalredirect_trailingzero', GLOBALREDIRECT_FEATURE_DISABLED),
  );

  $form['globalredirect_menu_check'] = array(
    '#type' => 'radios',
    '#title' => t('Menu Access Checking'),
    '#description' => t('If enabled, the module will check the user has access to the page before redirecting. This helps to stop redirection on protected pages and avoids giving away <em>secret</em> URL\'s. <strong>By default this feature is disabled to avoid any unexpected behaviour</strong>'),
    '#options' => array(
      GLOBALREDIRECT_FEATURE_DISABLED => t('Disabled'),
      GLOBALREDIRECT_MENU_CHECK_ENABLED => t('Enabled'),
    ),
    '#default_value' => variable_get('globalredirect_menu_check', GLOBALREDIRECT_FEATURE_DISABLED),
  );

  $form['globalredirect_case_sensitive_urls'] = array(
    '#type' => 'radios',
    '#title' => t('Case Sensitive URL Checking'),
    '#description' => t('If enabled, the module will compae the current URL to the alias stored in the system. If there are any differences in case then the user will be redirected to the correct URL.'),
    '#options' => array(
      GLOBALREDIRECT_FEATURE_DISABLED => t('Disabled'),
      GLOBALREDIRECT_CASE_SENSITIVE_URLS_ENABLED => t('Enabled'),
    ),
    '#default_value' => variable_get('globalredirect_case_sensitive_urls', GLOBALREDIRECT_CASE_SENSITIVE_URLS_ENABLED),
  );

  return system_settings_form($form);
}

