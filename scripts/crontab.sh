#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh local
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$@"

# shellcheck disable=SC2034
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"
  DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"
  DRUSH="${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/current/vendor/bin/drush"

  if [ ! -f "${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/disabled_cron" ]; then
    $DRUSH "${!DRUSH_ALIAS}" core:cron >> /var/log/drupal/"${ENVIRONMENT_NAME}_${!FOLDER_NAME}"_drupal_cron.log 2>&1
  fi
done
