#!/bin/bash

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh
. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.local.sh

# Enable development modules.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  for MODULE in "${DEVELOPMENT_MODULES[@]}"
  do
    $DRUSH en $MODULE -y
  done
fi
