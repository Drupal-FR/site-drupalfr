#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh
. $(dirname ${BASH_SOURCE[0]})/script-parameters.local.sh

. $SCRIPTS_PATH/tasks/composer_install.sh

echo -e "${LIGHT_GREEN}Without drush alias, change temporarily directory to www.${NC}"
cd $WWW_PATH

echo -e "${LIGHT_GREEN}Clear Drush cache in case of update.${NC}"
$DRUSH cache:clear drush

echo -e "${LIGHT_GREEN}Database backup.${NC}"
$DRUSH sql:dump --result-file="${PROJECT_PATH}/backups/${CURRENT_DATE}.sql" --gzip --structure-tables-key="common"

echo -e "${LIGHT_GREEN}Clear Redis cache because otherwise it is no emptied on site install and it provokes errors.${NC}"
$REDIS_FLUSH_COMMAND

echo -e "${LIGHT_GREEN}Install Drupal.${NC}"
$DRUSH site:install config_installer \
  --account-mail=$ACCOUNT_MAIL \
  --account-name=$ACCOUNT_NAME \
  --account-pass=$ACCOUNT_PASS \
  --site-mail=$SITE_MAIL \
  --site-name=$SITE_NAME \
  --locale=$DEFAULT_LANGUAGE \
  -y

echo -e "${LIGHT_GREEN}Set the site mail on install because it is ignored by config ignore.${NC}"
$DRUSH config:set system.site mail $SITE_MAIL -y

echo -e "${LIGHT_GREEN}Import configuration so splits and some side effects are taken into account.${NC}"
$DRUSH config:import -y

echo -e "${LIGHT_GREEN}Launch updates. Ensure that the database schema is up-to-date.${NC}"
$DRUSH updatedb --entity-updates -y

. $SCRIPTS_PATH/tasks/migrate_imports.sh
. $SCRIPTS_PATH/tasks/update_translations.sh

echo -e "${LIGHT_GREEN}Run CRON.${NC}"
$DRUSH core:cron

echo -e "${LIGHT_GREEN}Flush caches to be clean.${NC}"
$DRUSH cache:rebuild

echo -e "${LIGHT_GREEN}Back to the current directory.${NC}"
cd $CURRENT_PATH
