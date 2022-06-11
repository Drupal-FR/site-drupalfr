#!/usr/bin/env bash

usage() {
  printf "package-composer-install.sh [integ|staging|preprod|prod] [9.x|1.0.0]\n"
}

environment=''
version=''

if [ -n "$1" ]; then
  environment=$1
fi

if [ -n "$2" ]; then
  version=$2
fi

# Check that all required parameters are present.
if [ -z "${environment}" ]; then
  echo "Missing target environment parameter."
  usage
  exit 1
elif [ -z "${version}" ]; then
  echo "Missing version parameter."
  usage
  exit 1
fi

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh "${environment}"

# Ensure packaging environment is not impacted by the value of the target
# environment configuration.
# shellcheck disable=SC2034
CUSTOM_PHP_ALLOW_URL_FOPEN=1

BUILD_PATH="$(dirname "$(dirname "${BASH_SOURCE[0]}")")"/build
PACKAGE_PATH="${BUILD_PATH}"/"${version}"

echo -e "${COLOR_LIGHT_GREEN}Composer install.${COLOR_NC}"
if [ "${COMPOSER_ADD_REQUIRE_DEV}" = "yes" ]; then
  # Avoid side effect with Grumphp re-writing root git hook.
  composer config --working-dir="${PACKAGE_PATH}" \
    allow-plugins.phpro/grumphp false

  composer install --working-dir="${PACKAGE_PATH}" \
    --ignore-platform-reqs \
    --no-interaction \
    --no-progress
else
  composer install --working-dir="${PACKAGE_PATH}" \
    --ignore-platform-reqs \
    --no-dev \
    --no-interaction \
    --no-progress
fi

echo -e "${COLOR_LIGHT_GREEN}Rebuild Drupal paranoia.${COLOR_NC}"
composer drupal:paranoia --working-dir="${PACKAGE_PATH}"
