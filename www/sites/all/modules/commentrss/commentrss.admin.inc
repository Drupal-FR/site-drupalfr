<?php

/**
 * @file
 *   Administration callbacks for Comment RSS.
 */

/**
 * Menu callback; displays the commentrss module settings page.
 */
function commentrss_admin_settings() {
  $form['commentrss_site'] = array(
    '#type' => 'radios',
    '#title' => t('Enable aggregated feed for comments on all content on the entire website, and expose on pages'),
    '#options' => array(
      COMMENTRSS_SITE_DISABLED => t('Disabled'),
      COMMENTRSS_SITE_FRONT_PAGE => t('Enabled, expose only on front page'),
      COMMENTRSS_SITE_FRONT_AND_NODE_PAGE => t('Enabled, expose on front page and <a href="@url">default content listing</a> page', array('@url' => url('node'))),
      COMMENTRSS_SITE_ALL_PAGES => t('Enabled, expose on all pages'),
    ),
    '#description' => t('Where should the site comment feed be exposed, if at all enabled. If enabled, feed will be accessible at @url.', array('@url' => url('crss', array('absolute' => TRUE)))),
    '#default_value' => variable_get('commentrss_site', COMMENTRSS_SITE_FRONT_PAGE),
  );
  $form['commentrss_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable individual comment feeds for each post on the website'),
    '#description' => t('Feeds will be accessible on @url type of URLs.', array('@url' => url('crss/node/...', array('absolute' => TRUE)))),
    '#default_value' => variable_get('commentrss_node', TRUE),
  );
  $form['commentrss_term'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable individual comment feeds for each taxonomy term listing on the website'),
    '#description' => t('Feeds will be accessible at @url type of URLs. Only supports one term, no composition.', array('@url' => url('crss/term/...', array('absolute' => TRUE)))),
    '#default_value' => variable_get('commentrss_term', FALSE),
    '#access' => FALSE,
  );

  return system_settings_form($form);
}
