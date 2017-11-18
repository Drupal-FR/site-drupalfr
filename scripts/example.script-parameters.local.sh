#!/bin/bash

# Used to know if we want to download and enable development modules.
ENVIRONMENT_MODE="dev"
# Only "dev" is searched. So any other string can prevent development modules.
ENVIRONMENT_MODE="prod"

# Used to flush Redis cache before installing the website. Adapt the following
# command and settings to your installation.
REDIS_HOST="redis"
REDIS_DB=0

REDIS_FLUSH_COMMAND=""
# If you are not using Redis. Comment the following line.
REDIS_FLUSH_COMMAND="redis-cli -h ${REDIS_HOST} -n ${REDIS_DB} FLUSHDB"

# For Drush site-install.
ACCOUNT_MAIL="admin@example.com"
ACCOUNT_NAME="admin"
ACCOUNT_PASS="admin"
SITE_MAIL="admin@example.com"
SITE_NAME="Drupalfr"
