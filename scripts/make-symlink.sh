#!/usr/bin/env bash
# shellcheck disable=SC2029

usage() {
  printf "make-symlink.sh [integ|staging|preprod|prod] [9.x-2021-04-23-10h07m23s|1.0.0] [all|default]\n"
}

environment=''
package_name=''

if [ -n "$1" ]; then
  environment=$1
fi

if [ -n "$2" ]; then
  package_name=$2
fi

# Check that all required parameters are present.
if [ -z "${environment}" ]; then
  echo "Missing target environment parameter."
  usage
  exit 1
elif [ -z "${package_name}" ]; then
  echo "Missing package name parameter."
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
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$3"

IFS=',' read -ra FRONT_SERVERS_HOSTS <<< "$FRONT_SERVERS_HOSTS"

# Check that each server is reachable.
for FRONT_SERVERS_HOST in "${FRONT_SERVERS_HOSTS[@]}"
do
  if ! ping -c 1 "${FRONT_SERVERS_HOST}"; then
    echo -e "${COLOR_LIGHT_RED}Server ${FRONT_SERVERS_HOST}: Impossible to ping the server.${COLOR_NC}"
    exit 1
  fi
done

# Check that the release already exists on each server.
missing_release=''
for FRONT_SERVERS_HOST in "${FRONT_SERVERS_HOSTS[@]}"
do
  if ! $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "test -e ${DEPLOYMENT_PATH}/releases/${package_name}"; then
    echo -e "${COLOR_LIGHT_RED}Server ${FRONT_SERVERS_HOST}: The release ${package_name} is not present.${COLOR_NC}"
    missing_release='yes'
  fi
done

if [ "${missing_release}" = "yes" ]; then
  exit 1
fi

# Loop on each server to make new current symlinks.
for FRONT_SERVERS_HOST in "${FRONT_SERVERS_HOSTS[@]}"
do
  for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
  do
    FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: ${DRUPAL_SITE}: Create site folder in case it does not exist yet.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo -u ${PROJECT_USER} mkdir -p ${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: ${DRUPAL_SITE}: Remove current symlink.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo -u ${PROJECT_USER} rm -f ${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/current"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: ${DRUPAL_SITE}: Create new current symlink.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo -u ${PROJECT_USER} ln -s ${DEPLOYMENT_PATH}/releases/${package_name} ${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/current"

    # Copy .htaccess files during symlink creation to ensure the ones in shared
    # match the code base of the active release.
    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: ${DRUPAL_SITE}: Copy public .htaccess file.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo -u ${PROJECT_USER} cp -f ${DEPLOYMENT_PATH}/releases/${package_name}/scripts/scaffold/public_files.htaccess ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files/.htaccess"
    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: ${DRUPAL_SITE}: Copy private .htaccess file.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo -u ${PROJECT_USER} cp -f ${DEPLOYMENT_PATH}/releases/${package_name}/scripts/scaffold/private_files.htaccess ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}/.htaccess"
  done
done
