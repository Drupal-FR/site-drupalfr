#!/usr/bin/env bash

CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"

echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Install the modules required to import translations.${COLOR_NC}"
$DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" pm:install locale -y

echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Update translations status.${COLOR_NC}"
$DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" locale:check

echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Update translations.${COLOR_NC}"
$DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" locale:update

echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Import custom translations.${COLOR_NC}"
# shellcheck disable=SC2044
# Expected translation file pattern is "XXX.[langcode].po".
for TRANSLATION_FILE_PATH in $(find "${APP_PATH}"/translations/custom/*.po -type f)
do
  FILE_NAME=$(basename "${TRANSLATION_FILE_PATH}")
  LANGCODE=$(echo "${FILE_NAME}" | cut -d'.' -f2)
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" locale:import \
    "${LANGCODE}" \
    "${TRANSLATION_FILE_PATH}" \
    --type=not-customized \
    --override=all
done
