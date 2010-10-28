<?php
/**
 * Field templace for the timestamp field: use the relative time format.
 *
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 */
?>
<?php print drupalfr4_format_date($row->{$field->field_alias}); ?>
