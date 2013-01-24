# Dossier de travail
DIR="/home/drupalfr/scripts"

#############
# Version 7
#############
# Telechargement Drupal
/usr/bin/drush dl drupal-7 -q --drupal-project-rename=drupal7 --destination=$DIR
# Detection de version
VERSION=`drush status drupal-version --pipe -r $DIR/drupal7`

# Recuperation + copie de la trad de la version
wget http://ftp.drupal.org/files/translations/7.x/drupal/drupal-$VERSION.fr.po --output-document=$DIR/drupal-$VERSION.fr.po --quiet
cp $DIR/drupal-$VERSION.fr.po $DIR/drupal7/profiles/minimal/translations/
cp $DIR/drupal-$VERSION.fr.po $DIR/drupal7/profiles/standard/translations/
rm $DIR/drupal-$VERSION.fr.po
mv $DIR/drupal7 $DIR/drupal-$VERSION

# Tarball
tar cpzf drupal-7.latest.tar.gz --directory $DIR drupal-$VERSION
rm -r $DIR/drupal-$VERSION

# Stockage du numéro de version dans une variable pour affichage sur le site.
cd ../www7
drush vset drupalfr_version_d7 VERSION
cd ../

#############
# Version 6
#############
# Telechargement Drupal
/usr/bin/drush dl drupal-6 -q --drupal-project-rename=drupal6 --destination=$DIR
# Detection de version
VERSION=`drush status drupal-version --pipe -r $DIR/drupal6`
# Recuperation + copie de la trad de la version
wget http://ftp.drupal.org/files/translations/6.x/drupal/drupal-$VERSION.fr.po --output-document=$DIR/drupal-$VERSION.fr.po --quiet
mkdir $DIR/drupal6/profiles/default/translations
cp $DIR/drupal-$VERSION.fr.po $DIR/drupal6/profiles/default/translations/fr.po
rm $DIR/drupal-$VERSION.fr.po
mv $DIR/drupal6 $DIR/drupal-$VERSION

# Tarball
tar cpzf drupal-6.latest.tar.gz --directory $DIR drupal-$VERSION
rm -r $DIR/drupal-$VERSION

# Stockage du numéro de version dans une variable pour affichage sur le site.
cd ../www7
drush vset drupalfr_version_d6 VERSION
cd ../

exit
