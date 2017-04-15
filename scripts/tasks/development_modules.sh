#!/bin/bash

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh
. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.local.sh

# Enable development modules.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  MODULES=''
  for DEVELOPMENT_MODULE in "${DEVELOPMENT_MODULES[@]}"
  do
    MODULES="$MODULES $DEVELOPMENT_MODULE"
  done
  $DRUSH en $MODULES -y
fi
