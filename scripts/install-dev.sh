#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh local
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$@"

. "${SCRIPTS_PATH}"/tasks/composer_install.sh "${PROJECT_PATH}"

# shellcheck disable=SC2034
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  . "${SCRIPTS_PATH}"/tasks/dump_database.sh
  . "${SCRIPTS_PATH}"/tasks/install_drupal.sh
  . "${SCRIPTS_PATH}"/tasks/update_database.sh
  . "${SCRIPTS_PATH}"/tasks/import_configuration.sh
  . "${SCRIPTS_PATH}"/tasks/install_development_modules.sh
  . "${SCRIPTS_PATH}"/tasks/update_translations.sh
  . "${SCRIPTS_PATH}"/tasks/run_deploy_hooks.sh
  . "${SCRIPTS_PATH}"/tasks/import_default_content.sh
  . "${SCRIPTS_PATH}"/tasks/flush_cache.sh
  . "${SCRIPTS_PATH}"/tasks/run_cron.sh
done
