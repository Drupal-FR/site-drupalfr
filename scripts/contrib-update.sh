#!/usr/bin/env bash

# Use this script to make core and contrib updates.
# Launch the script after making a git pull and editing your composer.json.

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh local
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$@"

# shellcheck disable=SC2034
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  # Ensure environment is up-to-date.
  . "${SCRIPTS_PATH}"/tasks/update_database.sh
  . "${SCRIPTS_PATH}"/tasks/export_config_split_overrides.sh
  . "${SCRIPTS_PATH}"/tasks/import_configuration.sh

  . "${SCRIPTS_PATH}"/tasks/dump_database.sh
done

. "${SCRIPTS_PATH}"/tasks/composer_update.sh

# shellcheck disable=SC2034
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  . "${SCRIPTS_PATH}"/tasks/update_database.sh
  . "${SCRIPTS_PATH}"/tasks/update_translations.sh
  . "${SCRIPTS_PATH}"/tasks/flush_cache.sh
  . "${SCRIPTS_PATH}"/tasks/export_config_split_overrides.sh
  . "${SCRIPTS_PATH}"/tasks/export_configuration.sh

  . "${SCRIPTS_PATH}"/tasks/flush_cache.sh
done
