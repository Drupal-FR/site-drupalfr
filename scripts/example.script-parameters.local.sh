#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

# Used to know if we want to download and enable development modules.
ENVIRONMENT_MODE="dev"
# Only "dev" is searched. So any other string can prevent development modules.
ENVIRONMENT_MODE="prod"

# For Drush site-install.
ACCOUNT_MAIL=admin@example.com
ACCOUNT_NAME=admin
ACCOUNT_PASS=admin
SITE_MAIL=admin@example.com
SITE_NAME="Drupalfr"
