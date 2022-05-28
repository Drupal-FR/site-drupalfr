#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/script-parameters.sh

echo -e "${COLOR_LIGHT_GREEN}Run behat tests.${COLOR_NC}"
# When executed in Gitlab CI, adds report export.
if [ -n "${CI_PROJECT_DIR}" ]; then
  mkdir -p "${PROJECT_PATH}/report"
  "${PROJECT_PATH}"/vendor/bin/behat \
    --profile="${BEHAT_PROFILE}" \
    --config="${SCRIPTS_PATH}/tests/behat/behat.yml" \
    --format=junit \
    --out="${PROJECT_PATH}/report"
else
  "${PROJECT_PATH}"/vendor/bin/behat \
  --profile="${BEHAT_PROFILE}" \
  --config="${SCRIPTS_PATH}/tests/behat/behat.yml"
fi
