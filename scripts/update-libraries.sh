#!/bin/bash

function abspath() {
    python -c "import sys, os;sys.stdout.write(os.path.abspath(\"$@\"))"
}

FILE_PATH=$(abspath "${0}")
PROJECT_PATH=$(dirname $(dirname $FILE_PATH))

. $PROJECT_PATH/scripts/script-parameters.sh
. $PROJECT_PATH/scripts/script-parameters.local.sh

LIBRARIES_TEMP=$(mktemp -d)

# Create libraries directory in case it does not exist yet.
mkdir -p $WWW_PATH/libraries

# Dropzone.
mkdir -p $WWW_PATH/libraries/dropzone
wget https://github.com/enyo/dropzone/archive/v$DROPZONE_VERSION.zip                     -O $LIBRARIES_TEMP/dropzone.zip
unzip -o $LIBRARIES_TEMP/dropzone.zip -d $LIBRARIES_TEMP
rm -rf $LIBRARIES_TEMP/dropzone-$DROPZONE_VERSION/test
rsync -avz --delete $LIBRARIES_TEMP/dropzone-$DROPZONE_VERSION/                          $WWW_PATH/libraries/dropzone

rm -rf $LIBRARIES_TEMP
