#!/usr/bin/env bash

PRINTABLE_DRUPAL_SITES_LIST=$DRUPAL_SITES_LIST

usage() {
  printf "selection-site.sh local default\n"
  printf "selection-site.sh dev site_1\n"
  printf "selection-site.sh prod all\n"
  printf "Available sites are: all,%s\n" "${PRINTABLE_DRUPAL_SITES_LIST}"
}

# Check that the argument is there.
if [ -z "$1" ]; then
  echo "Selected site is missing."
  usage
  exit 1
fi

IFS=',' read -ra DRUPAL_SITES_LIST <<< "$DRUPAL_SITES_LIST"

if [ "$1" != "all" ]; then
  # Check that the selected site is among the available sites.
  FOUND=''
  for DRUPAL_SITE_SELECTION in "${DRUPAL_SITES_LIST[@]}"
  do
    if [ "${DRUPAL_SITE_SELECTION}" == "$1" ]; then
      DRUPAL_SITES_LIST=("$1")
      FOUND='yes'
    fi
  done

  if [ "${FOUND}" != "yes" ]; then
    echo "The selected site is not available."
    usage
    exit 1
  fi
fi
