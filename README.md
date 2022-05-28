# Docker Drupal project

[![pipeline status](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/badges/9.x/pipeline.svg)](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/commits/9.x)

A Drupal project template with Docker environment.

## Branches

* [9.x](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/tree/9.x): Default branch.
* [ci-tests](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/tree/ci-tests): Branch to have failing tests and ensure they detect errors.
* [ci-tests-example](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/tree/ci-tests-example): Branch to have some examples of custom config/code and tests written against it.
* [ci-contrib](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/tree/ci-contrib): To monitor projects I maintain/co-maintain on Drupal.org and be able to apply additional quality tools not present on Drupal.org.
* [ci-contrib-smile](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/tree/ci-contrib-smile): To monitor projects I maintain/co-maintain on Drupal.org sponsored by [Smile](https://www.smile.eu) and be able to apply additional quality tools not present on Drupal.org.
* [setup-entity-share](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/tree/setup-entity-share): Adaptation into a multi-site for [Entity Share](https://www.drupal.org/project/entity_share) development/testing/demonstration.

All branches are rebased against the `9.x` branch.

## Documentation

See the [docs](docs) folder for detailed documentation:

* [Requirements](docs/00-requirements/00-requirements.md)

For the developers:
* [Installation](docs/01-developer/10-installation.md)
* [PHPStorm configuration](docs/01-developer/11-phpstorm.md)
* [Git configuration](docs/01-developer/12-git.md)
* [Composer guidelines](docs/01-developer/13-composer.md)
* [Development workflow](docs/01-developer/14-development-workflow.md)
* [Daily basis commands](docs/01-developer/15-daily-basis-commands.md)
* [Front tips](docs/01-developer/16-front-tips.md)
* [Assets](docs/01-developer/17-assets.md)

For the tech leader:
* [Project initialization](docs/02-tech-lead/20-project-initialization.md)
* [Gitlab CI](docs/02-tech-lead/21-gitlab-ci.md)

Architecture:
* [Docker](docs/03-architecture/30-docker.md)
* [Tools](docs/03-architecture/31-tools.md)
* [Secrets handling](docs/03-architecture/32-secrets.md)
* [Logs handling](docs/03-architecture/33-logs.md)

Deployment:
* [Requirements](docs/04-deployment/40-requirements.md)
* [Structure](docs/04-deployment/41-structure.md)
* [Scripts](docs/04-deployment/42-scripts.md)

General points:
* [Troubleshooting](docs/05-troubleshooting.md)
* [Core update](docs/06-core-update.md)
