#!/usr/bin/env bash

CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"

echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Launch database updates.${COLOR_NC}"
$DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" updatedb --no-cache-clear -y

echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Launch database updates a second time. Just in case...${COLOR_NC}"
$DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" updatedb --no-cache-clear -y
