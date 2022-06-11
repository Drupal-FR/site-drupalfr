#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/script-parameters.sh local

shopt -s globstar nullglob

ERROR=''

for COMPOSER_JSON_FILE_PATH in "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/../app/{modules,profiles,themes}/custom/**/composer.json
do
  # Ensure what is detected is a file.
  if [ -f "${COMPOSER_JSON_FILE_PATH}" ]; then
    echo -e "${COLOR_LIGHT_GREEN}Composer validate on file: ${COMPOSER_JSON_FILE_PATH}${COLOR_NC}"

    DIRECTORY_PATH="$(dirname "${COMPOSER_JSON_FILE_PATH}")"
    if ! composer validate --working-dir="${DIRECTORY_PATH}"; then
      echo -e "${COLOR_LIGHT_RED}Composer validate failed on file: ${COMPOSER_JSON_FILE_PATH}${COLOR_NC}"
      ERROR='yes'
    fi
  fi
done

if [ "${ERROR}" = "yes" ]; then
  exit 1
fi
