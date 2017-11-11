# Drupal.fr

Bienvenue sur le dépôt du site drupal.fr

Si vous avez envie de contribuer au site, n'hésitez pas à nous contacter.

La contribution ce n'est pas que du code, il y a de quoi faire pour tout le
monde :).

Merci pour votre aide !

## Requirements

* Composer

## Installation

* Execute: `scripts/init.sh`
* Adapt the following files to your configuration:
  * scripts/script-parameters.local.sh
  * www/sites/default/settings.local.php
* Execute `scripts/install.sh` (do not if using docker, see below)

### Additional steps to install with Docker compose

* **Do not execute scripts/install.sh from your computer.**.
* Adapt the following files to your configuration:
  * docker-compose.yml
* Execute: `docker-compose up`
* In another tab, get a command-line in the container:
`docker exec -it container_name_web_1 /bin/bash`
 (*container_name* should be remplaced by the name of the **web** container)
* Execute `scripts/install.sh`

The website should be located at this address : http://127.0.0.1:8101/*

### Notes about configuration

We have switched from Features to Config split, Config ignore, Config installer.

Warning: when using drush config:export to export changes, the config ignore
settings is ignored. So please be cautious and review the changes before
committing.

### Q&A
#### How to find out the container names ?
You can use the command `docker ps` which list all the running docker containers.

![docker PS](http://i.imgur.com/SDgHsqs.png)

#### How to use drush within docker ?
You can use docker within the web container by using the alias `@docker.default.local`:

```
drush @docker.default.local status
```

Note : you have to "be" in the docroot folder (eg : `/project/www`)

#### How to import a custom dump

Put the dump in the `backups` folder and then in the **web** container you can use the following command:
```
zcat /project/backups/DUMP_NAME.sql.gz | mysql -u drupal -pdrupal -h mysql drupal
```
