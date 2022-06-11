#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh local
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$@"

# Prepare exclude option.
PUBLIC_FILES_EXCLUDE_OPTION=''
for PUBLIC_FILES_EXCLUDED_FROM_BACKUP in "${PUBLIC_FILES_EXCLUDED_FROM_BACKUP[@]}"
do
  PUBLIC_FILES_EXCLUDE_OPTION="${PUBLIC_FILES_EXCLUDE_OPTION} --exclude=${PUBLIC_FILES_EXCLUDED_FROM_BACKUP}"
done

PRIVATE_FILES_EXCLUDE_OPTION=''
for PRIVATE_FILES_EXCLUDED_FROM_BACKUP in "${PRIVATE_FILES_EXCLUDED_FROM_BACKUP[@]}"
do
  PRIVATE_FILES_EXCLUDE_OPTION="${PRIVATE_FILES_EXCLUDE_OPTION} --exclude=${PRIVATE_FILES_EXCLUDED_FROM_BACKUP}"
done

for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  CURRENT_SITE_FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"
  CURRENT_SITE_BACKUP_FILES="DRUPAL_SITE_${DRUPAL_SITE^^}_BACKUP_FILES"

  . "${SCRIPTS_PATH}"/tasks/dump_database.sh

  if [ "${!CURRENT_SITE_BACKUP_FILES}" = "true" ]; then
    echo -e "${COLOR_LIGHT_GREEN}Public files backup.${NC}"
    # shellcheck disable=2086
    # Avoid double quotes around $PUBLIC_FILES_EXCLUDE_OPTION because we specifically wants
    # word splitting.
    rsync -azPq ${PUBLIC_FILES_EXCLUDE_OPTION} "${APP_PATH}/sites/${!CURRENT_SITE_FOLDER_NAME}/files" "${PROJECT_PATH}/backups/${!CURRENT_SITE_FOLDER_NAME}/${CURRENT_DATE}/files-${!CURRENT_SITE_FOLDER_NAME}"

    echo -e "${COLOR_LIGHT_GREEN}Private files backup.${NC}"
    # shellcheck disable=2086
    # Avoid double quotes around $PRIVATE_FILES_EXCLUDE_OPTION because we specifically wants
    # word splitting.
    rsync -azPq ${PRIVATE_FILES_EXCLUDE_OPTION} "${PROJECT_PATH}/private_files/${!CURRENT_SITE_FOLDER_NAME}" "${PROJECT_PATH}/backups/${CURRENT_DATE}/${!CURRENT_SITE_FOLDER_NAME}/private_files-${!CURRENT_SITE_FOLDER_NAME}"
  fi
done
