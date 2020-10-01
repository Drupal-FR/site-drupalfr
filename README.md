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

## Documentation

Make sure to check [the project wiki on Github](https://github.com/Drupal-FR/site-drupalfr/wiki) 
to know how to install the project and all to contribute.
