#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh
. $(dirname ${BASH_SOURCE[0]})/script-parameters.local.sh

# Install sources.
. $SCRIPTS_PATH/tasks/composer_install.sh

# Without drush alias, change temporarily directory to www.
cd $WWW_PATH

# Clear Drush cache in case of update.
$DRUSH cache:clear drush

# Database backup.
$DRUSH sql:dump --result-file="${PROJECT_PATH}/backups/${CURRENT_DATE}.sql" --gzip --structure-tables-key="common"

# Disable external cache.
rm -f $WWW_PATH/sites/default/.cache_activated

# Install Drupal.
$DRUSH site:install config_installer \
  --account-mail=$ACCOUNT_MAIL \
  --account-name=$ACCOUNT_NAME \
  --account-pass=$ACCOUNT_PASS \
  --site-mail=$SITE_MAIL \
  --site-name=$SITE_NAME \
  --locale=$DEFAULT_LANGUAGE \
  -y

# Launch updates. Ensure that the database schema is up-to-date.
$DRUSH updatedb --entity-updates -y

. $SCRIPTS_PATH/tasks/migrate_imports.sh
. $SCRIPTS_PATH/tasks/update_translations.sh

# Run CRON.
$DRUSH core:cron

# Enable external cache.
touch $WWW_PATH/sites/default/.cache_activated
$DRUSH cache:rebuild

# Back to the current directory.
cd $CURRENT_PATH
