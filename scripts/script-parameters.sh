#!/usr/bin/env bash

if [ -z "$1" ]; then
  # shellcheck source=scripts/selection-environment.sh
  . "$(dirname "${BASH_SOURCE[0]}")"/selection-environment.sh local
else
  # shellcheck source=scripts/selection-environment.sh
  . "$(dirname "${BASH_SOURCE[0]}")"/selection-environment.sh "$1"
fi

# Test if jq exists so script-parameters.sh can be included when launched from
# host machine.
if command -v jq &> /dev/null; then
  # Export BEHAT_PARAMS dynamically.
  BEHAT_PARAMS=$(jq -n \
                 --arg APP_PATH "$APP_PATH" \
                 '{"extensions":{"Drupal\\DrupalExtension":{"drupal":{"drupal_root":$APP_PATH}}}}')
  export BEHAT_PARAMS
fi

# shellcheck disable=SC2034
CURRENT_PATH=$(pwd)
# shellcheck disable=SC2034
CURRENT_DATE=$(date "+%Y-%m-%d-%Hh%Mm%Ss")

# shellcheck disable=SC2034
DEVELOPMENT_MODULES=(
  config_devel
  config_inspector
  dblog
  devel
  devel_a11y
  devel_generate
  devel_php
  field_ui
#  purge_ui
#  renderviz
  speedboxes
  upgrade_status
  views_ui
  # As in this template we not use custom install profile, enable some modules.
  admin_toolbar_tools
  health_check
#  purge
#  purge_drush
#  purge_queuer_coretags
#  purge_processor_cron
#  purge_ui
  redis
#  search_api
#  search_api_solr
#  varnish_purger
#  varnish_purge_tags
)

# shellcheck disable=SC2034
FILES_EXCLUDED_FROM_PACKAGE=(
#  app/themes/custom/my_theme/assets/images/sources
#  app/themes/custom/my_theme/assets/scss
  app/sites/development.services.yml
  app/sites/example.settings.local.php
  app/sites/example.sites.php
  app/.eslintignore
  app/.ht.router.php
  backups
  conf/drupal/example.sites.php
  conf/env/example.composer.env
  docker
  docs
  drush/README.md
  scripts/assets
  scripts/quality
  scripts/tests
  scripts/contrib-sync.sh
  scripts/contrib-update.sh
  scripts/deploy-package.sh
  scripts/init.sh
  scripts/package-composer-install.sh
  scripts/package-finalize.sh
  .editorconfig
  .git
  .gitattributes
  .gitignore
  .gitlab-ci.yml
  docker-compose.yml
  example.dev.env
  example.gitlab.env
  example.integ.env
  example.preprod.env
  example.prod.env
  example.staging.env
  README.md
)

# shellcheck disable=SC2034
FILES_EXCLUDED_FROM_DOCKER_SYNC=(
  backups
# Once the project is initialized, a real Github Token should be versioned in
# the example.composer.env file.
#  conf/env/composer.env
  data
)

# shellcheck disable=SC2034
PUBLIC_FILES_EXCLUDED_FROM_BACKUP=(
  css
  js
  languages
  matomo
  php
  styles
)

# shellcheck disable=SC2034
PRIVATE_FILES_EXCLUDED_FROM_BACKUP=(
  styles
)

# shellcheck disable=SC2034
ENV_VARIABLES_WITH_SECRETS=(
  DRUPAL_SITE_DEFAULT_ACCOUNT_PASS
  # Only used for Docker environment.
#  DRUPAL_SITE_DEFAULT_DB_ROOT_PASSWORD
  DRUPAL_SITE_DEFAULT_DB_PASSWORD
)

# shellcheck source=scripts/secrets-replacement.sh
. "$(dirname "${BASH_SOURCE[0]}")"/secrets-replacement.sh
