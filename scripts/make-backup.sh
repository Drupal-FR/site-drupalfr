#!/usr/bin/env bash
# shellcheck disable=SC2029

usage() {
  printf "make-backup.sh [integ|staging|preprod|prod] [all|default]\n"
}

environment=''

if [ -n "$1" ]; then
  environment=$1
fi

# Check that all required parameters are present.
if [ -z "${environment}" ]; then
  echo "Missing target environment parameter."
  usage
  exit 1
elif [ "${environment}" = "local" ]; then
  echo "This script is not meant to be launched for local environment."
  usage
  exit 1
fi

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh "${environment}"

# shellcheck source=scripts/selection-site.sh
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$2"

# Check that the main front server is reachable.
if ! ping -c 1 "${FRONT_MAIN_SERVER_HOST}"; then
  echo -e "${COLOR_LIGHT_RED}Server ${FRONT_MAIN_SERVER_HOST}: Impossible to ping the server.${COLOR_NC}"
  exit 1
fi

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
  FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"
  BACKUP_FILES="DRUPAL_SITE_${DRUPAL_SITE^^}_BACKUP_FILES"
  DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"
  DRUSH="${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/current/vendor/bin/drush"

  # Test if the "current" symlink exists for this website.
  if $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "test -e ${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/current"; then
    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Create backup folder.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} mkdir -p ${DEPLOYMENT_PATH}/backups/${!FOLDER_NAME}/${CURRENT_DATE}"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Database backup.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} $DRUSH ${!DRUSH_ALIAS} sql:dump \
      --result-file=${DEPLOYMENT_PATH}/backups/${!FOLDER_NAME}/${CURRENT_DATE}/${environment}-${!FOLDER_NAME}.sql \
      --gzip"

    if [ "${!BACKUP_FILES}" = "true" ]; then
      echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Public files backup.${NC}"
      # shellcheck disable=2086
      # Avoid double quotes around $PUBLIC_FILES_EXCLUDE_OPTION because we specifically wants
      # word splitting.
      $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} rsync -azPq ${PUBLIC_FILES_EXCLUDE_OPTION} ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files ${DEPLOYMENT_PATH}/backups/${!FOLDER_NAME}/${CURRENT_DATE}/files-${environment}-${!FOLDER_NAME}"

      echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Private files backup.${NC}"
      # shellcheck disable=2086
      # Avoid double quotes around $PRIVATE_FILES_EXCLUDE_OPTION because we specifically wants
      # word splitting.
      $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} rsync -azPq ${PRIVATE_FILES_EXCLUDE_OPTION} ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME} ${DEPLOYMENT_PATH}/backups/${!FOLDER_NAME}/${CURRENT_DATE}/private_files-${environment}-${!FOLDER_NAME}"
    fi
  else
    echo -e "${COLOR_BROWN_ORANGE}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Is not enabled yet (no current symlink).${COLOR_NC}"
  fi
done
