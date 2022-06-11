# Deployment scripts

Use the Makefile commands to do your deployments. The commands will trigger the
appropriate scripts.

## Code deployment

Deploying a branch:
```bash
make generate-package TARGET_ENVIRONMENT=staging VERSION_TYPE=branch TARGET_VERSION=9.x
make deploy-package TARGET_ENVIRONMENT=staging PACKAGE_NAME=9.x-2021-04-23-10h07m23s
```

Deploying a tag:
```bash
make generate-package TARGET_ENVIRONMENT=prod VERSION_TYPE=tag TARGET_VERSION=2.1.5
make deploy-package TARGET_ENVIRONMENT=prod PACKAGE_NAME=2.1.5
```

## Sites update

If a site already exists and you want to update it you can execute:
```bash
make enable-release TARGET_ENVIRONMENT=staging PACKAGE_NAME=9.x-2021-04-23-10h07m23s SELECTED_SITE=site1
```

Note: a backup is done automatically.

## Sites installation

If it is the first time the site is installed on the target environment you need
to make the symlink:
```bash
make make-symlink TARGET_ENVIRONMENT=prod PACKAGE_NAME=9.x-2021-04-23-10h07m23s SELECTED_SITE=site1
```

When the symlink is present you can install the new website:
```bash
make site-install TARGET_ENVIRONMENT=prod SELECTED_SITE=site1
```

Note: no backup is done automatically (whereas it is for the dev environment).

## Sites maintenance

You can manually make a backup:
```bash
make make-backup TARGET_ENVIRONMENT=prod SELECTED_SITE=all
```

And don't forget to clear the backups (and the releases) sometimes:
```bash
make clear-backup TARGET_ENVIRONMENT=prod SELECTED_SITE=all
```

You can use the following command to restore a dump and launch the updates:
```bash
make restore-backup TARGET_ENVIRONMENT=prod SELECTED_SITE=site1
```

## Global notes

* when a script has a `SELECTED_SITE` parameter, you can put `all` to handle
  every websites of the target environment. The list of websites is in the .env
  file of this environment.
