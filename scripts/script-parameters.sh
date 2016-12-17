#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

CURRENT_PATH=$(pwd)

SCRIPTS_PATH=$PROJECT_PATH/scripts
WWW_PATH=$PROJECT_PATH/www

DRUSH=$WWW_PATH/vendor/bin/drush

CURRENT_DATE=$(date "+%Y-%m-%d-%Hh%Mm%Ss")

PROFILE=drupalfr
DEFAULT_LANGUAGE=fr
DEVELOPMENT_MODULES=(
  config_inspector
  dblog
  devel
  devel_a11y
  features_ui
  field_ui
  kint
  renderviz
  search_kint
  views_ui
  webprofiler
)

# External libraries version.
HIGHLIGHT_JS_VERSION=9.3.0
D3_VERSION=3.5.16
DROPZONE_VERSION=4.3.0
