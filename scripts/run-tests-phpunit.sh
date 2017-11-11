#!/bin/bash

usage() {
    echo "run-test-phpunit.sh arguments"
}

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh
. $(dirname ${BASH_SOURCE[0]})/script-parameters.local.sh

# Without drush alias, change temporarily directory to www.
cd $WWW_PATH

# Disable external cache.
rm -f $WWW_PATH/sites/default/.cache_activated

# Start Phantomjs in case of Javascript tests.
$PROJECT_PATH/vendor/bin/phantomjs --ssl-protocol=any --ignore-ssl-errors=true $PROJECT_PATH/vendor/jcalderonzumba/gastonjs/src/Client/main.js 8510 1024 768 2>&1 >> /dev/null &

# Run tests.
sudo -u $WEBSERVER_USER -E $PROJECT_PATH/vendor/bin/phpunit -c $PROJECT_PATH/scripts/tests/phpunit.xml $@

# Stop Phantomjs process. A bit brutal, maybe there is a better way.
pgrep phantomjs | xargs kill

# Enable external cache.
touch $WWW_PATH/sites/default/.cache_activated
$DRUSH cache:rebuild

# Back to the current directory.
cd $CURRENT_PATH
