#!/bin/bash

set -e

. $(dirname ${BASH_SOURCE[0]})/script-parameters.sh

# Dropzone.
rm -rf $WWW_PATH/libraries/dropzone/test
