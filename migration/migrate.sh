#!/bin/bash
set -xe

# Needs to be called from www7.

# Clean-up the database.
drush sqlc < ../migration/sql/pre-import.sql

# Import the D6 database.
cat ../export/drupalfr-live-sanitized.sql | sed -e 's/^) ENGINE=InnoDB/) ENGINE=MyISAM/' | drush sqlc

# Fix some of the properties of the D6 database.
drush sqlc < ../migration/sql/pre-migration.sql

# Try a migration.
drush update-db -y
