#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

. $PROJECT_PATH/scripts/script-parameters.sh
. $PROJECT_PATH/scripts/script-parameters.local.sh

# Enable development modules.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  for MODULE in "${DEVELOPMENT_MODULES[@]}"
  do
    $DRUSH en $MODULE -y
  done
fi
