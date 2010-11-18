<?php
// $Id: webform_handler_field_is_draft.inc,v 1.1.2.1 2010/03/03 02:51:31 quicksketch Exp $

/**
* @file
* Views handler to display the draft status of a submission.
*/

/**
 * Field handler to show if submission is draft or not.
 *
 * @ingroup views_field_handlers
 */
class webform_handler_field_is_draft extends views_handler_field {
  function render($values) {
     $is_draft = $values->{$this->field_alias};
     return $is_draft ? t('draft') : t('completed');
  }
}