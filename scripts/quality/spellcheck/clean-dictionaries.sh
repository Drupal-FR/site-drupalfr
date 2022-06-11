#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/script-parameters.sh local

shopt -s globstar nullglob

for DICTIONARY_FILE_PATH in "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/../scripts/quality/spellcheck/*.txt
do
  # Ensure what is detected is a file.
  if [ -f "${DICTIONARY_FILE_PATH}" ]; then
    echo -e "${COLOR_LIGHT_GREEN}Clean dictionary: ${DICTIONARY_FILE_PATH}${COLOR_NC}"
    cat ${DICTIONARY_FILE_PATH} | tr '[:upper:]' '[:lower:]' | LC_ALL=C sort -u -o ${DICTIONARY_FILE_PATH}
  fi
done
