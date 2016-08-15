#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

. $PROJECT_PATH/scripts/script-parameters.sh
. $PROJECT_PATH/scripts/script-parameters.local.sh

# Test that composer is installed.
if ! hash "composer" 2> /dev/null; then
    echo "ERROR: Composer needs to be installed. Aborting.";
    exit 1;
fi

# Update source.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
    composer install --working-dir=$WWW_PATH
else
    composer install --working-dir=$WWW_PATH --no-dev
fi
composer dump-autoload --working-dir=$WWW_PATH --optimize

# Without drush alias, change temporarily directory to www.
cd $WWW_PATH

# Database backup.
$DRUSH sql-dump --result-file="${PROJECT_PATH}/backups/${CURRENT_DATE}.sql" --gzip

# Launch updates. Ensure that the database schema is up-to-date.
$DRUSH updb --entity-updates -y

# Enable development modules.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  $DRUSH en \
    config_inspector \
    dblog \
    devel \
    devel_a11y \
    features_ui \
    field_ui \
    views_ui \
    webprofiler \
    -y
fi

# Revert features.
$DRUSH features-import -y --bundle=drupalfr core
$DRUSH features-import -y --bundle=drupalfr site
$DRUSH features-import -y --bundle=drupalfr user
$DRUSH features-import -y --bundle=drupalfr homepage
$DRUSH features-import -y --bundle=drupalfr page
$DRUSH features-import -y --bundle=drupalfr company
$DRUSH features-import -y --bundle=drupalfr job_offer
$DRUSH features-import -y --bundle=drupalfr showcase
$DRUSH features-import -y --bundle=drupalfr local_group
$DRUSH features-import -y --bundle=drupalfr event
$DRUSH features-import -y --bundle=drupalfr media
# Waiting for https://www.drupal.org/node/2672490
#$DRUSH features-import -y --bundle=drupalfr drupalfr

# Translation updates.
$DRUSH locale-check
$DRUSH locale-update

# Import content.
# For update.sh import only content if the environment is dev to not risk
# breaking prod.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
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
  # Re-import users to update references as we do not create stub.
  $DRUSH migrate-import drupalfr_user --update
fi

# Back to the current directory.
cd $CURRENT_PATH
