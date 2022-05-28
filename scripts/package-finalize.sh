#!/usr/bin/env bash

# Build folder had been created.
# Git archive has been done.
# Composer install and paranoia had been executed.
# Assets compilation had been done.

usage() {
  printf "package-finalize.sh [integ|staging|preprod|prod] [branch|tag] [9.x|1.0.0]\n"
}

environment=''
version_type=''
version=''

if [ -n "$1" ]; then
  environment=$1
fi

if [ -n "$2" ]; then
  version_type=$2
fi

if [ -n "$3" ]; then
  version=$3
fi

# Check that all required parameters are present.
if [ -z "${environment}" ]; then
  echo "Missing target environment parameter."
  usage
  exit 1
elif [ -z "${version_type}" ]; then
  echo "Missing version type parameter."
  usage
  exit 1
elif [[ "${version_type}" != "tag" && "${version_type}" != "branch" ]]; then
  echo "Incorrect version type parameter. allowed values are tag or branch."
  usage
  exit 1
elif [ -z "${version}" ]; then
  echo "Missing version parameter."
  usage
  exit 1
fi

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh "${environment}"

BUILD_PATH="$(dirname "$(dirname "${BASH_SOURCE[0]}")")"/build
PACKAGE_PATH="${BUILD_PATH}"/"${version}"

FINAL_VERSION_NAME="${PROJECT_NAME}"-"${version}"
SCRIPTS_VERSION_NAME="${version}"
if [ "${version_type}" == "branch" ]; then
  SCRIPTS_VERSION_NAME="${version}"-${CURRENT_DATE}
  FINAL_VERSION_NAME="${PROJECT_NAME}"-"${version}"-${CURRENT_DATE}
fi
ARCHIVE_NAME="${FINAL_VERSION_NAME}".tar.gz

rsync -avz --ignore-existing "${PACKAGE_PATH}"/example."${environment}".env "${PACKAGE_PATH}"/.env
echo -e "${COLOR_LIGHT_GREEN}File copied at: ${PACKAGE_PATH}/.env${COLOR_NC}"

rsync -avz --ignore-existing "${PACKAGE_PATH}"/conf/drupal/example.sites.php "${PACKAGE_PATH}"/conf/drupal/sites.php
echo -e "${COLOR_LIGHT_GREEN}File copied at: ${PACKAGE_PATH}/conf/drupal/sites.php${COLOR_NC}"

# Replace secrets in the .env with plain values.
for ENV_VARIABLE_TO_REPLACE in "${ENV_VARIABLES_WITH_SECRETS[@]}"
do
  sed -i -E "s#${ENV_VARIABLE_TO_REPLACE}=(.*)#${ENV_VARIABLE_TO_REPLACE}=${!ENV_VARIABLE_TO_REPLACE}#" "${PACKAGE_PATH}"/.env
done

# shellcheck disable=SC2128
IFS=',' read -ra DRUPAL_SITES_LIST <<< "$DRUPAL_SITES_LIST"
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  FOLDER_NAME="DRUPAL_SITE_${DRUPAL_SITE^^}_FOLDER_NAME"

  rsync -avz --ignore-existing "${PACKAGE_PATH}"/conf/drupal/"${!FOLDER_NAME}"/example.settings.local.php "${PACKAGE_PATH}"/conf/drupal/"${!FOLDER_NAME}"/settings.local.php
  echo -e "${COLOR_LIGHT_GREEN}File copied at: ${PACKAGE_PATH}/conf/drupal/${!FOLDER_NAME}/settings.local.php${COLOR_NC}"

  FILES_EXCLUDED_FROM_PACKAGE=("${FILES_EXCLUDED_FROM_PACKAGE[@]}" "app/sites/${!FOLDER_NAME}/files")
  FILES_EXCLUDED_FROM_PACKAGE=("${FILES_EXCLUDED_FROM_PACKAGE[@]}" "conf/drupal/${!FOLDER_NAME}/example.settings.local.php")
  if [ "${DRUPAL_DEVELOPMENT_MODE_ENABLED}" != "yes" ]; then
    FILES_EXCLUDED_FROM_PACKAGE=("${FILES_EXCLUDED_FROM_PACKAGE[@]}" "conf/drupal/${!FOLDER_NAME}/development.services.yml")
  fi
  FILES_EXCLUDED_FROM_PACKAGE=("${FILES_EXCLUDED_FROM_PACKAGE[@]}" "private_files/${!FOLDER_NAME}")
done

for FILE_EXCLUDED_FROM_PACKAGE in "${FILES_EXCLUDED_FROM_PACKAGE[@]}"
do
  echo -e "${COLOR_LIGHT_GREEN}Delete file: ${PACKAGE_PATH}/${FILE_EXCLUDED_FROM_PACKAGE}${COLOR_NC}"
  rm -rf "${PACKAGE_PATH:?}"/"${FILE_EXCLUDED_FROM_PACKAGE}"
done

# Rename package folder for easier archive extraction.
mv "${BUILD_PATH}"/"${version}" "${BUILD_PATH}"/"${FINAL_VERSION_NAME}"
tar -cz -C "${BUILD_PATH}" -f "${BUILD_PATH}"/"${ARCHIVE_NAME}" "${FINAL_VERSION_NAME}"

rm -rf "${BUILD_PATH:?}"/"${FINAL_VERSION_NAME}"

echo -e "${COLOR_LIGHT_GREEN}Export version name for usage in CI${COLOR_NC}"
rm -f "${BUILD_PATH}"/version_name
touch "${BUILD_PATH}"/version_name
echo "${SCRIPTS_VERSION_NAME}" >> "${BUILD_PATH}"/version_name
echo -e "${COLOR_LIGHT_PURPLE}make deploy-package TARGET_ENVIRONMENT=${environment} PACKAGE_NAME=${SCRIPTS_VERSION_NAME}${COLOR_NC}"
