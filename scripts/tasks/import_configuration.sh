#!/usr/bin/env bash

CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"
CURRENT_SITE_HAS_EXPORTED_CONFIG="DRUPAL_SITE_${DRUPAL_SITE^^}_HAS_EXPORTED_CONFIG"

if [ "${!CURRENT_SITE_HAS_EXPORTED_CONFIG}" = "true" ]; then
  echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Import configuration.${COLOR_NC}"
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" config:import -y

  echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Import configuration a second time. In case the config of modules altering configuration import has changed.${COLOR_NC}"
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" config:import -y
fi
