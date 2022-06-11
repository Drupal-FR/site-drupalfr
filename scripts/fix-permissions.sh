#!/usr/bin/env bash
# shellcheck disable=SC2029

usage() {
  printf "fix-permissions.sh [integ|staging|preprod|prod] [9.x-2021-04-23-10h07m23s|1.0.0]\n"
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

IFS=',' read -ra FRONT_SERVERS_HOSTS <<< "$FRONT_SERVERS_HOSTS"
# shellcheck disable=SC2128
IFS=',' read -ra DRUPAL_SITES_LIST <<< "$DRUPAL_SITES_LIST"

# Check that each server is reachable.
for FRONT_SERVERS_HOST in "${FRONT_SERVERS_HOSTS[@]}"
do
  if ! ping -c 1 "${FRONT_SERVERS_HOST}"; then
    echo -e "${COLOR_LIGHT_RED}Server ${FRONT_SERVERS_HOST}: Impossible to ping the server.${COLOR_NC}"
    exit 1
  fi
done

# Loop on each server.
for FRONT_SERVERS_HOST in "${FRONT_SERVERS_HOSTS[@]}"
do
  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Set permissions to project sources.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod 0775 -R ${DEPLOYMENT_PATH}/releases/${package_name}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${PROJECT_GROUP} -R ${DEPLOYMENT_PATH}/releases/${package_name}"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Set execution mode to scripts.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod u+x,g+x -R ${DEPLOYMENT_PATH}/releases/${package_name}/scripts"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod u+x,g+x -R ${DEPLOYMENT_PATH}/releases/${package_name}/vendor/bin"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Set permissions for contrib translations folder.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod g+w -R ${DEPLOYMENT_PATH}/releases/${package_name}/app/translations/contrib"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${WEBSERVER_GROUP} -R ${DEPLOYMENT_PATH}/releases/${package_name}/app/translations/contrib"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Set permissions for sites.php.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod 0644 ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/sites.php"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod 0444 ${DEPLOYMENT_PATH}/releases/${package_name}/app/sites/sites.php"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${PROJECT_GROUP} ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/sites.php"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${PROJECT_GROUP} ${DEPLOYMENT_PATH}/releases/${package_name}/app/sites/sites.php"

  for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
  do
    FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: ${DRUPAL_SITE}: Set permissions for configuration folders.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod 0775 -R ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/${!FOLDER_NAME}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${WEBSERVER_GROUP} -R ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/${!FOLDER_NAME}"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: ${DRUPAL_SITE}: Set permissions for settings files.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod 0644 ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/${!FOLDER_NAME}/services.yml"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod 0644 ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/${!FOLDER_NAME}/settings.local.php"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chmod 0444 ${DEPLOYMENT_PATH}/releases/${package_name}/app/sites/${!FOLDER_NAME}/settings.php"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${PROJECT_GROUP} ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/${!FOLDER_NAME}/services.yml"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${PROJECT_GROUP} ${DEPLOYMENT_PATH}/releases/${package_name}/conf/drupal/${!FOLDER_NAME}/settings.local.php"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "sudo chown ${PROJECT_USER}:${PROJECT_GROUP} ${DEPLOYMENT_PATH}/releases/${package_name}/app/sites/${!FOLDER_NAME}/settings.php"
  done
done

# For operations in the shared folder, only do it on main server.
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Set permissions for public files folder.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chmod g+w -R ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files"
  $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chown ${PROJECT_USER}:${WEBSERVER_GROUP} -R ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Set permissions for private files folder.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chmod g+w -R ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}"
  $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chown ${PROJECT_USER}:${WEBSERVER_GROUP} -R ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}"
done
