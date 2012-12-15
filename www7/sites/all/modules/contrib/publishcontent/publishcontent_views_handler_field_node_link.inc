<?php

/**
 * Field handler to present a link node publish.
 */
class publishcontent_views_handler_field_node_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['status'] = 'status';
    $this->additional_fields['uid'] = 'uid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['publish'] = 
    $options['unpublish'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['publish'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for publishing'),
      '#default_value' => $this->options['publish'],
    );
    $form['unpublish'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display for unpublishing'),
      '#default_value' => $this->options['unpublish'],
    );
  }


  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // ensure user has access to edit this node.
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->type = $values->{$this->aliases['type']};
    $node->status = $values->{$this->aliases['status']};
    $node->uid = $values->{$this->aliases['uid']};

    $options = array('query' => drupal_get_destination());

    if ($node->status && _publishcontent_unpublish_access($node)) {
      $op = 'unpublish';
    }

    if (!$node->status && _publishcontent_publish_access($node)) {
      $op = 'publish';
    }

    if (isset($op)) {
      $options['attributes'] = array('class' => array('publishcontent-link', 'publishcontent-' . $op));
      return l(empty($this->options[$op]) ? t($op) : $this->options[$op],
               "node/$node->nid/$op/". drupal_get_token(), $options);
    }
  }
}
