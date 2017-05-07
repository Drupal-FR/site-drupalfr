#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh
. $(dirname ${BASH_SOURCE[0]})/script-parameters.local.sh

# Without drush alias, change temporarily directory to www.
cd $WWW_PATH

# Database backup.
$DRUSH sql-dump --result-file="${PROJECT_PATH}/backups/${CURRENT_DATE}.sql" --gzip --structure-tables-key=common

# Put the site in maintenance mode.
$DRUSH state-set system.maintenance_mode 1

# Install sources.
. $SCRIPTS_PATH/tasks/composer_install.sh

# Launch updates. Ensure that the database schema is up-to-date.
$DRUSH updb --entity-updates -y

# Enable development modules.
. $SCRIPTS_PATH/tasks/development_modules.sh

# Revert features.
$DRUSH features-import -y $PROFILE

# Import content.
# For update.sh import only content if the environment is dev to not risk
# breaking prod.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  . $SCRIPTS_PATH/tasks/migrate_imports.sh
fi

# Translation updates.
. $SCRIPTS_PATH/tasks/update_translations.sh

# Remove the maintenance mode.
$DRUSH state-set system.maintenance_mode 0

# Run CRON.
$DRUSH cron

# Back to the current directory.
cd $CURRENT_PATH
