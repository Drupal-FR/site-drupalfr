#!/bin/bash

set -e

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh

echo -e "${LIGHT_GREEN}Update translations status.${NC}"
$DRUSH locale:check

echo -e "${LIGHT_GREEN}Update translations.${NC}"
$DRUSH locale:update

echo -e "${LIGHT_GREEN}Import custom translations.${NC}"
$DRUSH pm:enable drush_language -y
$DRUSH language:import:translations --langcode="${DEFAULT_LANGUAGE}" $WWW_PATH/translations/custom/drupalfr.po
