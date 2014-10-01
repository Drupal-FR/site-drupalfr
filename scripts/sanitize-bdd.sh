#!/bin/bash

#@file
# dump Drupal database, import in temporary base and clean sensitive information before redump
#
# IMPORTANT
# You need to create a mutlisite named as $tmpsite in sites/ and set a valid settings.php on the $tmpsite databases
#
#@author Guillaume Bec <guillaume.bec@gmail.com>

TMPSITE=sandfr
EXPORTFILE="/var/www/site-drupalfr/export/sandfr-$(date +%F).sql.gz"
DRUPAL=/var/www/site-drupalfr/www7
SANSQL=/var/www/site-drupalfr/scripts/sanitize.sql
CONFDRUSH=/var/www/site-drupalfr/scripts/drushrc.php
DRUSH=/usr/local/bin/drush

$DRUSH sql-sync -y -r $DRUPAL -c $CONFDRUSH --structure-tables-key=sanitize default $TMPSITE
#$DRUSH sql-sync -y -r $DRUPAL -c $CONFDRUSH --structure-tables-key=sanitize --create-db default $TMPSITE
echo "SYNC DONE"
$DRUSH -y -r $DRUPAL -l $TMPSITE sql-cli < $SANSQL
echo "APPLY SANITIZE"
$DRUSH -y -r $DRUPAL -l $TMPSITE sql-dump --gzip > $EXPORTFILE
echo "EXPORT DUMP"
