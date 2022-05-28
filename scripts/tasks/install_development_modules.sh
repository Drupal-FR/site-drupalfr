#!/usr/bin/env bash

CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"

if [ "${DRUPAL_INSTALL_DEVELOPMENT_MODULES}" = "yes" ]; then
  echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Install development modules.${COLOR_NC}"
  MODULES=''
  # shellcheck disable=2153
  for DEVELOPMENT_MODULE in "${DEVELOPMENT_MODULES[@]}"
  do
    MODULES="${MODULES} ${DEVELOPMENT_MODULE}"
  done
  # shellcheck disable=2086
  # Avoid double quotes around $MODULES because we specifically wants word
  # splitting.
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" pm:install ${MODULES} -y
fi
