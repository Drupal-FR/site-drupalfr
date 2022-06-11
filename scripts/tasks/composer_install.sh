#!/usr/bin/env bash

# Check that the argument is there.
if [ -z "$1" ]; then
  echo "Working directory is missing."
  usage
  exit 1
fi

echo -e "${COLOR_LIGHT_GREEN}Composer install.${COLOR_NC}"
if [ "${COMPOSER_ADD_REQUIRE_DEV}" = "yes" ]; then
    composer install --working-dir="$1" --no-interaction
else
    composer install --working-dir="$1" --no-interaction --no-dev
fi

echo -e "${COLOR_LIGHT_GREEN}Rebuild Drupal paranoia.${COLOR_NC}"
composer drupal:paranoia --working-dir="$1"

echo -e "${COLOR_LIGHT_GREEN}Clear Drush cache in case of Drush update.${COLOR_NC}"
$DRUSH cache:clear drush
