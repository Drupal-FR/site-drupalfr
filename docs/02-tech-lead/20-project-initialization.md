# Project initialization

## Adapt files to your project

### example.*.env

Adapt `COMPOSE_FILE` with the tools you want to use. The available tools are in
the `docker` folder:
* devtools.yml
* elasticsearch.yml
* maildev.yml
* mysql.yml
* redis.yml
* solr.yml
* varnish.yml

Note: `docker-compose.yml` at the project root is mandatory.

Also change the other variables for your need.

### conf/env/example.composer.env

You may define your custom `COMPOSER_AUTH` token and commit it, so it avoid
everyone in the project to have to define their own.

Also needed in `.gitlab-ci.yml`, in the `variables` you have to add
`COMPOSER_AUTH` if you are using your own Gitlab instance.

### scripts/script-parameters.sh

You may want to update some variables to optimize your package, for example to
remove SCSS files after compilation had been done.

### .gitlab-ci.yml

Depending on your own Gitlab CI, you may need to add a tag on tasks to ensure
it is executed on a Gitlab runner with Docker.

When your environments are ready, you can uncomment the package and deploy tasks.

Adapt the branch names to your project.

### New sites

If you are in a multi-sites architecture, you will need to update the following
files/folders:
* conf/drupal/example.sites.php
* drush/sites

## Init configuration

This step assumes that you are using core configuration management on your
project.

### Installation
Execute:
```bash
make project-init-dev
make docker-up
make docker-site-install SELECTED_SITE=all
```

### Configuration

* enable the modules you need
* configure Config Split
  * a `dev` split with the development modules in the "blacklist"
  * an `overrides` split that will be used for config overridable by the client
    using the "greylist".
  * in `conf/drupal/default/sync`, there are sample config files that you can
    copy to re-inject later.
* configure Config Ignore
  * in `conf/drupal/default/sync`, there are sample config files that you can
    copy to re-inject later.
* configure Purge modules
  * in `conf/drupal/default/sync`, there are sample config files that you can
    copy to re-inject later.
* export configuration: `make docker-shell-web-cmd "drush @default.alias cex -y`

### Update files

In `scripts/install-dev.sh` you can comment the line that enables the
development modules: `"${SCRIPTS_PATH}"/tasks/enable_development_modules.sh`

In `scripts/tasks/export_config_split_overrides.sh`, uncomment the lines.

In `scripts/update.sh`, edit the lines about the `Overrides` Config Split:
```bash
    echo -e "${COLOR_BROWN_ORANGE}Enable or remove config split overrides export.${COLOR_NC}"
#    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Re-export overrides split to ensure exported overrides are up-to-date.${COLOR_NC}"
#    $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${WEBSERVER_USER} $DRUSH ${!DRUSH_ALIAS} config-split:export overrides -y"
```

becomes:
```bash
    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Re-export overrides split to ensure exported overrides are up-to-date.${COLOR_NC}"
    $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${WEBSERVER_USER} $DRUSH ${!DRUSH_ALIAS} config-split:export overrides -y"
```

Note: there are 2 places to edit in this file.

In `example.*.env`, edit the `DRUPAL_SITE_*_HAS_EXPORTED_CONFIG` variables to
`true`.

### Switch to minimal profile

In the case you have used the "standard" profile to init your configuration, you
will need to switch to an installation profile without hook_install. Installing
from existing configuration with an installation profile with a hook_install is
not supported yet.

1. Edit the `DRUPAL_SITE_*_INSTALLATION_PROFILE` variables in the example.*.env
   files.
2. Edit the `conf/drupal/default/sync/core.extension.yml` file:
    ```yaml
       module:
         ...
         minimal: 1000
       ...
       profile: minimal
    ```
3. Reinstall: `make docker-site-install SELECTED_SITE=all`
