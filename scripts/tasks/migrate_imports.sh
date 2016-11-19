#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

. $PROJECT_PATH/scripts/script-parameters.sh
. $PROJECT_PATH/scripts/script-parameters.local.sh

# Import content.
$DRUSH en drupalfr_migrate -y
$DRUSH migrate-import drupalfr_file --update
$DRUSH migrate-import drupalfr_user --update
$DRUSH migrate-import drupalfr_website_type --update
$DRUSH migrate-import drupalfr_drupal_version --update
$DRUSH migrate-import drupalfr_page --update
$DRUSH migrate-import drupalfr_company --update
$DRUSH migrate-import drupalfr_job_offer --update
$DRUSH migrate-import drupalfr_showcase --update
$DRUSH migrate-import drupalfr_local_group --update
$DRUSH migrate-import drupalfr_event --update
$DRUSH migrate-import drupalfr_feed --update
# Re-import users to update references as we do not create stub.
$DRUSH migrate-import drupalfr_user --update

# Clear search_api indexes.
$DRUSH search-api-clear
