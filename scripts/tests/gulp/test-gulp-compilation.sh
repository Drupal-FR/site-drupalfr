#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/script-parameters.sh local

##############################################
# Files to be checked (absolute or relative to
# project root).
##############################################

FILES_WHICH_SHOULD_EXIST=(
  # Let's test custom 'mod1' module,, with a mod1_js1.es6.js file:
  # Relative example
  #"app/modules/custom/mod1/assets/js/mod1_js1.js"
  # Absolute example
  #"/project/app/modules/custom/mod1/assets/js/mod1_js1.js"
)

##############################################
# Test logic.
##############################################

FILES_NOT_FOUND=()

# Test each file existence.
for FILE in "${FILES_WHICH_SHOULD_EXIST[@]}"
do
  echo -e "Testing existence of ${COLOR_LIGHT_GREEN}\"${FILE}\"${COLOR_NC}"

  if [ ! -f ${FILE} ]; then
    FILES_NOT_FOUND+=("${FILE}")
  fi
done

if [ ! "0" -eq ${#FILES_NOT_FOUND[@]} ]; then
  echo -e "${COLOR_LIGHT_RED}${#FILES_NOT_FOUND[@]} file(s) not found${COLOR_NC}"

  for FILE in "${FILES_NOT_FOUND[@]}"
  do
    echo -e ${FILE}
  done

  exit 1;
fi

echo -e "${COLOR_LIGHT_GREEN}\o/ All files exist${COLOR_NC}"

exit 0;
