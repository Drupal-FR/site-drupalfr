#!/bin/bash

currentscriptpath ()
{
local fullpath=`echo "$(readlink -f $0)"`
local fullpath_length=`echo ${#fullpath}`
local scriptname="$(basename $0)"
local scriptname_length=`echo ${#scriptname}`
local result_length=`echo $fullpath_length - $scriptname_length - 1 | bc`
local result=`echo $fullpath | head -c $result_length`
echo $result
}

# Dossier de travail
RESULT=$(currentscriptpath)
DIR="$RESULT/.."
#DRUSH="drush"
DRUSH="/usr/local/bin/drush"

#############
# Version 8
#############
# Telechargement Drupal
$DRUSH dl drupal-8 -q -y --drupal-project-rename=drupal8 --destination=$DIR

# Detection de version
VERSION=`cat $DIR/drupal8/core/lib/Drupal.php | grep "const VERSION = " | tail -n 1`
# Example of $VERSION: "const VERSION = '8.0.3';"
stringlen=$(( ${#VERSION} - 19 - 2 ))
VERSION=${VERSION:19:$stringlen}

# Stockage du numéro de version dans une variable pour affichage sur le site.
$DRUSH -r $DIR/www7 vset drupalfr_current_version -q --yes --exact --format="string" "$VERSION"

rm -rf $DIR/drupal8

##############
## Version 7
##############
# Telechargement Drupal
$DRUSH dl drupal-7 -q -y --drupal-project-rename=drupal7 --destination=$DIR

# Detection de version
VERSION=`$DRUSH core-status drupal-version --format=list -r $DIR/drupal7`

# Recuperation + copie de la trad de la version
wget http://ftp.drupal.org/files/translations/7.x/drupal/drupal-$VERSION.fr.po --output-document=$DIR/drupal-$VERSION.fr.po --quiet
cp $DIR/drupal-$VERSION.fr.po $DIR/drupal7/profiles/minimal/translations/
cp $DIR/drupal-$VERSION.fr.po $DIR/drupal7/profiles/standard/translations/
rm $DIR/drupal-$VERSION.fr.po
mv $DIR/drupal7 $DIR/drupal-$VERSION

# Tarball
tar cpzf $DIR/scripts/drupal-7.latest.tar.gz --directory $DIR drupal-$VERSION
rm -r $DIR/drupal-$VERSION

# Stockage du numéro de version dans une variable pour affichage sur le site.
$DRUSH -r $DIR/www7 vset drupalfr_previous_version -q --yes --exact --format="string" "$VERSION"

exit
