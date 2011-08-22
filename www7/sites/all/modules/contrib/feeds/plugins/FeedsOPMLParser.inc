<?php
// $Id: FeedsOPMLParser.inc,v 1.7.2.3 2010/10/25 23:59:31 alexb Exp $

/**
 * @file
 * OPML Parser plugin.
 */

/**
 * Feeds parser plugin that parses OPML feeds.
 */
class FeedsOPMLParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    feeds_include_library('opml_parser.inc', 'opml_parser');
    $opml = opml_parser_parse($fetcher_result->getRaw());
    $result = new FeedsParserResult($opml['items']);
    $result->title = $opml['title'];
    return $result;
  }

  /**
   * Return mapping sources.
   */
  public function getMappingSources() {
    return array(
      'title' => array(
        'name' => t('Feed title'),
        'description' => t('Title of the feed.'),
      ),
      'xmlurl' => array(
        'name' => t('Feed URL'),
        'description' => t('URL of the feed.'),
      ),
    ) + parent::getMappingSources();
  }
}
