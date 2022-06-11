# Installation

## Init

Execute:
```bash
make project-init-dev
```

## Adapt files to your configuration

### conf/env/composer.env

You may define your custom `COMPOSER_AUTH` token if none had been versioned
during project initialization.

## Release the Kraken

Execute:
```bash
make docker-upd
make docker-site-install SELECTED_SITE=all
```

## Access the website(s)

You will have:
* the website accessible through Apache:
  * https://web-ddp9.docker.localhost
  * https://en-web-ddp9.docker.localhost
  * https://fr-web-ddp9.docker.localhost
* the website accessible through Varnish:
  * https://varnish-ddp9.docker.localhost
  * https://en-varnish-ddp9.docker.localhost
  * https://fr-varnish-ddp9.docker.localhost
* a mail catcher: https://mail-ddp9.docker.localhost
