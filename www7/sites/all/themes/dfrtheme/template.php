<?php

/**
 * Implements template_html_head_alter().
 *
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function dfrtheme_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Implements template_preprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute.
 * This is mostly duplicated from template_preprocess_search_block_form().
 *
 * @see template_preprocess_search_block_form()
 */
function dfrtheme_preprocess_drupalfr_search_form(&$variables) {
  $variables['search'] = array();
  $hidden = array();

  // Provide variables named after form keys so themers can print each element
  // independently.
  foreach (element_children($variables['form']) as $key) {
    $type = $variables['form'][$key]['#type'];
    if ($type == 'hidden' || $type == 'token') {
      $hidden[] = drupal_render($variables['form'][$key]);
    }
    else {
      $variables['search'][$key] = drupal_render($variables['form'][$key]);
    }
  }
  // Hidden form elements have no value to themers. No need for separation.
  $variables['search']['hidden'] = implode($hidden);

  // Collect all form elements to make it easier to print the whole form.
  $variables['drupalfr_search_form'] = implode($variables['search']);
  $variables['drupalfr_search_form'] = str_replace('type="text"', 'type="search"', $variables['drupalfr_search_form']);
}

/**
 * Implements template_preprocess_html().
 */
function dfrtheme_preprocess_html(&$variables) {
  $variables['doctype'] = _dfrtheme_doctype();

  if ($variables['is_admin']) {
    $variables['classes_array'][] = 'admin';
  }

  if (!$variables['is_front']) {
    // Add unique classes for each page and website section.
    $path = drupal_get_path_alias($_GET['q']);
    $temp = explode('/', $path, 2);
    $section = array_shift($temp);
    $page_name = array_shift($temp);

    if (isset($page_name)) {
      $variables['classes_array'][] = dfrtheme_id_safe('page-' . $page_name);
    }

    $variables['classes_array'][] = dfrtheme_id_safe('section-' . $section);

    // Add template suggestions.
    $variables['theme_hook_suggestions'][] = "page__section__" . $section;
    $variables['theme_hook_suggestions'][] = "page__" . $page_name;

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          // Remove 'section-node'.
          array_pop($variables['classes_array']);
        }
        // Add 'section-node-add'.
        $body_classes[] = 'section-node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          // Remove 'section-node'.
          array_pop($variables['classes_array']);
        }
        // Add 'section-node-edit' or 'section-node-delete'.
        $body_classes[] = 'section-node-' . arg(2);
      }
    }
  }
}

/**
 * Implements template_preprocess_page().
 */
function dfrtheme_preprocess_page(&$variables) {
  if (isset($variables['node_title'])) {
    $variables['title'] = $variables['node_title'];
  }
  // Replace page title by a link to the original article on full page of
  // planet's articles.
  if (isset($variables['node'])) {
    $node = $variables['node'];
    if ($node->type == 'planete') {
      $variables['title'] = l($node->title, $node->field_planete_url[LANGUAGE_NONE][0]['url']);
    }
  }

  // Adding classes whether #navigation is here or not.
  if (!empty($variables['main_menu']) or !empty($variables['sub_menu'])) {
    $variables['classes_array'][] = 'with-navigation';
  }

  if (!empty($variables['secondary_menu'])) {
    $variables['classes_array'][] = 'with-subnav';
  }

  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render
    // elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
  $variables['site_slogan'] = variable_get('site_slogan', FALSE);
}

/**
 * Implements template_preprocess_maintenance_page().
 */
function dfrtheme_preprocess_maintenance_page(&$variables) {
  // Manually include these as they're not available outside
  // template_preprocess_page().
  $variables['grddl_profile'] = 'http://www.w3.org/1999/xhtml/vocab';

  $variables['doctype'] = _dfrtheme_doctype();

  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_css(drupal_get_path('theme', 'dfrtheme') . '/css/maintenance-page.css');
}

/**
 * Implements template_preprocess_node().
 *
 * Adds extra classes to node container for advanced theming.
 */
function dfrtheme_preprocess_node(&$variables) {
  // Add view_mode class.
  $variables['classes_array'][] = 'node-' . $variables['view_mode'];

  $variables['submitted'] = t('Submitted by !username on ', array('!username' => $variables['name']));
  $variables['submitted_date'] = t('!datetime', array('!datetime' => $variables['date']));
  $variables['submitted_pubdate'] = format_date($variables['created'], 'custom', 'Y-m-d\TH:i:s');

  // Change the date format for the planet.
  if ($variables['type'] == 'planete' && $variables['view_mode'] == 'teaser') {
    $variables['submitted_pubdate'] = format_date($variables['created'], 'custom', 'l j F Y');
  }
  $variables['theme_hook_suggestions'][] = 'node__' . $variables['node']->type . '__' . $variables['view_mode'];
}

/**
 * Implements template_preprocess_user().
 *
 * Adds extra classes to node container for advanced theming.
 */
function dfrtheme_preprocess_user_profile(&$variables) {
  if ($variables['elements']['#view_mode'] == 'planete_author') {
    $variables['user_link'] = l($variables['elements']['#account']->name, 'user/' . $variables['elements']['#account']->uid);
    $variables['theme_hook_suggestions'][] = 'user_profile__' . $variables['elements']['#view_mode'];
  }
}

/**
 * Implements template_preprocess_block().
 */
