#!/usr/bin/env bash

# Ensure ENV_VARIABLES_WITH_SECRETS is defined.
# shellcheck disable=SC2153
if [ -z ${ENV_VARIABLES_WITH_SECRETS+x} ]; then
  echo "secrets-replacement.sh should not be called directly. But only by scripts-parametrers.sh."
  exit 1
fi

for ENV_VARIABLE_WITH_SECRET in "${ENV_VARIABLES_WITH_SECRETS[@]}"
do
  # Gitlab CI.
  if [ -n "${CI_PROJECT_DIR}" ]; then
    if [ -n "${SELECTED_ENV}" ]; then
      EXPECTED_GITLAB_CI_VARIABLE_NAME="${ENV_VARIABLE_WITH_SECRET}_${SELECTED_ENV}"
      if [ -n "${!EXPECTED_GITLAB_CI_VARIABLE_NAME}" ]; then
        declare "$ENV_VARIABLE_WITH_SECRET"="${!EXPECTED_GITLAB_CI_VARIABLE_NAME}"
      else
        echo -e "${COLOR_BROWN_ORANGE}Variable ${EXPECTED_GITLAB_CI_VARIABLE_NAME} not configured in Gitlab CI.${COLOR_NC}"
      fi
    else
      echo -e "${COLOR_LIGHT_RED}SELECTED_ENV variable not exported by the selection-environment.sh script.${COLOR_NC}"
      exit 1
    fi
  # Bitwarden.
  elif [ "${!ENV_VARIABLE_WITH_SECRET:0:3}" == 'bw:' ]; then
    if ! command -v bw &> /dev/null; then
      echo -e "${COLOR_LIGHT_RED}Bitwarden CLI is not installed. Impossible to use secrets.${COLOR_NC}"
      exit 1
    fi

    if [ -z "${BW_SESSION}" ]; then
      echo -e "${COLOR_LIGHT_RED}Bitwarden session not found. Please unlock your vault with 'bw login' or 'bw unlock' (if already logged in) and then export your Bitwarden session.${COLOR_NC}"
      exit 1
    fi

    ENV_VARIABLE_WITH_SECRET_VALUE=${!ENV_VARIABLE_WITH_SECRET}
    BW_ITEM_ID=${!ENV_VARIABLE_WITH_SECRET:3:${#ENV_VARIABLE_WITH_SECRET_VALUE}}
    BW_ITEM_VALUE=$(bw get password "${BW_ITEM_ID}" --session "${BW_SESSION}")

    if [ -n "${BW_ITEM_VALUE}" ]; then
      declare "$ENV_VARIABLE_WITH_SECRET"="${BW_ITEM_VALUE}"
    else
      echo -e "${COLOR_LIGHT_RED}Bitwarden item empty or not found for ${ENV_VARIABLE_WITH_SECRET}.${COLOR_NC}"
      exit 1
    fi
  fi
done
