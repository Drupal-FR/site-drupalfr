#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh

echo -e "${LIGHT_GREEN}Copy example files.${NC}"
rsync -avz --ignore-existing $PROJECT_PATH/example.docker-compose.yml           $PROJECT_PATH/docker-compose.yml
rsync -avz --ignore-existing $SCRIPTS_PATH/example.script-parameters.local.sh   $SCRIPTS_PATH/script-parameters.local.sh
rsync -avz --ignore-existing $PROJECT_PATH/conf/drupal/default/example.settings.local.php $PROJECT_PATH/conf/drupal/default/settings.local.php

echo -e "${LIGHT_GREEN}Create public files directory.${NC}"
mkdir -p $WWW_PATH/sites/default/files

echo -e "${LIGHT_GREEN}Ensure the following directories are present. Otherwise automated tests fail.${NC}"
mkdir -p $WWW_PATH/modules
mkdir -p $WWW_PATH/profiles
mkdir -p $WWW_PATH/themes

echo -e "${LIGHT_GREEN}Permissions are for dev environments. It should be less permissive.${NC}"
chmod 777 $WWW_PATH/sites/default/files
