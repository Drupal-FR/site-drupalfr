#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh local
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$@"

# Before new symlink.
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"

  . "${SCRIPTS_PATH}"/tasks/dump_database.sh

  echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Enable maintenance mode.${COLOR_NC}"
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" state:set system.maintenance_mode 1
done

if [ "${COMPOSER_INSTALL_ON_UPDATE}" == "yes" ]; then
  . "${SCRIPTS_PATH}"/tasks/composer_install.sh "${PROJECT_PATH}"
fi

# After new symlink.
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"

  . "${SCRIPTS_PATH}"/tasks/update_database.sh

  echo -e "${COLOR_LIGHT_GREEN}Clear cache to be sure cache are cleared even if there is no update.${COLOR_NC}"
  # Otherwise for example 'drush config:export' does not detect that there are
  # changes to export.
  . "${SCRIPTS_PATH}"/tasks/flush_cache.sh

  . "${SCRIPTS_PATH}"/tasks/export_config_split_overrides.sh
  . "${SCRIPTS_PATH}"/tasks/import_configuration.sh
  . "${SCRIPTS_PATH}"/tasks/update_translations.sh
  . "${SCRIPTS_PATH}"/tasks/run_deploy_hooks.sh

  # Re-export overrides split to ensure exported overrides are up-to-date.
  . "${SCRIPTS_PATH}"/tasks/export_config_split_overrides.sh

  if [ "${DRUPAL_IMPORT_DEFAULT_CONTENT_ON_UPDATE}" = "yes" ]; then
    . "${SCRIPTS_PATH}"/tasks/import_default_content.sh
  fi

  echo -e "${COLOR_LIGHT_GREEN}Disable maintenance mode.${COLOR_NC}"
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" state:set system.maintenance_mode 0

  . "${SCRIPTS_PATH}"/tasks/flush_cache.sh
  . "${SCRIPTS_PATH}"/tasks/run_cron.sh
done
