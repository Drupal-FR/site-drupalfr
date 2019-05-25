#!/bin/bash

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh
. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.local.sh

echo -e "${LIGHT_GREEN}Import content.${NC}"
$DRUSH pm:enable drupalfr_migrate -y
$DRUSH migrate:import drupalfr_file --update
$DRUSH migrate:import drupalfr_user --update
$DRUSH migrate:import drupalfr_media --update
$DRUSH migrate:import drupalfr_paragraph_banner --update
$DRUSH migrate:import drupalfr_paragraph_block --update
$DRUSH migrate:import drupalfr_paragraph_html --update
$DRUSH migrate:import drupalfr_paragraph_title_text_media --update
$DRUSH migrate:import drupalfr_paragraph_trombinoscope_item --update
$DRUSH migrate:import drupalfr_paragraph_trombinoscope --update
$DRUSH migrate:import drupalfr_paragraph_layout_twocol --update
$DRUSH migrate:import drupalfr_website_type --update
$DRUSH migrate:import drupalfr_website_sector --update
$DRUSH migrate:import drupalfr_website_drupal_version --update
$DRUSH migrate:import drupalfr_drupal_version --update
$DRUSH migrate:import drupalfr_page --update
$DRUSH migrate:import drupalfr_job_offer --update
$DRUSH migrate:import drupalfr_showcase --update
$DRUSH migrate:import drupalfr_local_group --update
$DRUSH migrate:import drupalfr_event --update
$DRUSH migrate:import drupalfr_feed --update
$DRUSH migrate:import drupalfr_news_type --update
$DRUSH migrate:import drupalfr_news --update
$DRUSH migrate:import drupalfr_store --update
$DRUSH migrate:import drupalfr_product_variation_membership --update
$DRUSH migrate:import drupalfr_product_membership --update
$DRUSH migrate:import drupalfr_forum_tag --update
$DRUSH migrate:import drupalfr_forum_hierarchy --update
$DRUSH migrate:import drupalfr_localize_glossary_language --update
$DRUSH migrate:import drupalfr_localize_glossary_context --update
$DRUSH migrate:import drupalfr_localize_glossary_translation --update
$DRUSH migrate:import drupalfr_basic_block --update
$DRUSH migrate:import drupalfr_menu_link --update
# Re-import users to update references as we do not create stub.
$DRUSH migrate:import drupalfr_user --update
# Re-import paragraphs to update links to pages.
$DRUSH migrate:import drupalfr_paragraph_title_text_media --update

echo -e "${LIGHT_GREEN}Clear search API indexes.${NC}"
$DRUSH search-api:clear
