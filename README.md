# Drupal.fr

Bienvenue sur le dépôt du site drupal.fr

Si vous avez envie de contribuer au site, n'hésitez pas à nous contacter.

La contribution ce n'est pas que du code, il y a de quoi faire pour tout le
monde :).

Merci pour votre aide !

## Requirements

To use with Docker:
* Docker
* Docker compose

To use outside of Docker:
* Composer

Scripts inside the scripts folder don't have dependencies on Docker. So its
could be executed outside of a Docker environment.

Some adjustment in configuration files may be required.

## Services

If using Docker with the default provided configuration, you will have:
* the website accessible through Apache: http://web.drupalfr8.docker.localhost or http://127.0.0.1:8101
* the website accessible through Varnish: http://varnish.drupalfr8.docker.localhost or http://127.0.0.1:8100
* a mail catcher: http://mail.drupalfr8.docker.localhost or http://127.0.0.1:8105

Note: You have to adapt your DNS configuration to inform your computer to search
for local websites.

For example, you can edit your /etc/hosts file and add an entry like:
```
127.0.0.1  web.drupalfr8.docker.localhost varnish.drupalfr8.docker.localhost mail.drupalfr8.docker.localhost
```

## Installation

* Execute: `scripts/init.sh`
* Adapt the following files to your configuration:
  * scripts/script-parameters.local.sh
  * conf/drupal/default/settings.local.php
* Execute `scripts/install.sh` (do not if using docker, see below)

### Additional steps to install with Docker compose

* **Do not execute scripts/install.sh from your computer.**.
* Adapt the following files to your configuration:
  * .env
  * docker-compose.yml
* Execute: 

```bash
docker-compose pull
docker-compose up
```

* In another tab, get a command-line in the container:

```bash
docker-compose exec web /bin/bash
```

* Execute:

```bash
cd ..
./scripts/install.sh
```

#### Traefik integration

If there's a local traefik reverse-proxy on your development environment, you
can access the site through http://drupalfr.docker/

Alternative hostnames can be provided by setting environment variables, for
instance using a .env file. See the example.env file for the available
variables.

A Traefik configuration and docker-compose.yml are in the `conf/traefik` folder
to ease the usage.

To start Traefik:

```bash
cd conf/traefik
docker-compose pull
docker-compose up
```

### Notes about configuration

We have switched from Features to Config split, Config ignore, Config installer.

**Warning:** when using `drush config:export` to export changes, the config
ignore settings is ignored. So please be cautious and review the changes before
committing.

### Q/A
#### How to find out the container names?

You can use the command `docker ps` which list all the running docker
containers.

![docker PS](http://i.imgur.com/SDgHsqs.png)

#### How to use drush within docker?

You have to "be" in the docroot folder (eg: `/project/www`) or the project
folder (eg: `/project`)

Optional: you can use Drush within the web container by using the alias
`@docker.default`:

```bash
drush @docker.default status
```

#### How to import a custom dump?

Put the dump in the `backups` folder and then in the **web** container you can
use the following command:

```bash
zcat /project/backups/DUMP_NAME.sql.gz | mysql -u drupal -pdrupal -h mysql drupal
```

Or using the following commands:

```bash
cd /project/backups
tar -xvzf /project/backups/DUMP_NAME.sql.gz
cd ..
drush sql-cli < /project/backups/DUMP_NAME.sql
```
