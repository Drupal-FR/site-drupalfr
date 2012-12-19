<?php

/**
 * Logic around Solr field schema information.
 */
class SearchApiSolrField {

  /**
   * @var array
   *   Human-readable labels for Solr schema properties.
   */
  public static $schemaLabels = array(
    'I' => 'Indexed',
    'T' => 'Tokenized',
    'S' => 'Stored',
    'M' => 'Multivalued',
    'V' => 'TermVector Stored',
    'o' => 'Store Offset With TermVector',
    'p' => 'Store Position With TermVector',
    'O' => 'Omit Norms',
    'L' => 'Lazy',
    'B' => 'Binary',
    'C' => 'Compressed',
    'f' => 'Sort Missing First',
    'l' => 'Sort Missing Last',
  );

  /**
   * @var stdclass
   *   The original field object.
   */
  protected $field;

  /**
   * @var array
   *   An array of schema properties for this field. This will be a subset of
   *   the SearchApiSolrField::schemaLabels array.
   */
  protected $schema;

  /**
   * Constructor.
   *
   * @param stdClass $field
   *   A field object from Solr's "Luke" servlet.
   */
  public function __construct($field) {
    $this->field = $field;
  }

  /**
   * Get the type of the Solr field, according to the Solr schema.
   *
   * Note that field types like "text", "boolean", and "date" are conventions,
   * but their presence and behavior are entirely determined by the particular
   * schema.xml file used by a Solr core.
   *
   * @return string
   *   The type of the Solr field.
   */
  public function getType() {
    return $this->field->type;
  }

  /**
   * Get an array of field properties.
   *
   * @return array
   *   An array of properties describing the solr schema. The array keys are
   *   single-character codes, and the values are human-readable labels. This
   *   will be a subset of the SearchApiSolrField::schemaLabels array.
   */
  public function getSchema() {
    if (!isset($this->schema)) {
      foreach (str_split(str_replace('-', '', $this->field->schema)) as $key) {
        $this->schema[$key] = isset(self::$schemaLabels[$key]) ? self::$schemaLabels[$key] : $key;
      }
    }
    return $this->schema;
  }

  /**
   * Get the "dynamic base" of this field.
   *
   * This typically looks like 'ss_*, and is used to aggregate fields based on
   * "hungarian" naming conventions.
   *
   * @return string
   *   The mask describing the solr aggregate field, if there is one.
   */
  public function getDynamicBase() {
    return isset($this->field->dynamicBase) ? $this->field->dynamicBase : NULL;
  }

  /**
   * Determine whether this field may be suitable for use as a key field.
   *
   * Unfortunately, it seems like the best way to find an actual uniqueKey field
   * according to Solr is to examine the Solr core's schema.xml.
   *
   * @return boolean
   *   Whether the field is suitable for use as a key.
   */
  public function isPossibleKey() {
    return !$this->getDynamicBase()
      && !in_array($this->getType(), array('boolean', 'date', 'text'))
      && $this->isStored()
      && !$this->isMultivalued();
  }

  /**
   * Determine whether a field is suitable for sorting.
   *
   * In order for a field to yield useful sorted results in Solr, it must be
   * indexed, not multivalued, and not tokenized. It's ok if a field is
   * tokenized and yields only one token, but there's no general way to check
   * for that.
   *
   * @return boolean
   *   Whether the field is suitable for sorting.
   */
  public function isSortable() {
    return $this->isIndexed()
      && !$this->isMultivalued()
      && !$this->isTokenized();
  }

  /**
   * The following functions return information about specific properties of this field.
   *
   * @return boolean
   */
  public function isIndexed() {
    $this->getSchema();
    return isset($this->schema['I']);
  }
  public function isTokenized() {
    $this->getSchema();
    return isset($this->schema['T']);
  }
  public function isStored() {
    $this->getSchema();
    return isset($this->schema['S']);
  }
  public function isMultivalued() {
    $this->getSchema();
    return isset($this->schema['M']);
  }
  public function isTermVectorStored() {
    $this->getSchema();
    return isset($this->schema['V']);
  }
  public function isStoreOffsetWithTermVector() {
    $this->getSchema();
    return isset($this->schema['o']);
  }
  public function isStorePositionWithTermVector() {
    $this->getSchema();
    return isset($this->schema['p']);
  }
  public function isOmitNorms() {
    $this->getSchema();
    return isset($this->schema['O']);
  }
  public function isLazy() {
    $this->getSchema();
    return isset($this->schema['L']);
  }
  public function isBinary() {
    $this->getSchema();
    return isset($this->schema['B']);
  }
  public function isCompressed() {
    $this->getSchema();
    return isset($this->schema['C']);
  }
  public function isSortMissingFirst() {
    $this->getSchema();
    return isset($this->schema['f']);
  }
  public function isSortMissingLast() {
    $this->getSchema();
    return isset($this->schema['l']);
  }
}
