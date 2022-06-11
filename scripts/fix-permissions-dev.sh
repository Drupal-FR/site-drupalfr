#!/usr/bin/env bash

# This script is though to be run as root.
# Use this script only to fix permissions on a dev environment where you don't
# care about permissions and only want things to work.

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh local
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$1"

usage() {
  printf "fix-permissions-dev.sh all 1000 1000\n"
  printf "fix-permissions-dev.sh site1 14590 10000\n"
}

host_uid=''
host_gid=''

if [ -n "$2" ]; then
  host_uid=$2
fi

if [ -n "$3" ]; then
  host_gid=$3
fi

# Check that all required parameters are present.
if [ -z "${host_uid}" ]; then
  echo "Missing host uid parameter."
  usage
  exit 1
elif [ -z "${host_gid}" ]; then
  echo "Missing host gid parameter."
  usage
  exit 1
fi

echo -e "${COLOR_LIGHT_GREEN}Change owner of folders.${COLOR_NC}"
chown "${host_uid}":"${host_gid}" -R \
  "${PROJECT_PATH}"/app \
  "${PROJECT_PATH}"/backups \
  "${PROJECT_PATH}"/build \
  "${PROJECT_PATH}"/conf \
  "${PROJECT_PATH}"/vendor

echo -e "${COLOR_LIGHT_GREEN}Change permissions of temporary directory.${COLOR_NC}"
chmod 0777 -R /tmp

# shellcheck disable=SC2034
for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  . "${SCRIPTS_PATH}"/tasks/fix_stream_wrappers_permissions.sh
done
