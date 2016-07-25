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

if [ "${ENVIRONMENT_MODE}" = "dev" ]; then
  # Highlight.js.
  mkdir -p $WWW_PATH/libraries/highlight/styles
  wget http://cdn.jsdelivr.net/highlight.js/$HIGHLIGHT_JS_VERSION/highlight.min.js       -O $WWW_PATH/libraries/highlight/highlight.pack.js
  wget http://cdn.jsdelivr.net/highlight.js/$HIGHLIGHT_JS_VERSION/styles/default.min.css -O $WWW_PATH/libraries/highlight/styles/idea.css

  # D3.js.
  mkdir -p $WWW_PATH/libraries/d3
  wget https://github.com/mbostock/d3/releases/download/v$D3_VERSION/d3.zip -P $LIBRARIES_TEMP
  unzip -o $LIBRARIES_TEMP/d3.zip -d $WWW_PATH/libraries/d3
fi

rm -rf $LIBRARIES_TEMP