function dfrtheme_preprocess_block(&$variables) {
  // Add a striping class & id.
  $variables['classes_array'][] = 'block-' . $variables['zebra'];
  $variables['classes_array'][] = 'block-' . $variables['block_id'];

  // Title class.
  $variables['title_attributes_array']['class'][] = 'block-title';

  // In the header region visually hide block titles.
  if (in_array($variables['block']->region, array('menu'))) {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
}

/**
 * Implements theme_menu_tree().
 */
function dfrtheme_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}

/**
 * Return a themed breadcrumb trail.
 */
function dfrtheme_breadcrumb($variables) {
  $breadcrumb = isset($variables['breadcrumb']) ? $variables['breadcrumb'] : array();

  // Append title to breadcrumb.
  $title = drupal_get_title();
  if (!empty($title)) {
    $breadcrumb[] = '<span class="current" title="Vous êtes ici">' . $title . '</span>';
  }

  // >>.
  return implode(' &raquo; ', $breadcrumb);
}

/**
 * Determine whether to show floating tabs.
 *
 * @return bool
 */
function dfrtheme_tabs_float() {
  $float = (bool) theme_get_setting('dfrtheme_tabs_float');
  $float_node = (bool) theme_get_setting('dfrtheme_tabs_node');
  $is_node = (arg(0) === 'node' && is_numeric(arg(1)));

  if ($float) {
    return ($float_node) ? $is_node : TRUE;
  }
  return FALSE;
}

/**
 * Converts a string to a suitable html ID attribute. Taken from "basic".
 *
 * http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 * valid ID attribute in HTML. This function:
 * - Ensures an ID starts with an alpha character by optionally adding an 'n'.
 * - Replaces any character except A-Z, numbers, and underscores with dashes.
 * - Converts entire string to lowercase.
 *
 * @param string $string
 *   The string.
 *
 * @return string
 *   The converted string.
 */
function dfrtheme_id_safe($string) {
  // Strip accents.
  $accents = '/&([A-Za-z]{1,2})(tilde|grave|acute|circ|cedil|uml|lig);/';
  $string = preg_replace($accents, '$1', htmlentities(utf8_decode($string)));
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or
  // underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  // If the first character is not a-z, add 'n' in front.
  // Don't use ctype_alpha since its locale aware.
  if (strlen($string) > 0 && !ctype_lower($string{0})) {
    $string = 'id' . $string;
  }
  return $string;
}

/**
 * Generate doctype for templates.
 */
function _dfrtheme_doctype() {
  return (module_exists('rdf')) ? '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN"' . "\n" . '"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">' : '<!DOCTYPE html>' . "\n";
}

/**
 * Generate the HTML output for a menu link and submenu.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return string
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function dfrtheme_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  // Adding a class depending on the TITLE of the link (not constant).
  $element['#attributes']['class'][] = dfrtheme_id_safe($element['#title']);
  // Adding a class depending on the ID of the link (constant).
  $element['#attributes']['class'][] = 'mid-' . $element['#original_link']['mlid'];

  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Implements theme_preprocess_menu_local_task().
 *
 * Override or insert variables into theme_menu_local_task().
 */
function dfrtheme_preprocess_menu_local_task(&$variables) {
  $link = & $variables['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function dfrtheme_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

/**
 * Implements hook_form_alter().
 */
function dfrtheme_form_alter(&$form, &$form_state, $form_id) {
  // User login form (block & page).
  if ($form_id == 'user_login_block' || $form_id == 'user_login') {
    // Re-order fields.
    $form['openid_identifier']['#weight'] = 0;
    $form['name']['#weight'] = 0;
    $form['pass']['#weight'] = 1;
    $form['actions']['#weight'] = 2;
    $form['openid_links']['#weight'] = 3;
    $form['links']['#weight'] = 4;
  }
}

/**
 * Implements theme_field().
 */
function dfrtheme_field($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . '&nbsp;:&nbsp;</div>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Implements hook_preprocess_views_view().
 */
function dfrtheme_preprocess_views_view(&$variables) {
  if ($variables['view']->name == 'offres' && $variables['view']->current_display == 'block_1') {
    $path = 'node/add/offre';
    $options = array();

    // Force users to login first and then bring them back to the form.
    if (user_is_anonymous()) {
      $path = 'user/login';
      $options['query'] = array('destination' => 'node/add/offre');
    }

    $variables['empty'] = "<p>Il n'y a aucune offre pour le moment, pourquoi ne pas " . l(t('proposer la vôtre'), $path, $options) . " ?</p>";
    $link_create_offer = l(t('Déposer une offre'), 'node/add/offre', array('attributes' => array('class' => array('btn-link'))));
    $link_see_offers = l(t('Voir toutes les offres'), 'emploi/', array('attributes' => array('class' => array('btn-link'))));

    // Update link path if you are logged in or not.
    if ($variables['view']->total_rows > 0) {
      if (user_is_anonymous()) {
        $link_create_offer = l(t('Déposer une offre'), 'user/login', array('query' => array('destination' => 'node/add/offre'), 'attributes' => array('class' => array('btn-link'))));
      }
    }
    $footer = $link_create_offer . ' ' . $link_see_offers;
    $variables['footer'] = $footer;
  }
}

/**
 * Implements hook_theme().
 */
function dfrtheme_theme() {
  return array(
    'forum_list' => array(
      'template' => 'forum-list',
      'path' => drupal_get_path('theme', 'dfrtheme') . '/templates/forum/',
      'variables' => array('forums' => NULL, 'parents' => NULL, 'tid' => NULL),
    ),
    'forum_submitted' => array(
      'template' => 'forum-submitted',
      'path' => drupal_get_path('theme', 'dfrtheme') . '/templates/forum/',
      'variables' => array('topic' => NULL),
    ),
  );
}
