#!/usr/bin/env bash

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "$(dirname "$(dirname "${BASH_SOURCE[0]}")")")"/script-parameters.sh local

usage() {
  printf "\n"
  printf "Usage:\n\t%s --create|--remove\n" "$0"
  printf "\n"
  printf "Options:\n"
  printf "\t--create : Create the tests module.\n"
  printf "\t--remove : Remove the tests module.\n"
  printf "\n"
  exit 0
}

CREATE=false
REMOVE=false;

case "$1" in
  -h)
    usage
    ;;
  --create)
    CREATE=true
    ;;
  --remove)
    REMOVE=true
    ;;
esac

if [ "$CREATE" = false ] && [ "$REMOVE" = false ]; then
  echo -e "${COLOR_LIGHT_RED}Error: Please either use the --create or the --remove option.${COLOR_NC}"
  exit 1
fi

TESTS_MODULE_ROOT_DIR="${APP_PATH}/modules/custom/project_tests"
TESTS_MODULE_CONFIG_DIR="$TESTS_MODULE_ROOT_DIR/config/optional"

remove_tests_module() {
  if [ -d "$TESTS_MODULE_ROOT_DIR" ]; then
    echo -e "${COLOR_LIGHT_GREEN}Remove the tests module.${COLOR_NC}"
    rm -rf "$TESTS_MODULE_ROOT_DIR"
  fi
}

create_tests_module() {
  remove_tests_module

  echo -e "${COLOR_LIGHT_GREEN}Create the tests module folder.${COLOR_NC}"
  mkdir -p "$TESTS_MODULE_CONFIG_DIR"

  echo -e "${COLOR_LIGHT_GREEN}Copy project configuration the tests module.${COLOR_NC}"
  printf "name: 'Project Tests'\ntype: module\npackage: 'Testing'" > "$TESTS_MODULE_ROOT_DIR/project_tests.info.yml"

  echo -e "${COLOR_LIGHT_GREEN}Copy configuration into the tests module.${COLOR_NC}"
  find "${PROJECT_PATH}/conf/drupal/${DRUPAL_SITE_DEFAULT_FOLDER_NAME}/sync" \
      -maxdepth 1 \
      -type f \
      -name "*.yml" \
      -not -name "core.extension.yml" \
      -not -name "system.site.yml" \
      -exec cp {} "$TESTS_MODULE_CONFIG_DIR" \;

  echo -e "${COLOR_LIGHT_GREEN}Remove uuid key from config files.${COLOR_NC}"
  find "$TESTS_MODULE_CONFIG_DIR" \
      -type f \
      -exec sed -i '1 s/^uuid:.*//' {} \;
}

if [ "$CREATE" = true ]; then
  create_tests_module
fi

if [ "$REMOVE" = true ]; then
  remove_tests_module
fi
