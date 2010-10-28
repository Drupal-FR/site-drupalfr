<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _drupalfr_workflow_content_default_fields() {
  $fields = array();

  // Exported field: field_workflow_status
  $fields[] = array(
    'field_name' => 'field_workflow_status',
    'type_name' => 'forum',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_plain' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_html' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'pending|Ouverte
accepted|Réponse acceptée
closed|Fermée',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => 'pending',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Statut',
      'weight' => '-3',
      'description' => '',
      'type' => 'optionwidgets_select',
      'module' => 'optionwidgets',
    ),
  );

  // Translatables
  array(
    t('Statut'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_rules_defaults().
 */
function _drupalfr_workflow_rules_defaults() {
  return array(
    'rules' => array(
      'drupalfr_workflow_1' => array(
        '#type' => 'rule',
        '#set' => 'event_comment_publish',
        '#label' => 'Comment is published',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'drupalfr_workflow' => 'drupalfr_workflow',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'Utilisateur',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '1',
              '#argument map' => array(
                'user' => 'comment_author',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'drupalfr_workflow_2' => array(
        '#type' => 'rule',
        '#set' => 'event_comment_unpublish',
        '#label' => 'Comment is unpublished',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'drupalfr_workflow' => 'drupalfr_workflow',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#type' => 'action',
            '#settings' => array(
              'points' => '-1',
              '#argument map' => array(
                'user' => 'comment_author',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'Utilisateur',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
          ),
        ),
        '#version' => 6003,
      ),
      'drupalfr_workflow_5' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_flagged_accepted_answer',
        '#label' => 'Comment is flagged',
        '#active' => 1,
        '#weight' => '1',
        '#categories' => array(
          'drupalfr_workflow' => 'drupalfr_workflow',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Load a user account',
              'new variables' => array(
                'flagged_comment_author' => array(
                  'label' => 'flagged comment author',
                  'label callback' => FALSE,
                  'type' => 'user',
                ),
              ),
              'module' => 'User',
              'eval input' => array(
                '0' => 'userid',
                '1' => 'username',
              ),
            ),
            '#name' => 'rules_action_load_user',
            '#settings' => array(
              'username' => '',
              'userid' => '[comment:comment-author-uid]',
              '#argument map' => array(
                'user_loaded' => 'flagged_comment_author',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'userid' => array(
                    '0' => 'comment',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
          '1' => array(
            '#type' => 'action',
            '#settings' => array(
              'points' => '10',
              '#argument map' => array(
                'user' => 'flagged_comment_author',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'Utilisateur',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#weight' => 1,
          ),
          '2' => array(
            '#weight' => 2,
            '#info' => array(
              'label' => 'Populate le contenu du commentaire flaggé\'s field \'field_workflow_status\'',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Contenu',
                ),
              ),
              'eval input' => array(
                '0' => 'code',
                '1' => 'value|0|value',
              ),
              'module' => 'CCK',
            ),
            '#name' => 'content_rules_action_populate_field',
            '#settings' => array(
              'field_name' => 'field_workflow_status',
              '#argument map' => array(
                'node' => 'node',
              ),
              'value' => array(
                '0' => array(
                  'value' => 'accepted',
                ),
              ),
              'code' => '',
              'vars' => array(),
            ),
            '#type' => 'action',
          ),
          '3' => array(
            '#type' => 'action',
            '#settings' => array(
              'title' => '[Résolu] [node:title]',
              '#argument map' => array(
                'node' => 'node',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'title' => array(
                    '0' => 'node',
                  ),
                ),
              ),
            ),
            '#name' => 'rules_action_set_node_title',
            '#info' => array(
              'label' => 'Set the flagged comment\'s content\'s title',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Contenu',
                ),
                'title' => array(
                  'type' => 'string',
                  'label' => 'Titre',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 3,
          ),
          '4' => array(
            '#type' => 'action',
            '#settings' => array(
              'immediate' => 0,
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_save_var_node',
            '#info' => array(
              'module' => 'Node',
              'base' => 'rules_action_save_variable',
              'label' => 'Save a contenu',
              'arguments' => array(
                'var_name' => array(
                  'type' => 'value',
                  'default value' => 'node',
                ),
                'node' => array(
                  'type' => 'node',
                  'label' => 'contenu',
                ),
              ),
            ),
            '#weight' => 4,
          ),
        ),
        '#version' => 6003,
      ),
      'drupalfr_workflow_6' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_unflagged_accepted_answer',
        '#label' => 'Comment is unflagged',
        '#active' => 1,
        '#weight' => '1',
        '#categories' => array(
          'drupalfr_workflow' => 'drupalfr_workflow',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'username' => '',
              'userid' => '[comment:comment-author-uid]',
              '#argument map' => array(
                'user_loaded' => 'flagged_comment_author',
              ),
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'userid' => array(
                    '0' => 'comment',
                  ),
                ),
              ),
            ),
            '#name' => 'rules_action_load_user',
            '#info' => array(
              'label' => 'Load a user account',
              'new variables' => array(
                'flagged_comment_author' => array(
                  'label' => 'flagged comment author',
                  'label callback' => FALSE,
                  'type' => 'user',
                ),
              ),
              'module' => 'User',
              'eval input' => array(
                '0' => 'userid',
                '1' => 'username',
              ),
            ),
            '#weight' => 0,
          ),
          '1' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'Utilisateur',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '-10',
              '#argument map' => array(
                'user' => 'flagged_comment_author',
              ),
            ),
            '#type' => 'action',
          ),
          '2' => array(
            '#type' => 'action',
            '#settings' => array(
              'field_name' => 'field_workflow_status',
              '#argument map' => array(
                'node' => 'node',
              ),
              'value' => array(
                '0' => array(
                  'value' => 'pending',
                ),
              ),
              'code' => '',
              'vars' => array(),
            ),
            '#name' => 'content_rules_action_populate_field',
            '#info' => array(
              'label' => 'Populate le contenu du commentaire flaggé\'s field \'field_workflow_status\'',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Contenu',
                ),
              ),
              'eval input' => array(
                '0' => 'code',
                '1' => 'value|0|value',
              ),
              'module' => 'CCK',
            ),
            '#weight' => 0,
          ),
          '3' => array(
            '#weight' => 0,
            '#info' => array(
              'module' => 'Node',
              'base' => 'rules_action_save_variable',
              'label' => 'Save a contenu',
              'arguments' => array(
                'var_name' => array(
                  'type' => 'value',
                  'default value' => 'node',
                ),
                'node' => array(
                  'type' => 'node',
                  'label' => 'contenu',
                ),
              ),
            ),
            '#name' => 'rules_save_var_node',
            '#settings' => array(
              'immediate' => 0,
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'drupalfr_workflow_3' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'Forum post is published',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          'drupalfr_workflow' => 'drupalfr_workflow',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'forum' => 'forum',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Created content is Sujet de forum',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Contenu',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'Utilisateur',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '1',
              '#argument map' => array(
                'user' => 'author',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}
