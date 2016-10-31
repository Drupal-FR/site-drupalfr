#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

. $PROJECT_PATH/scripts/script-parameters.sh
. $PROJECT_PATH/scripts/script-parameters.local.sh

DRUPAL_VERSION=${1:-drupal-8}
DRUPAL_TEMP=$(mktemp -d)

# Download Drupal in temp directory.
$DRUSH dl "$DRUPAL_VERSION" --destination=$DRUPAL_TEMP --drupal-project-rename=drupal-8 --quiet -y

# Create default directory in case it does not exist yet.
mkdir -p $WWW_PATH/sites/default

# Update specific files.
rsync -avz $DRUPAL_TEMP/drupal-8/autoload.php                       $WWW_PATH/autoload.php
rsync -avz $DRUPAL_TEMP/drupal-8/index.php                          $WWW_PATH/index.php

rsync -avz $DRUPAL_TEMP/drupal-8/.htaccess                          $WWW_PATH/.htaccess
# Do not erase existing robots.txt file.
rsync -avz --ignore-existing $DRUPAL_TEMP/drupal-8/robots.txt       $WWW_PATH/robots.txt

# Add temporarily write permission to default directory.
chmod u+w $WWW_PATH/sites/default

rsync -avz $DRUPAL_TEMP/drupal-8/sites/default/default.services.yml $WWW_PATH/sites/default/default.services.yml
rsync -avz $DRUPAL_TEMP/drupal-8/sites/default/default.settings.php $WWW_PATH/sites/default/default.settings.php
rsync -avz $DRUPAL_TEMP/drupal-8/sites/example.sites.php            $WWW_PATH/sites/example.sites.php

# Remove write permission to default directory.
chmod u-w $WWW_PATH/sites/default

# Development files.
if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  rsync -avz $DRUPAL_TEMP/drupal-8/.csslintrc                       $WWW_PATH/.csslintrc
  rsync -avz $DRUPAL_TEMP/drupal-8/.editorconfig                    $WWW_PATH/.editorconfig
  rsync -avz $DRUPAL_TEMP/drupal-8/.eslintignore                    $WWW_PATH/.eslintignore
  rsync -avz $DRUPAL_TEMP/drupal-8/.eslintrc                        $WWW_PATH/.eslintrc
  rsync -avz $DRUPAL_TEMP/drupal-8/.gitattributes                   $WWW_PATH/.gitattributes
fi

rm -rf $DRUPAL_TEMP/drupal-8
