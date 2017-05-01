#!/bin/bash

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh
. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.local.sh

# Import content.
$DRUSH en drupalfr_migrate -y
$DRUSH migrate-import drupalfr_file --update
$DRUSH migrate-import drupalfr_user --update
$DRUSH migrate-import drupalfr_media --update
$DRUSH migrate-import drupalfr_paragraph --update
$DRUSH migrate-import drupalfr_website_type --update
$DRUSH migrate-import drupalfr_drupal_version --update
$DRUSH migrate-import drupalfr_page --update
$DRUSH migrate-import drupalfr_company --update
$DRUSH migrate-import drupalfr_job_offer --update
$DRUSH migrate-import drupalfr_showcase --update
$DRUSH migrate-import drupalfr_local_group --update
$DRUSH migrate-import drupalfr_event --update
$DRUSH migrate-import drupalfr_feed --update
$DRUSH migrate-import drupalfr_news_type --update
$DRUSH migrate-import drupalfr_news --update
$DRUSH migrate-import drupalfr_basic_block --update
$DRUSH migrate-import drupalfr_menu_link --update
# Re-import users to update references as we do not create stub.
$DRUSH migrate-import drupalfr_user --update

# Clear search_api indexes.
$DRUSH search-api-clear
