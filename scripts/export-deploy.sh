#!/bin/bash

#@file
# dump Drupal database, import in temporary base and clean sensitive information before redump
#
# IMPORTANT
# You need to create a mutlisite named as $tmpsite in sites/ and set a valid settings.php on the $tmpsite databases
#
#@author Guillaume Bec <guillaume.bec@gmail.com>

TMPSITE=sandfr
EXPORTFILE="/home/drupalfr/export/sandfr-$(date +%F).sql"
DRUPAL=/home/drupalfr/www/
SANSQL=/home/drupalfr/scripts/sanitize.sql
CONFDRUSH=/home/drupalfr/scripts/drushrc.php
DRUSH=/usr/bin/drush

$DRUSH sql-sync -y -r $DRUPAL -c $CONFDRUSH --structure-tables-key=sanitize --create-db default $TMPSITE
$DRUSH sql-dump -y -r $DRUPAL -c $CONFDRUSH --structure-tables-key=sanitize --create-db default $TMPSITE
$DRUSH -y -r $DRUPAL -l $TMPSITE sql-cli < $SANSQL
$DRUSH -y -r $DRUPAL -l $TMPSITE sql-dump > $EXPORTFILE
