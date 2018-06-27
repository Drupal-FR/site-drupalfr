#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh
. $(dirname ${BASH_SOURCE[0]})/script-parameters.local.sh

echo -e "${LIGHT_GREEN}Run behat tests.${NC}"
$PROJECT_PATH/vendor/bin/behat --config="${SCRIPTS_PATH}/tests/behat/behat.yml"
