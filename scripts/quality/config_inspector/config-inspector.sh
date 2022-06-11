#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/script-parameters.sh local
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/selection-site.sh "$@"

for DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  CURRENT_SITE_DRUSH_ALIAS="DRUPAL_SITE_${DRUPAL_SITE^^}_DRUSH_ALIAS"

  echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Install Configuration Inspector.${COLOR_NC}"
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" pm:install config_inspector -y

  echo -e "${COLOR_LIGHT_GREEN}${DRUPAL_SITE}: Inspect for configuration schema error.${COLOR_NC}"
  $DRUSH "${!CURRENT_SITE_DRUSH_ALIAS}" config:inspect --only-error
done
