#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh
. $(dirname ${BASH_SOURCE[0]})/script-parameters.local.sh

# Without drush alias, change temporarily directory to www.
cd $WWW_PATH

# Database backup.
$DRUSH sql:dump --result-file="${PROJECT_PATH}/backups/${CURRENT_DATE}.sql" --gzip --structure-tables-key="common"

# Put the site in maintenance mode.
$DRUSH state:set system.maintenance_mode 1

# Install sources.
. $SCRIPTS_PATH/tasks/composer_install.sh

# Clear cache to be sure cache are cleared even if there is no update or Drush
# has been updated.
$DRUSH cache:rebuild

# Launch updates. Ensure that the database schema is up-to-date.
$DRUSH updatedb --entity-updates -y

# Enable development modules.
#. $SCRIPTS_PATH/tasks/development_modules.sh

# Export prod config split in case of overrides.
$DRUSH config-split:export prod -y

# Import configuration.
$DRUSH config:import -y

# Import content.
# For update.sh import only content if the environment is dev to not risk
# breaking prod.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  . $SCRIPTS_PATH/tasks/migrate_imports.sh
fi

# Translation updates.
. $SCRIPTS_PATH/tasks/update_translations.sh

# Remove the maintenance mode.
$DRUSH state:set system.maintenance_mode 0

# Run CRON.
$DRUSH core:cron

# Back to the current directory.
cd $CURRENT_PATH
