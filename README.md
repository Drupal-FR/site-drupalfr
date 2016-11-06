# Drupal.fr

Bienvenue sur le dépôt du site drupal.fr

Si vous avez envie de contribuer au site, n'hésitez pas à nous contacter.

La contribution ce n'est pas que du code, il y a de quoi faire pour tout le
monde :).

Merci pour votre aide !

## Requirements

* Composer

## Installation

* Execute: **scripts/init.sh**
* Adapt the following files to your configuration:
  * scripts/script-parameters.local.sh
  * www/sites/default/settings.local.php
* In the **www** directory, execute: **composer install** to get the project's Drush.
* Execute **scripts/install.sh**

### Additional steps to install with Docker compose

* Do not execute scripts/install.sh from your computer.
* Adapt the following files to your configuration:
  * docker-compose.yml
* Execute: **docker-compose up**
* In another tab, get a command-line in the container: **docker exec -it container_name_php_1 /bin/bash**
* Execute scripts/install.sh
