#!/bin/bash

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh
. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.local.sh

# Installation.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
    composer install --working-dir=$PROJECT_PATH
else
    composer install --working-dir=$PROJECT_PATH --no-dev
fi
