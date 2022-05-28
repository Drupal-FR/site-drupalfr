# Troubleshooting

## File permissions

If you have a file permission issue, execute:

```bash
make docker-fix-permissions
```

Warning: use this script only if you are on a dev environment and execute it as
root. If you are using it inside the Docker container you are already root.

## Solr configuration change not taken into account

Solr configuration is taken into account only when the Solr container is **created**.

Because the entrypoint script copies the provided configuration into the Solr
core when creating it initially.

So to update your Solr configuration, you need to update the files in `conf/solr`
and then delete your solr container and recreate it.

## Elasticsearch containers stopping

Create a file `/etc/sysctl.d/elasticsearch.conf` with the following content:

```
# This is needed for running Elasticsearch in containers.
vm.max_map_count=262144
```

Then restart Docker.

## Xdebug breakpoint not taken into account (while it was working)

Try either or both:
* in `.env`: change `CUSTOM_PHP_OPCACHE_REVALIDATE_FREQ` and set it to 5 per
example and restart your environment.
* in `docker-compose.yml`: remove the `:delegated` of some volumes like
`- .:/project:delegated`

## Xdebug not working when using Drush

You must have mapped like in this [documentation file](./01-developer/11-phpstorm.md#configuration)
for the `web` server.

Also you should use `./vendor/bin/drush` and not `drush` directly, because `drush`
is the Drush launcher which [disables Xdebug by default](https://github.com/drush-ops/drush-launcher#xdebug-compatibility).

## Mysql dump not working

Sometimes the drush sql:dump command fails depending on the Mysql/MariaDB/Percona
version/configuration.

In this case you may uncomment the line `extra-dump` in the
[Drush config file](../drush/drush.yml).

## macOS: bad substitution

### Upgrade bash

The default bash version is 3.x

```bash
$ bash --version
GNU bash, version 3.2.57(1)-release (x86_64-apple-darwin18)
Copyright (C) 2007 Free Software Foundation, Inc.
```

To install the newest version available, use `brew`

```bash
brew install bash
```

It should be installed here: `/usr/local/bin/bash`

```bash
$ which -a bash
/usr/local/bin/bash
/bin/bash
```

Add it in the allowed shells

```bash
$ sudo vim /etc/shells
```

Set it as your default shell

```bash
$ chsh -s /usr/local/bin/bash
```
