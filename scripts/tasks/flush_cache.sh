#!/usr/bin/env bash

CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"

echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Flush caches to be clean.${COLOR_NC}"
$DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" cache:rebuild
