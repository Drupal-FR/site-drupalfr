<?php
/**
 * @file
 * Contains the SearchApiSpellcheckSolr class.
 */

/**
 * Spellcheck class which can provide spelling suggestions. The constructor
 * populates the instance with any suggestions returned by Solr.
 */
class SearchApiSpellcheckSolr extends SearchApiSpellcheck {

  /**
   * Constructor.
   *
   * If solr has returned spelling suggestion then loop through them and add
   * them to this spellcheck service.
   *
   * @param Apache_Solr_Response $response
   *   The Solr response object.
   */
  function __construct(Apache_Solr_Response $response) {
    if (isset($response->spellcheck->suggestions)) {
      $suggestions = $response->spellcheck->suggestions;
      foreach ($suggestions as $word => $data) {
        foreach ($data->suggestion as $suggestion) {
          $this->addSuggestion(new SearchApiSpellcheckSuggestion($word, $suggestion));
        }
      }
    }
  }

}
