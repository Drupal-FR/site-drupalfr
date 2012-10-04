<?php
/**
 * Argument handler to accept a file id.
 */
class views_handler_argument_file_fid extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the file.
   */
  function title_query() {
    $titles = array();
    $result = db_query(db_rewrite_sql('SELECT f.fid, f.filename FROM {files} f WHERE f.fid IN (' . db_placeholders($this->value, 'int') . ')', 'f', 'fid'), $this->value);
    while ($file = db_fetch_object($result)) {
      $titles[] = check_plain($file->filename);
    }
    return $titles;
  }
}
