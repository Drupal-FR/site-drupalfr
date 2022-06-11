#!/usr/bin/env bash
# shellcheck disable=SC2029

usage() {
  printf "clear-backup.sh [prod] [all|default]\n"
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

for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Select a backup to delete.${COLOR_NC}"
  select folder_to_delete in $($SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "ls -d ${DEPLOYMENT_PATH}/backups/${!FOLDER_NAME}/*")
  do
    if [ -n "$folder_to_delete" ]; then
      $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} rm -rf $folder_to_delete"
      echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Folder deleted.${COLOR_NC}"
    fi
    break
  done
done
