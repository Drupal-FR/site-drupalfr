#!/usr/bin/env bash

usage() {
  printf "selection-environment.sh local\n"
  printf "selection-environment.sh dev\n"
  printf "selection-environment.sh prod\n"
}

environment=''

if [ -n "$1" ]; then
  environment=$1
fi

# Check that all required parameters are present.
if [ -z "${environment}" ]; then
  echo "Missing target environment parameter."
  usage
  exit 1
fi

if [ "$1" != "local" ]; then
  environment_filename=example."${environment}".env
  export SELECTED_ENV=${environment^^}
else
  environment_filename=.env
  export SELECTED_ENV=local
fi

# Check that the env file exists.
if [ ! -f "$(dirname "${BASH_SOURCE[0]}")"/../"${environment_filename}" ]; then
  echo "The target environment file does not exist."
  usage
  exit 1
fi

# Load variables from target environment example env file.
set -o allexport
# shellcheck disable=SC1091
# shellcheck source=example.prod.env
source "$(dirname "${BASH_SOURCE[0]}")"/../"${environment_filename}"
set +o allexport
