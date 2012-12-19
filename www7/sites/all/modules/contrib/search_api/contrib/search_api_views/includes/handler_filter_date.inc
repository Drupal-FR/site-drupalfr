<?php

/**
 * Views filter handler base class for handling all "normal" cases.
 */
class SearchApiViewsHandlerFilterDate extends SearchApiViewsHandlerFilter {

  /**
   * Provide a form for setting the filter value.
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['value']['#description'] = t('A date in any format understood by <a href="@doc-link">PHP</a>. For example, "@date1" or "@date2".', array(
      '@doc-link' => 'http://php.net/manual/en/function.strtotime.php',
      '@date1' => format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i:s'),
      '@date2' => 'now + 1 day',
    ));
  }

  /**
   * Add this filter to the query.
   */
  public function query() {
    if ($this->operator === 'empty') {
      $this->query->condition($this->real_field, NULL, '=', $this->options['group']);
    }
    elseif ($this->operator === 'not empty') {
      $this->query->condition($this->real_field, NULL, '<>', $this->options['group']);
    }
    else {
      while (is_array($this->value)) {
        $this->value = $this->value ? reset($this->value) : NULL;
      }
      $v = is_numeric($this->value) ? $this->value : strtotime($this->value, REQUEST_TIME);
      if ($v !== FALSE) {
        $this->query->condition($this->real_field, $v, $this->operator, $this->options['group']);
      }
    }
  }

}
