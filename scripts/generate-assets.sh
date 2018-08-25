#!/bin/bash

set -e

# You should have the following tools installed globally:
# - npm

usage() {
    echo "generate-assets.sh build|watch"
}

while getopts "h" opt; do
    case "${opt}" in
        h)
            usage; exit 0;;
        \?)
            usage >&2; exit 1;;
    esac
done

if [ -z "$1" ]
then
    ASSETS_MODE='build'
else
    ASSETS_MODE="$1"
fi

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh

echo -e "${LIGHT_GREEN}Change temporarily directory.${NC}"
cd $WWW_PATH/themes/custom/drupal_france

echo -e "${LIGHT_GREEN}Install npm packages.${NC}"
npm install

echo -e "${LIGHT_GREEN}Generate assets with mode: ${ASSETS_MODE}${NC}"
npm run-script $ASSETS_MODE

echo -e "${LIGHT_GREEN}Back to the current directory.${NC}"
cd $CURRENT_PATH
