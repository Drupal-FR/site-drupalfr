#!/bin/bash

# Clean-up the database.
drush sqlc < sql/pre-import.sql

# Import the D6 database.
drush sqlc < ../export/drupalfr-live-sanitized.sql

# Fix some of the properties of the D6 database.
drush sqlc < sql/pre-migration.sql

# Try a migration.
cd ../www7
drush update-db -y
