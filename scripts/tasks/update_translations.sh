#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

. $PROJECT_PATH/scripts/script-parameters.sh
. $PROJECT_PATH/scripts/script-parameters.local.sh

$DRUSH locale-check
$DRUSH locale-update
$DRUSH language-import $DEFAULT_LANGUAGE $WWW_PATH/profiles/$PROFILE/translations/$PROFILE.po --replace
