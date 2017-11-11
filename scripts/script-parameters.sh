#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

PROJECT_PATH=$(abspath $(dirname $(dirname ${BASH_SOURCE[0]})))

CURRENT_PATH=$(pwd)

SCRIPTS_PATH=$PROJECT_PATH/scripts
WWW_PATH=$PROJECT_PATH/www

DRUSH=$PROJECT_PATH/vendor/bin/drush

CURRENT_DATE=$(date "+%Y-%m-%d-%Hh%Mm%Ss")

PROFILE="drupalfr"
DEFAULT_LANGUAGE="fr"
DEVELOPMENT_MODULES=(
  blazy_ui
  config_inspector
  context_ui
  dblog
  devel
#  devel_a11y
  devel_generate
  field_ui
#  kint
  purge_ui
#  renderviz
#  search_kint
  serialization
  views_ui
  webprofiler
)

WEBSERVER_USER='www-data'

# Colors.
NC='\033[0m' # No Color
BLACK='\033[0;30m'
DARK_GRAY='\033[1;30m'
RED='\033[0;31m'
LIGHT_RED='\033[1;31m'
GREEN='\033[0;32m'
LIGHT_GREEN='\033[1;32m'
BROWN_ORANGE='\033[0;33m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
LIGHT_BLUE='\033[1;34m'
PURPLE='\033[0;35m'
LIGHT_PURPLE='\033[1;35m'
CYAN='\033[0;36m'
LIGHT_CYAN='\033[1;36m'
LIGHT_GRAY='\033[0;37m'
WHITE='\033[1;37m'
