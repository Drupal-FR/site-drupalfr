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

# Enable modules.
drush en -y pathologic
drush en -y drupalfr_user
drush en -y content_migrate
drush en -y markdown
drush en -y dfr_documentation
drush en -y toolbar
drush en -y token
drush en -y contextual
drush en -y image
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
drush en -y drupalfr_general

# Revert the feature to remove the DB stored version of the view and use the new one from the export. 
drush fr -y drupalfr_forums
drush fr -y drupalfr_general

# Remove the fantom link about user listing.
drush php-eval '_menu_delete_item(menu_link_load(1209), 1);'

drush cc all
drush updatedb -y --verbose
drush en -y dfr_format
drush cc all
drush en -y migrate
drush -y content-migrate-fields
drush cc all

drush en -y dfr_migration

# Migrate user profiles.
drush dfrum

###
### The site is now in D7.
###

# Disable useless modules and delete unused views.
drush dis -y dfr_migration content_migrate aggregator rdf
drush php-eval '$view = views_ui_cache_load("liste_user"); $view->delete();'
drush php-eval '$view = views_ui_cache_load("user_quota"); $view->delete();'
drush php-eval '$view = views_ui_cache_load("drupal_news"); $view->delete();'
drush php-eval '$view = views_ui_cache_load("content_admin"); $view->delete();'
drush php-eval '$view = views_ui_cache_load("admin_content"); $view->delete();'
drush php-eval '$view = views_ui_cache_load("Moderation_commentaires"); $view->delete();'
drush php-eval '$view = views_ui_cache_load("Documentation"); $view->delete();'
drush php-eval '$view = views_ui_cache_load("Comment_admin"); $view->delete();'

# Configure BUEditor.
drush vset --exact bueditor_user '1'
drush eval "print json_encode(array(13=>array('weight'=>'0','editor'=>'0','alt'=>'0'),4=>array('weight'=>'0','editor'=>'0','alt'=>'0'),11=>array('weight'=>'0','editor'=>'0','alt'=>'0'),9=>array('weight'=>'0','editor'=>'0','alt'=>'0'),12=>array('weight'=>'0','editor'=>'0','alt'=>'0'),7=>array('weight'=>'0','editor'=>'0','alt'=>'0'),2=>array('editor'=>'1','alt'=>'0','weight'=>11),1=>array('weight'=>12,'editor'=>'0','alt'=>'0'),))" | drush --exact vset --format=json bueditor_roles -

# Fix flag migration.
drush sqlq "UPDATE flag_types SET type = 'comment_node_forum' WHERE type = 'forum'"

# Remove a useless redirection.
drush php-eval "redirect_delete(1);"

# Update the homepage node content.
drush php-script ../migration/update_content.php

# Add some bits of customizations.
drush vset admin_theme seven
drush php-eval "theme_enable(array('dfrtheme'));"
drush vset theme_default dfrtheme
drush php-eval "theme_disable(array('garland'));"

# Update site_name & site_slogan.
drush vset site_name "Drupalfr.org"
drush vset site_slogan "Communauté Drupal France et francophonie"

# Performance wise options.
drush sqlq "DELETE FROM system WHERE status = 0"
drush vset locale_cache_length 65535

# Disable UI modules.
drush dis -y rules_admin views_ui

# Trigger whatever cron has to do.
drush cron

# TODO: disable dblog
