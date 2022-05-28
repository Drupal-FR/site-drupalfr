#!/usr/bin/env bash

usage() {
  printf "init.sh -e [dev|gitlab|integ|staging|preprod|prod]\n"
  printf "Options:\n\t%s\n"\
   "-e The environment"
}

environment=''

while getopts ":he:" opt
do
  case "${opt}" in
	h)
    usage
    exit 0
    ;;
	e)
    environment="${OPTARG}"
    ;;
	:)
    echo "Option -$OPTARG requires an argument."
    usage
    exit 1
    ;;
	\?)
    usage
    exit 1
    ;;
  esac
done

# Check that all required parameters are there.
if [ -z "${environment}" ]; then
  echo "Missing environment parameter."
  usage
  exit 1
fi

# Load variables from example.prod.env file to have the color variables (.env is
# not initialized yet).
set -o allexport
# shellcheck disable=SC1091
# shellcheck source=example.prod.env
source "$(dirname "${BASH_SOURCE[0]}")"/../example."${environment}".env
set +o allexport

PROJECT_PATH="$(dirname "$(dirname "${BASH_SOURCE[0]}")")"

echo -e "${COLOR_LIGHT_GREEN}Copy example files.${COLOR_NC}"

rsync -avz --ignore-existing "${PROJECT_PATH}"/example."${environment}".env "${PROJECT_PATH}"/.env
echo -e "${COLOR_LIGHT_RED}Please check values in ${PROJECT_PATH}/.env${COLOR_NC}"

rsync -avz --ignore-existing "${PROJECT_PATH}"/conf/env/example.composer.env "${PROJECT_PATH}"/conf/env/composer.env
echo -e "${COLOR_LIGHT_RED}Please check values in ${PROJECT_PATH}/conf/env/composer.env${COLOR_NC}"

rsync -avz --ignore-existing "${PROJECT_PATH}"/conf/drupal/example.sites.php "${PROJECT_PATH}"/conf/drupal/sites.php
echo -e "${COLOR_LIGHT_RED}Please check values in ${PROJECT_PATH}/conf/drupal/sites.php${COLOR_NC}"

IFS=',' read -ra DRUPAL_SITES_LIST <<< "$DRUPAL_SITES_LIST"
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"

  rsync -avz --ignore-existing "${PROJECT_PATH}"/conf/drupal/"${!FOLDER_NAME}"/example.settings.local.php "${PROJECT_PATH}"/conf/drupal/"${!FOLDER_NAME}"/settings.local.php
  echo -e "${COLOR_LIGHT_RED}Please check values in ${PROJECT_PATH}/conf/drupal/${!FOLDER_NAME}/settings.local.php${COLOR_NC}"

  echo -e "${COLOR_LIGHT_GREEN}Create public files directory for ${!FOLDER_NAME}.${COLOR_NC}"
  mkdir -p "${PROJECT_PATH}"/app/sites/"${!FOLDER_NAME}"/files

  echo -e "${COLOR_LIGHT_GREEN}Permissions are for dev environments. It should be less permissive.${COLOR_NC}"
  chmod 777 "${PROJECT_PATH}"/app/sites/"${!FOLDER_NAME}"/files
done
