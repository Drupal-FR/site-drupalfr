# Daily commands

In addition to the previously seen commands.

## File permissions

If you have a file permission issue, execute:
```bash
make docker-fix-permissions
```

After exporting configuration for example.

## Assets compilation

```bash
make docker-compile-assets
```

## Connect to the web container

```bash
make docker-shell-web
```

## Clear Drupal caches

```bash
make docker-drush "@default.alias cr"
```
or
```bash
make docker-shell-web-cmd "drush @default.alias cr"
```

## Import a dump

```bash
make docker-shell-web
tar -xvzf /project/backups/DUMP_NAME.sql.gz
drush @default.alias sql:drop
drush @default.alias sql:cli < /project/backups/DUMP_NAME.sql
```

## General note

Inside the `web` container you can launch scripts directly if you want, but it
is preferred to use the `Makefile` to execute scripts.
