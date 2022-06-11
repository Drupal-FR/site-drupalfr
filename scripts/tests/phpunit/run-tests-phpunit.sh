#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/script-parameters.sh local

# Detect if executed in Gitlab CI or not.
# Note: When using testdox, there is no export of the result of the visited
# pages. Just leave EXPORT empty if you need it.
EXPORT='--testdox'
if [ -n "${CI_PROJECT_DIR}" ]; then
  # Temporary directory as the phpunit command is executed as the webserver
  # user.
  EXPORT="--log-junit /tmp/phpunit.xml"
fi

ERROR=''

echo -e "${COLOR_LIGHT_GREEN}Create the folder sites/simpletest.${COLOR_NC}"
mkdir -p "${APP_PATH}"/sites/simpletest

echo -e "${COLOR_LIGHT_GREEN}Give permissions to webserver user on sites/simpletest.${COLOR_NC}"
chmod 0777 -R "${APP_PATH}"/sites/simpletest

echo -e "${COLOR_LIGHT_GREEN}Workaround for drupal-composer/drupal-paranoia make symlink to sites/simpletest.${COLOR_NC}"
rm -rf "${WWW_PATH}"/sites/simpletest
ln -s "${APP_PATH}"/sites/simpletest "${WWW_PATH}"/sites/simpletest

. "${SCRIPTS_PATH}"/tests/phpunit/manage-tests-module.sh --create

echo -e "${COLOR_LIGHT_GREEN}Run tests.${COLOR_NC}"
# shellcheck disable=SC2086
if ! sudo -u "${WEBSERVER_USER}" -E "${PROJECT_PATH}"/vendor/bin/phpunit -c "${PROJECT_PATH}"/scripts/tests/phpunit/phpunit.xml.dist ${EXPORT}; then
  echo -e "${COLOR_LIGHT_RED}PHPUnit tests failed.${COLOR_NC}"
  ERROR='yes'
fi

. "${SCRIPTS_PATH}"/tests/phpunit/manage-tests-module.sh --remove

# Report management.
if [ -n "${CI_PROJECT_DIR}" ]; then
  mv /tmp/phpunit.xml "${PROJECT_PATH}/phpunit.xml"
fi

if [ "${ERROR}" = "yes" ]; then
  exit 1
fi
