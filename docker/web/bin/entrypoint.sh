#!/bin/bash
set -e

eval $(/usr/local/bin/fixuid)

if [ ! "true" == "$XDEBUG_ENABLED" ]; then
    # Disable Xdebug.
    sudo rm -f /usr/local/etc/php/conf.d/docker-php-ext-xdebug.ini
else
    # Enable Xdebug.
    sudo ln -sf /usr/local/etc/php/conf.d/docker-php-ext-xdebug.disabled /usr/local/etc/php/conf.d/docker-php-ext-xdebug.ini
fi



# Call the parent image CMD script.



# first arg is `-f` or `--some-option`
if [ "${1#-}" != "$1" ]; then
	set -- /usr/local/bin/apache2-foreground "$@"
fi

exec "$@"
