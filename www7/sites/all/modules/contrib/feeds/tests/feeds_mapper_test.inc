<?php
// $Id: feeds_mapper_test.inc,v 1.2.4.2 2010/11/01 16:58:21 tristanoneil Exp $

/**
 * @file
 * Helper class with auxiliary functions for feeds mapper module tests.
 */

// Require FeedsWebTestCase class definition.
require_once(dirname(__FILE__) .'/feeds.test.inc');

/**
 * Base class for implementing Feeds Mapper test cases.
 */
class FeedsMapperTestCase extends FeedsWebTestCase {

  // A lookup map to select the widget for each field type.
  private static $field_widgets = array(
    'date' => 'date_text',
    'datestamp' => 'date_text',
    'datetime' => 'date_text',
    'number_decimal' => 'number',
    'email' => 'email_textfield',
    'emimage' => 'emimage_textfields',
    'emaudio' => 'emaudio_textfields',
    'filefield' => 'filefield_widget',
    'image' => 'imagefield_widget',
    'link_field' => 'link_field',
    'number_float' => 'number',
    'number_integer' => 'number',
    'nodereference' => 'nodereference_select',
    'text' => 'text_textfield',
    'userreference' => 'userreference_select',
   );

  /**
   * Assert that a form field for the given field with the given value
   * exists in the current form.
   *
   * @param $field_name
   *   The name of the field.
   * @param $value
   *   The (raw) value expected for the field.
   * @param $index
   *   The index of the field (for q multi-valued field).
   *
   * @see FeedsMapperTestCase::getFormFieldsNames()
   * @see FeedsMapperTestCase::getFormFieldsValues()
   */
  protected function assertNodeFieldValue($field_name, $value, $index = 0) {
    $names = $this->getFormFieldsNames($field_name, $index);
    $values = $this->getFormFieldsValues($field_name, $value);
    foreach($names as $k => $name){
      $value = $values[$k];
      $this->assertFieldByName($name, $value, t('Found form field %name for %field_name with the expected value.', array('%name' => $name, '%field_name' => $field_name)));
    }
  }

  /**
   * Returns the form fields names for a given CCK field. Default implementation
   * provides support for a single form field with the following name pattern
   * <code>"field_{$field_name}[{$index}][value]"</code>
   *
   * @param $field_name
   *   The name of the CCK field.
   * @param $index
   *   The index of the field (for q multi-valued field).
   *
   * @return
   *   An array of form field names.
   */
  protected function getFormFieldsNames($field_name, $index) {
    return array("field_{$field_name}[und][{$index}][value]");
  }

  /**
   * Returns the form fields values for a given CCK field. Default implementation
   * returns a single element array with $value casted to a string.
   *
   * @param $field_name
   *   The name of the CCK field.
   * @param $value
   *   The (raw) value expected for the CCK field.
   * @return An array of form field values.
   */
  protected function getFormFieldsValues($field_name, $value) {
    return array((string)$value);
  }

  /**
   * Create a new content-type, and add a field to it. Mostly copied from
   * cck/tests/content.crud.test ContentUICrud::testAddFieldUI
   *
   * @param $typename
   *   (Optional) the name of the content-type to create, defaults to a random name.
   * @param $fields
   *   (Optional) an keyed array of $field_name => $field_type used to add additional
   *   fields to the new content type.
   *
   * @return
   *   The name of the new typename.
   */
  final protected function createContentType($typename = NULL, $fields = array()) {
    if ($typename == NULL) {
      $typename = strtolower($this->randomName());
    }

    // Create the content type.
    $edit = array(
      'type' => $typename,
      'name' => $typename,
    );
    $this->drupalPost('admin/structure/types/add', $edit, 'Save content type');
    $this->assertText('The content type ' . $typename . ' has been added', 'Content type created');

    $admin_type_url = 'admin/structure/types/manage/'. str_replace('_', '-', $typename);

    // Create the fields
    foreach ($fields as $field_name => $options) {
      if(is_string($options)) {
        $options = array('type' => $options);
      }
      $field_type = isset($options['type']) ? $options['type'] : 'text';
      $field_widget = isset($options['widget']) ? $options['widget'] : $this->selectFieldWidget($field_name, $field_type);
      $this->assertTrue($field_widget !== NULL, "Field type $field_type supported");
      $label = $field_name . '_' . $field_type . '_label';
      $edit = array(
        'fields[_add_new_field][label]' => $label,
        'fields[_add_new_field][field_name]' => $field_name,
        'fields[_add_new_field][type]' => $field_type,
        'fields[_add_new_field][widget_type]' => $field_widget,
      );
      $this->drupalPost($admin_type_url . '/fields', $edit, 'Save');

      // (Default) Configure the field.
      $edit = isset($options['settings']) ? $options['settings'] : array();
      $this->drupalPost(NULL, $edit, 'Save field settings');
      $this->assertText('Updated field ' . $label);
    }

    return $typename;
  }

  /**
   * Select the widget for the field. Default implementation provides widgets
   * for Date, Number, Text, Node reference, User reference, Email, Emfield,
   * Filefield, Image, and Link.
   *
   * Extracted as a method to allow test implementations to add widgets for
   * the tested CCK field type(s). $field_name allow to test the same
   * field type with different widget (is this useful ?)
   *
   * @param $field_name
   *   The name of the field.
   * @param $field_type
   *   The CCK type of the field.
   *
   * @return
   *   The widget for this field, or NULL if the field_type is not
   *   supported by this test class.
   */
  protected function selectFieldWidget($field_name, $field_type) {
    $field_widgets = FeedsMapperTestCase::$field_widgets;
    return isset($field_widgets[$field_type]) ? $field_widgets[$field_type] : NULL;
  }
}
