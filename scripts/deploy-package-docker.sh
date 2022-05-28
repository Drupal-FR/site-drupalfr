#!/usr/bin/env bash
# shellcheck disable=SC2029

usage() {
  printf "deploy-package-docker.sh [integ|staging|preprod|prod] [9.x-2021-04-23-10h07m23s|1.0.0]\n"
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

suffixed_package_name="${PROJECT_NAME}"-"${package_name}"
complete_package_name="${suffixed_package_name}".tar.gz
package_local_path="$(dirname "${BASH_SOURCE[0]}")"/../build/"${complete_package_name}"

# Check that the package exists.
if [ ! -f "${package_local_path}" ]; then
  echo "The package file does not exist."
  usage
  exit 1
fi

IFS=',' read -ra FRONT_SERVERS_HOSTS <<< "$FRONT_SERVERS_HOSTS"
# shellcheck disable=SC2128
IFS=',' read -ra DRUPAL_SITES_LIST <<< "$DRUPAL_SITES_LIST"

# Prepare exclude option.
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"

  FILES_EXCLUDED_FROM_DOCKER_SYNC=("${FILES_EXCLUDED_FROM_DOCKER_SYNC[@]}" "app/sites/${!FOLDER_NAME}/files")
  FILES_EXCLUDED_FROM_DOCKER_SYNC=("${FILES_EXCLUDED_FROM_DOCKER_SYNC[@]}" "private_files/${!FOLDER_NAME}")
done

EXCLUDE_OPTION=''
for FILE_EXCLUDED_FROM_DOCKER_SYNC in "${FILES_EXCLUDED_FROM_DOCKER_SYNC[@]}"
do
  EXCLUDE_OPTION="${EXCLUDE_OPTION} --exclude=${FILE_EXCLUDED_FROM_DOCKER_SYNC}"
done

# Check that each server is reachable.
for FRONT_SERVERS_HOST in "${FRONT_SERVERS_HOSTS[@]}"
do
  if ! ping -c 1 "${FRONT_SERVERS_HOST}"; then
    echo -e "${COLOR_LIGHT_RED}Server ${FRONT_SERVERS_HOST}: Impossible to ping the server.${COLOR_NC}"
    exit 1
  fi
done

# Loop on each server to deploy the archive, etc.
for FRONT_SERVERS_HOST in "${FRONT_SERVERS_HOSTS[@]}"
do
  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Create folders architecture in case it does not exist yet.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "mkdir -p ${DEPLOYMENT_PATH}/releases"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "mkdir -p ${DEPLOYMENT_PATH}/sources"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Clear releases folder.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "rm -rf ${DEPLOYMENT_PATH}/releases/*"

  # Check if the release already exists on the server.
  if ! $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "test -e ${DEPLOYMENT_PATH}/releases/${package_name}"; then
    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Deploy archive.${COLOR_NC}"
    rsync -azP -e "${SSH}" "${package_local_path}" "${SSH_USER}"@"${FRONT_SERVERS_HOST}":/tmp/

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Extract archive.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "tar -xzf /tmp/${complete_package_name} -C /tmp"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Move extracted source to deployment folder.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "mv /tmp/${suffixed_package_name} /tmp/${package_name}"
    $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "mv /tmp/${package_name} ${DEPLOYMENT_PATH}/releases"

    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Special handling of the docker-compose.yml file as it is excluded from packaging.${COLOR_NC}"
    # shellcheck disable=2140
    rsync -azP -e "${SSH}" "./docker-compose.yml" "${SSH_USER}"@"${FRONT_SERVERS_HOST}":"${DEPLOYMENT_PATH}/releases/${package_name}/"
    # shellcheck disable=2140
    rsync -azP -e "${SSH}" "./docker" "${SSH_USER}"@"${FRONT_SERVERS_HOST}":"${DEPLOYMENT_PATH}/releases/${package_name}/"
    # shellcheck disable=2140
    rsync -azP -e "${SSH}" "./conf/env/example.composer.env" "${SSH_USER}"@"${FRONT_SERVERS_HOST}":"${DEPLOYMENT_PATH}/releases/${package_name}/conf/env/composer.env"
  else
    echo -e "${COLOR_BROWN_ORANGE}Server ${FRONT_SERVERS_HOST}: Release folder already exists.${COLOR_NC}"
  fi

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Sync sources.${COLOR_NC}"
  # shellcheck disable=2027
  # shellcheck disable=2086
  # Avoid double quotes around $EXCLUDE_OPTION because we specifically wants
  # word splitting.
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "rsync -azPq --no-g --delete ${EXCLUDE_OPTION} ${DEPLOYMENT_PATH}/releases/${package_name}/ ${DEPLOYMENT_PATH}/sources/"

  echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_SERVERS_HOST}: Update containers and sites.${COLOR_NC}"
  $SSH "${SSH_USER}"@"${FRONT_SERVERS_HOST}" "cd ${DEPLOYMENT_PATH}/sources && \
    make docker-down && \
    make docker-upd && \
    make docker-drupal-paranoia && \
    make docker-site-update SELECTED_SITE=all"
done
