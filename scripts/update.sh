#!/bin/bash

set -e

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh

echo -e "${LIGHT_GREEN}Without drush alias, change temporarily directory to www.${NC}"
cd $WWW_PATH

echo -e "${LIGHT_GREEN}Database backup.${NC}"
$DRUSH sql:dump --result-file="${PROJECT_PATH}/backups/${CURRENT_DATE}.sql" --gzip --structure-tables-key="common"

echo -e "${LIGHT_GREEN}Put the site in maintenance mode.${NC}"
$DRUSH state:set system.maintenance_mode 1

. $SCRIPTS_PATH/tasks/composer_install.sh

echo -e "${LIGHT_GREEN}Clear cache to be sure cache are cleared even if there is no update or Drush has been updated.${NC}"
$DRUSH cache:rebuild

echo -e "${LIGHT_GREEN}Launch updates. Ensure that the database schema is up-to-date.${NC}"
$DRUSH updatedb --entity-updates -y

echo -e "${LIGHT_GREEN}Export prod config split in case of overrides.${NC}"
$DRUSH config-split:export prod -y

echo -e "${LIGHT_GREEN}Import configuration.${NC}"
$DRUSH config:import -y

# For update.sh import only content if the environment is dev to not risk
# breaking prod.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  . $SCRIPTS_PATH/tasks/migrate_imports.sh
fi

. $SCRIPTS_PATH/tasks/update_translations.sh

echo -e "${LIGHT_GREEN}Remove the maintenance mode.${NC}"
$DRUSH state:set system.maintenance_mode 0

echo -e "${LIGHT_GREEN}Run CRON.${NC}"
$DRUSH core:cron

echo -e "${LIGHT_GREEN}Back to the current directory.${NC}"
cd $CURRENT_PATH
