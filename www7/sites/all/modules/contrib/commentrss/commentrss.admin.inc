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

  $form['individual_comment_feeds'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Individual comment feeds'),
  );

  $form['individual_comment_feeds']['commentrss_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable individual comment feeds for each post on the website'),
    '#description' => t('Feeds will be accessible on @url type of URLs.', array('@url' => url('crss/node/...', array('absolute' => TRUE)))),
    '#default_value' => variable_get('commentrss_node', TRUE),
  );

  $form['individual_comment_feeds']['commentrss_node_type'] = array(
    '#type' => 'select',
    '#options' => node_type_get_names(),
    '#title' => t('Show the feed icon to specific content types:'),
    '#description' => t('Select which content types will present an individual feed icon. If blank, will show up for all content types.'),
    '#default_value' => variable_get('commentrss_node_type', array()),
    '#multiple' => TRUE,
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
