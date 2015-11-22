<?php
/**
 * @file
 * drupalfr_commerce.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function drupalfr_commerce_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'carte';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'carte';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'carte';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own licenses';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Appliquer';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Trier par';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Éléments par page';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Tout -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Décalage';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« premier';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ précédent';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'suivant ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'dernier »';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: User: Commerce License */
  $handler->display->display_options['relationships']['commerce_license']['id'] = 'commerce_license';
  $handler->display->display_options['relationships']['commerce_license']['table'] = 'users';
  $handler->display->display_options['relationships']['commerce_license']['field'] = 'commerce_license';
  $handler->display->display_options['relationships']['commerce_license']['required'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: User: Nom réel */
  $handler->display->display_options['fields']['field_realname']['id'] = 'field_realname';
  $handler->display->display_options['fields']['field_realname']['table'] = 'field_data_field_realname';
  $handler->display->display_options['fields']['field_realname']['field'] = 'field_realname';
  $handler->display->display_options['fields']['field_realname']['label'] = '';
  $handler->display->display_options['fields']['field_realname']['element_label_colon'] = FALSE;
  /* Field: User: Numéro adhérent */
  $handler->display->display_options['fields']['field_membership_number']['id'] = 'field_membership_number';
  $handler->display->display_options['fields']['field_membership_number']['table'] = 'field_data_field_membership_number';
  $handler->display->display_options['fields']['field_membership_number']['field'] = 'field_membership_number';
  $handler->display->display_options['fields']['field_membership_number']['label'] = '';
  $handler->display->display_options['fields']['field_membership_number']['element_label_colon'] = FALSE;
  /* Field: Commerce License: Expires */
  $handler->display->display_options['fields']['expires']['id'] = 'expires';
  $handler->display->display_options['fields']['expires']['table'] = 'commerce_license';
  $handler->display->display_options['fields']['expires']['field'] = 'expires';
  $handler->display->display_options['fields']['expires']['relationship'] = 'commerce_license';
  $handler->display->display_options['fields']['expires']['label'] = '';
  $handler->display->display_options['fields']['expires']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expires']['date_format'] = 'custom';
  $handler->display->display_options['fields']['expires']['custom_date_format'] = 'd/m/Y';
  $handler->display->display_options['fields']['expires']['second_date_format'] = 'long';
  /* Sort criterion: Commerce License: Expires */
  $handler->display->display_options['sorts']['expires']['id'] = 'expires';
  $handler->display->display_options['sorts']['expires']['table'] = 'commerce_license';
  $handler->display->display_options['sorts']['expires']['field'] = 'expires';
  $handler->display->display_options['sorts']['expires']['relationship'] = 'commerce_license';
  $handler->display->display_options['sorts']['expires']['order'] = 'DESC';
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['exception']['title'] = 'Tout';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'current_user_or_role';
  $handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
    4 => '4',
  );
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Commerce License: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_license';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'commerce_license';
  $handler->display->display_options['filters']['status_1']['value'] = array(
    2 => '2',
  );

  /* Display: PDF Page */
  $handler = $view->new_display('pdf', 'PDF Page', 'pdf_carte');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'pdf_unformatted';
  $handler->display->display_options['row_plugin'] = 'pdf_fields';
  $handler->display->display_options['row_options']['formats'] = array(
    'name' => array(
      'position' => array(
        'object' => 'page',
        'corner' => 'top_left',
        'x' => '',
        'y' => '4',
        'width' => '',
        'height' => '',
      ),
      'text' => array(
        'font_size' => '',
        'font_family' => 'default',
        'font_style' => array(
          'b' => 0,
          'i' => 0,
          'u' => 0,
          'd' => 0,
          'o' => 0,
        ),
        'align' => NULL,
        'hyphenate' => NULL,
        'color' => '',
      ),
      'render' => array(
        'is_html' => 1,
        'minimal_space' => '1',
        'eval_before' => '',
        'bypass_eval_before' => 0,
        'eval_after' => '',
        'bypass_eval_after' => 0,
      ),
    ),
    'field_realname' => array(
      'position' => array(
        'object' => 'page',
        'corner' => 'top_left',
        'x' => '',
        'y' => '12',
        'width' => '',
        'height' => '',
      ),
      'text' => array(
        'font_size' => '',
        'font_family' => 'default',
        'font_style' => array(
          'b' => 0,
          'i' => 0,
          'u' => 0,
          'd' => 0,
          'o' => 0,
        ),
        'align' => NULL,
        'hyphenate' => NULL,
        'color' => '',
      ),
      'render' => array(
        'is_html' => 1,
        'minimal_space' => '1',
        'eval_before' => '',
        'bypass_eval_before' => 0,
        'eval_after' => '',
        'bypass_eval_after' => 0,
      ),
    ),
    'field_membership_number' => array(
      'position' => array(
        'object' => 'page',
        'corner' => 'top_left',
        'x' => '5',
        'y' => '38',
        'width' => '',
        'height' => '',
      ),
      'text' => array(
        'font_size' => '',
        'font_family' => 'default',
        'font_style' => array(
          'b' => 0,
          'i' => 0,
          'u' => 0,
          'd' => 0,
          'o' => 0,
        ),
        'align' => NULL,
        'hyphenate' => NULL,
        'color' => '',
      ),
      'render' => array(
        'is_html' => 1,
        'minimal_space' => '1',
        'eval_before' => '',
        'bypass_eval_before' => 0,
        'eval_after' => '',
        'bypass_eval_after' => 0,
      ),
    ),
    'expires' => array(
      'position' => array(
        'object' => 'page',
        'corner' => 'top_left',
        'x' => '42',
        'y' => '38',
        'width' => '',
        'height' => '',
      ),
      'text' => array(
        'font_size' => '',
        'font_family' => 'default',
        'font_style' => array(
          'b' => 0,
          'i' => 0,
          'u' => 0,
          'd' => 0,
          'o' => 0,
        ),
        'align' => NULL,
        'hyphenate' => NULL,
        'color' => '',
      ),
      'render' => array(
        'is_html' => 1,
        'minimal_space' => '1',
        'eval_before' => '',
        'bypass_eval_before' => 0,
        'eval_after' => '',
        'bypass_eval_after' => 0,
      ),
    ),
  );
  $handler->display->display_options['row_options']['leading_template'] = '0';
  $handler->display->display_options['row_options']['template'] = 'carte_adherent_verso.pdf';
  $handler->display->display_options['row_options']['succeed_template'] = '0';
  $handler->display->display_options['path'] = 'user/%/carte.pdf';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Carte';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['leading_template'] = '0';
  $handler->display->display_options['template'] = 'carte_adherent_verso.pdf';
  $handler->display->display_options['succeed_template'] = 'carte_adherent_recto.pdf';
  $translatables['carte'] = array(
    t('Master'),
    t('carte'),
    t('plus'),
    t('Appliquer'),
    t('Réinitialiser'),
    t('Trier par'),
    t('Asc'),
    t('Desc'),
    t('Éléments par page'),
    t('- Tout -'),
    t('Décalage'),
    t('« premier'),
    t('‹ précédent'),
    t('suivant ›'),
    t('dernier »'),
    t('Commerce License'),
    t('Tout'),
    t('PDF Page'),
    t('more'),
  );
  $export['carte'] = $view;

  $view = new view();
  $view->name = 'members';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Adhérents';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Adhérents';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Appliquer';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Trier par';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Éléments par page';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Tout -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Décalage';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« premier';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ précédent';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'suivant ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'dernier »';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'order_id' => 'order_id',
    'license_id' => 'license_id',
    'field_membership_number' => 'field_membership_number',
    'name' => 'name',
    'mail_1' => 'mail_1',
    'commerce_customer_billing' => 'commerce_customer_billing',
    'commerce_billy_pdf_invoice_link' => 'commerce_billy_pdf_invoice_link',
    'field_receive_membership_card' => 'field_receive_membership_card',
    'granted' => 'granted',
  );
  $handler->display->display_options['style_options']['default'] = 'order_id';
  $handler->display->display_options['style_options']['info'] = array(
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'license_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_membership_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_customer_billing' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_billy_pdf_invoice_link' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_receive_membership_card' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'granted' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  $handler->display->display_options['header']['result']['content'] = 'Afichage @start - @end sur @total';
  /* Relationship: Commerce Order: Owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: Commerce Order: Referenced line items */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Relationship: User: Commerce License */
  $handler->display->display_options['relationships']['commerce_license']['id'] = 'commerce_license';
  $handler->display->display_options['relationships']['commerce_license']['table'] = 'users';
  $handler->display->display_options['relationships']['commerce_license']['field'] = 'commerce_license';
  $handler->display->display_options['relationships']['commerce_license']['relationship'] = 'uid';
  $handler->display->display_options['relationships']['commerce_license']['required'] = TRUE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = 'Commande';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Commerce License: License ID */
  $handler->display->display_options['fields']['license_id']['id'] = 'license_id';
  $handler->display->display_options['fields']['license_id']['table'] = 'commerce_license';
  $handler->display->display_options['fields']['license_id']['field'] = 'license_id';
  $handler->display->display_options['fields']['license_id']['relationship'] = 'commerce_license';
  $handler->display->display_options['fields']['license_id']['label'] = 'License';
  /* Field: User: Numéro adhérent */
  $handler->display->display_options['fields']['field_membership_number']['id'] = 'field_membership_number';
  $handler->display->display_options['fields']['field_membership_number']['table'] = 'field_data_field_membership_number';
  $handler->display->display_options['fields']['field_membership_number']['field'] = 'field_membership_number';
  $handler->display->display_options['fields']['field_membership_number']['relationship'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Pseudo';
  /* Field: User: E-mail */
  $handler->display->display_options['fields']['mail_1']['id'] = 'mail_1';
  $handler->display->display_options['fields']['mail_1']['table'] = 'users';
  $handler->display->display_options['fields']['mail_1']['field'] = 'mail';
  $handler->display->display_options['fields']['mail_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['mail_1']['label'] = 'Courriel';
  $handler->display->display_options['fields']['mail_1']['link_to_user'] = 'user';
  /* Field: Commerce Order: Adresse de facturation */
  $handler->display->display_options['fields']['commerce_customer_billing']['id'] = 'commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['table'] = 'field_data_commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['field'] = 'commerce_customer_billing';
  /* Field: Commerce Order: Invoice PDF */
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['id'] = 'commerce_billy_pdf_invoice_link';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['table'] = 'views_entity_commerce_order';
  $handler->display->display_options['fields']['commerce_billy_pdf_invoice_link']['field'] = 'commerce_billy_pdf_invoice_link';
  /* Field: Commerce Line item: Recevoir carte adhérent */
  $handler->display->display_options['fields']['field_receive_membership_card']['id'] = 'field_receive_membership_card';
  $handler->display->display_options['fields']['field_receive_membership_card']['table'] = 'field_data_field_receive_membership_card';
  $handler->display->display_options['fields']['field_receive_membership_card']['field'] = 'field_receive_membership_card';
  $handler->display->display_options['fields']['field_receive_membership_card']['relationship'] = 'commerce_line_items_line_item_id';
  /* Field: Commerce License: Granted */
  $handler->display->display_options['fields']['granted']['id'] = 'granted';
  $handler->display->display_options['fields']['granted']['table'] = 'commerce_license';
  $handler->display->display_options['fields']['granted']['field'] = 'granted';
  $handler->display->display_options['fields']['granted']['relationship'] = 'commerce_license';
  $handler->display->display_options['fields']['granted']['label'] = 'Date adhésion';
  $handler->display->display_options['fields']['granted']['date_format'] = 'custom';
  $handler->display->display_options['fields']['granted']['custom_date_format'] = 'd/m/Y';
  $handler->display->display_options['fields']['granted']['second_date_format'] = 'long';
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    15 => '15',
  );
  $handler->display->display_options['filters']['rid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['operator_id'] = 'rid_op';
  $handler->display->display_options['filters']['rid']['expose']['label'] = 'Rôles';
  $handler->display->display_options['filters']['rid']['expose']['operator'] = 'rid_op';
  $handler->display->display_options['filters']['rid']['expose']['identifier'] = 'rid';
  $handler->display->display_options['filters']['rid']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
    14 => 0,
    15 => 0,
  );
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Actif';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
    14 => 0,
    15 => 0,
  );
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = array(
    'completed' => 'completed',
    'invoiced' => 'invoiced',
  );
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['relationship'] = 'uid';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'Courriel';
  $handler->display->display_options['filters']['mail']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Nom';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    1 => 0,
    13 => 0,
    11 => 0,
    7 => 0,
    12 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
  );
  /* Filter criterion: Commerce License: Status */
  $handler->display->display_options['filters']['status_2']['id'] = 'status_2';
  $handler->display->display_options['filters']['status_2']['table'] = 'commerce_license';
  $handler->display->display_options['filters']['status_2']['field'] = 'status';
  $handler->display->display_options['filters']['status_2']['relationship'] = 'commerce_license';
  $handler->display->display_options['filters']['status_2']['value'] = array(
    2 => '2',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/people/members';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Adhérents';
  $handler->display->display_options['menu']['name'] = 'management';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view-%timestamp-yyyy-%timestamp-mm-%timestamp-dd.xls';
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['path'] = 'admin/people/members/export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['members'] = array(
    t('Master'),
    t('Adhérents'),
    t('plus'),
    t('Appliquer'),
    t('Réinitialiser'),
    t('Trier par'),
    t('Asc'),
    t('Desc'),
    t('Éléments par page'),
    t('- Tout -'),
    t('Décalage'),
    t('« premier'),
    t('‹ précédent'),
    t('suivant ›'),
    t('dernier »'),
    t('Afichage @start - @end sur @total'),
    t('Order owner'),
    t('Line items referenced by commerce_line_items'),
    t('Commerce License'),
    t('Commande'),
    t('License'),
    t('.'),
    t(','),
    t('Numéro adhérent'),
    t('Pseudo'),
    t('Courriel'),
    t('Adresse de facturation'),
    t('Invoice PDF'),
    t('Recevoir carte adhérent'),
    t('Date adhésion'),
    t('Rôles'),
    t('Actif'),
    t('Nom'),
    t('Page'),
    t('more'),
    t('Data export'),
  );
  $export['members'] = $view;

  $view = new view();
  $view->name = 'membership';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_product';
  $view->human_name = 'Adhésion';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Adhésion individuelle';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access checkout';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Appliquer';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Trier par';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<p>Adhérer en tant que personne physique. L\'adhésion est annuelle.</p>
