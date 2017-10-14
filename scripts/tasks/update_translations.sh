#!/bin/bash

. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.sh
. $(dirname $(dirname ${BASH_SOURCE[0]}))/script-parameters.local.sh

$DRUSH locale:check
$DRUSH locale:update

$DRUSH pm:enable drush_language
$DRUSH language:import:translations $DEFAULT_LANGUAGE $WWW_PATH/profiles/custom/$PROFILE/translations/$PROFILE.po --replace
$DRUSH pm:uninstall drush_language
