#!/bin/bash

# You should have the following tools installed globally:
# - PHPCS
# - PHPCBF
# - Drupal CS registered
# - ESLint

usage() {
    echo "check-fix-standards.sh path_to_check"
}

while getopts "h" opt; do
    case "${opt}" in
        h)
            usage; exit 0;;
        \?)
            usage >&2; exit 1;;
    esac
done

if [ -z "$1" ] ; then
    echo 'The path to check is required';
    exit 1;
fi

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh
. $(dirname ${BASH_SOURCE[0]})/script-parameters.local.sh


echo -e "${LIGHT_GREEN}PHP Code Sniffer: Drupal${NC}"
phpcs --standard=Drupal --extensions='php,module,inc,install,test,profile,theme,css,info,txt,md' $1

echo -e "${LIGHT_GREEN}PHP Code Sniffer: DrupalPractice${NC}"
phpcs --standard=DrupalPractice --extensions='php,module,inc,install,test,profile,theme,css,info,txt,md' $1

echo -e "${LIGHT_GREEN}PHP Code Beautifier: Drupal${NC}"
phpcbf --standard=Drupal --extensions='php,module,inc,install,test,profile,theme,css,info,txt,md' $1

echo -e "${LIGHT_GREEN}ESLint${NC}"
eslint -c $WWW_PATH/core/.eslintrc.json --fix $1