<p>L\'adhésion pour les personnes morales n\'est pas disponible en ligne pour l\'instant, veuillez nous <a href="/contact">contacter</a> pour toute demande.</p>';
  $handler->display->display_options['header']['area']['format'] = '3';
  /* Field: Commerce Product: Prix */
  $handler->display->display_options['fields']['commerce_price']['id'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['table'] = 'field_data_commerce_price';
  $handler->display->display_options['fields']['commerce_price']['field'] = 'commerce_price';
  $handler->display->display_options['fields']['commerce_price']['label'] = '';
  $handler->display->display_options['fields']['commerce_price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_price']['settings'] = array(
    'calculation' => '0',
  );
  /* Field: Commerce Product: Add to Cart form */
  $handler->display->display_options['fields']['add_to_cart_form']['id'] = 'add_to_cart_form';
  $handler->display->display_options['fields']['add_to_cart_form']['table'] = 'views_entity_commerce_product';
  $handler->display->display_options['fields']['add_to_cart_form']['field'] = 'add_to_cart_form';
  $handler->display->display_options['fields']['add_to_cart_form']['label'] = '';
  $handler->display->display_options['fields']['add_to_cart_form']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['add_to_cart_form']['show_quantity'] = 0;
  $handler->display->display_options['fields']['add_to_cart_form']['default_quantity'] = '1';
  $handler->display->display_options['fields']['add_to_cart_form']['combine'] = 0;
  $handler->display->display_options['fields']['add_to_cart_form']['display_path'] = 0;
  $handler->display->display_options['fields']['add_to_cart_form']['line_item_type'] = 'individual_membership';
  /* Filter criterion: Commerce Product: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'individual_membership' => 'individual_membership',
  );
  /* Filter criterion: Commerce Product: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Block adhésion individuelle sidebar */
  $handler = $view->new_display('block', 'Block adhésion individuelle sidebar', 'block_individual_membership_sidebar');

  /* Display: Block adhésion individuelle profile */
  $handler = $view->new_display('block', 'Block adhésion individuelle profile', 'block_individual_membership_profile');
  $translatables['membership'] = array(
    t('Master'),
    t('Adhésion individuelle'),
    t('plus'),
    t('Appliquer'),
    t('Réinitialiser'),
    t('Trier par'),
    t('Asc'),
    t('Desc'),
    t('<p>Adhérer en tant que personne physique. L\'adhésion est annuelle.</p>
<p>L\'adhésion pour les personnes morales n\'est pas disponible en ligne pour l\'instant, veuillez nous <a href="/contact">contacter</a> pour toute demande.</p>'),
    t('Block adhésion individuelle sidebar'),
    t('more'),
    t('Block adhésion individuelle profile'),
  );
  $export['membership'] = $view;

  $view = new view();
  $view->name = 'user_licenses';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_license';
  $view->human_name = 'User licenses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Licences';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'plus';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own licenses';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Appliquer';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Réinitialiser';
  $handler->display->display_options['exposed_form']['options']['exposed_sorts_label'] = 'Trier par';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_label'] = 'Éléments par page';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all_label'] = '- Tout -';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Décalage';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« premier';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ précédent';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'suivant ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'dernier »';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'license_id' => 'license_id',
    'status' => 'status',
    'expires' => 'expires',
    'granted' => 'granted',
  );
  $handler->display->display_options['style_options']['default'] = 'license_id';
  $handler->display->display_options['style_options']['info'] = array(
    'license_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'expires' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'granted' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<p>Suivi de votre adhésion à l\'association. L\'adhésion est valable un an de date à date. Les avantages réservés aux adhérents ne sont valables que pour les adhérents à jour de leur cotisation.</p>';
  $handler->display->display_options['header']['area']['format'] = '3';
  /* Field: Commerce License: License ID */
  $handler->display->display_options['fields']['license_id']['id'] = 'license_id';
  $handler->display->display_options['fields']['license_id']['table'] = 'commerce_license';
  $handler->display->display_options['fields']['license_id']['field'] = 'license_id';
  $handler->display->display_options['fields']['license_id']['label'] = 'Identifiant de demande';
  /* Field: Commerce License: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_license';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Commerce License: Granted */
  $handler->display->display_options['fields']['granted']['id'] = 'granted';
  $handler->display->display_options['fields']['granted']['table'] = 'commerce_license';
  $handler->display->display_options['fields']['granted']['field'] = 'granted';
  $handler->display->display_options['fields']['granted']['label'] = 'Date d\'activation';
  $handler->display->display_options['fields']['granted']['date_format'] = 'long';
  $handler->display->display_options['fields']['granted']['second_date_format'] = 'long';
  /* Field: Commerce License: Expires */
  $handler->display->display_options['fields']['expires']['id'] = 'expires';
  $handler->display->display_options['fields']['expires']['table'] = 'commerce_license';
  $handler->display->display_options['fields']['expires']['field'] = 'expires';
  $handler->display->display_options['fields']['expires']['label'] = 'Date d\'expiration';
  $handler->display->display_options['fields']['expires']['date_format'] = 'long';
  $handler->display->display_options['fields']['expires']['second_date_format'] = 'long';
  /* Contextual filter: Commerce License: Owner uid */
  $handler->display->display_options['arguments']['owner']['id'] = 'owner';
  $handler->display->display_options['arguments']['owner']['table'] = 'commerce_license';
  $handler->display->display_options['arguments']['owner']['field'] = 'owner';
  $handler->display->display_options['arguments']['owner']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['owner']['exception']['title'] = 'Tout';
  $handler->display->display_options['arguments']['owner']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['owner']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['owner']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['owner']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['owner']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['owner']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['owner']['validate']['type'] = 'current_user_or_role';
  $handler->display->display_options['arguments']['owner']['validate_options']['restrict_roles'] = TRUE;
  $handler->display->display_options['arguments']['owner']['validate_options']['roles'] = array(
    4 => '4',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/adhesion';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Mon adhésion';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['user_licenses'] = array(
    t('Master'),
    t('Licences'),
    t('plus'),
    t('Appliquer'),
    t('Réinitialiser'),
    t('Trier par'),
    t('Asc'),
    t('Desc'),
    t('Éléments par page'),
    t('- Tout -'),
    t('Décalage'),
    t('« premier'),
    t('‹ précédent'),
    t('suivant ›'),
    t('dernier »'),
    t('<p>Suivi de votre adhésion à l\'association. L\'adhésion est valable un an de date à date. Les avantages réservés aux adhérents ne sont valables que pour les adhérents à jour de leur cotisation.</p>'),
    t('Identifiant de demande'),
    t('.'),
    t(','),
    t('Status'),
    t('Date d\'activation'),
    t('Date d\'expiration'),
    t('Tout'),
    t('Page'),
    t('more'),
  );
  $export['user_licenses'] = $view;

  return $export;
}
