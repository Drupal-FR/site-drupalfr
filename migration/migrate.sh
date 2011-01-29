#!/bin/bash
set -e

# Needs to be called from www7.

# Clean-up the database.
drush sqlc < ../migration/sql/pre-import.sql

# Import the D6 database.
drush sqlc < ../export/drupalfr-live-sanitized.sql

# Fix some of the properties of the D6 database.
drush sqlc < ../migration/sql/pre-migration.sql

# Try a migration.
drush update-db -y
