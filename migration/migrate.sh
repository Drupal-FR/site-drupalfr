#!/bin/bash
set -xe


# Synchronize files.
#rm -Rf ../www7/sites/default/files
#cp -r ../files ../www7/sites/default
#find ../www7/sites/default/files -type d -print0 | xargs -0 chmod g+rwxs
#find ../www7/sites/default/files -type f -print0 | xargs -0 chmod g+rw

# Clean-up the database.
drush sqlc < ../migration/sql/pre-import.sql

# Import the D6 database.
cat ../export/drupalfr-live-sanitized.sql | sed -e 's/^) ENGINE=InnoDB/) ENGINE=MyISAM/' | drush sqlc

# Fix some of the properties of the D6 database.
drush sqlc < ../migration/sql/pre-migration.sql

# Try a migration.
drush updatedb -y --verbose

# Disable some useless core modules
drush dis -y rdf

# enable modules
drush en -y pathologic
drush en -y drupalfr_user
drush en -y content_migrate
drush en -y markdown
drush en -y dfr_documentation
drush en -y toolbar
drush en -y token
drush en -y contextual
drush en -y image
drush en -y rdf
drush en -y shortcut
drush en -y codefilter
drush en -y entity
drush en -y diff
drush en -y mollom
drush en -y pathauto
drush en -y redirect
drush en -y strongarm
drush en -y views views_ui
drush en -y features
drush en -y email link node_reference user_reference
drush en -y flag 
drush en -y rules rules_admin
drush en -y phone
drush en -y dfr_emploi
drush en -y userpoints userpoints_rules
drush en -y transliteration
drush en -y drupalfr_edito
drush en -y twitter_block
drush en -y dfr_blocks
drush en -y drupalfr_events
drush en -y bueditor
drush en -y drupalfr_forums

drush cc all
drush updatedb -y --verbose
drush en -y dfr_format
drush cc all
drush en -y migrate
drush -y content-migrate-fields
drush cc all

drush en -y dfr_migration

# Lancer la  migration des utilisateurs
drush dfrum

# Configure BUEditor
drush vset --exact bueditor_user '1'
drush eval "print json_encode(array(13=>array('weight'=>'0','editor'=>'0','alt'=>'0'),4=>array('weight'=>'0','editor'=>'0','alt'=>'0'),11=>array('weight'=>'0','editor'=>'0','alt'=>'0'),9=>array('weight'=>'0','editor'=>'0','alt'=>'0'),12=>array('weight'=>'0','editor'=>'0','alt'=>'0'),7=>array('weight'=>'0','editor'=>'0','alt'=>'0'),2=>array('editor'=>'1','alt'=>'0','weight'=>11),1=>array('weight'=>12,'editor'=>'0','alt'=>'0'),))" | drush --exact vset --format=json bueditor_roles -

# Fix flag migration
drush sqlq "UPDATE flag_types SET type = 'comment_node_forum' WHERE type = 'forum'"

# Remove a useless redirection.
drush php-eval "redirect_delete(1);"

# Add some bits of customizations
drush vset admin_theme seven

# update site_name & site_slogan
drush vset site_name "Drupalfr.org"
drush vset site_slogan "Communauté Drupal France et francophonie"

# Performance wise options
drush sqlq "DELETE FROM system WHERE status = 0"
drush vset locale_cache_length 65535
# Don't forget to disable views_ui and dblog in production

echo "aller sur la page admin/migration et lancer la migration\n";
