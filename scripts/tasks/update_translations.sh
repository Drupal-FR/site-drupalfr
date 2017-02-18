#!/bin/bash

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh
. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.local.sh

$DRUSH locale-check
$DRUSH locale-update
$DRUSH language-import $DEFAULT_LANGUAGE $WWW_PATH/profiles/$PROFILE/translations/$PROFILE.po --replace
