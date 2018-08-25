#!/bin/bash

set -e

usage() {
    echo "run-test-phpunit.sh arguments"
}

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh

echo -e "${LIGHT_GREEN}Without drush alias, change temporarily directory to www.${NC}"
cd $WWW_PATH

echo -e "${LIGHT_GREEN}Start Phantomjs in case of Javascript tests.${NC}"
$PROJECT_PATH/vendor/bin/phantomjs --ssl-protocol=any --ignore-ssl-errors=true $PROJECT_PATH/vendor/jcalderonzumba/gastonjs/src/Client/main.js 8510 1024 768 2>&1 >> /dev/null &

echo -e "${LIGHT_GREEN}Run tests.${NC}"
sudo -u $WEBSERVER_USER -E $PROJECT_PATH/vendor/bin/phpunit -c $PROJECT_PATH/scripts/tests/phpunit.xml $@

echo -e "${LIGHT_GREEN}Stop Phantomjs process. A bit brutal, maybe there is a better way.${NC}"
pgrep phantomjs | xargs kill

echo -e "${LIGHT_GREEN}Back to the current directory.${NC}"
cd $CURRENT_PATH
