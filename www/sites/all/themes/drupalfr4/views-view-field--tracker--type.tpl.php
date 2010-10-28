<?php
 /**
  * Field templace for the node type field: use small icons to display type.
  *
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  */
?>
<?php
  $type = $row->{$field->field_alias};
  $image = path_to_theme() . '/images/type-' . str_replace('_', '-', $type) . '.png';
  if (file_exists($image)) {
    print theme('image', $image, $output, $output);
  }
  else {
    print ' ';
  }
?>
