# Composer guidelines

## Specific version of packages

**Use a specific version for Drupal packages in the "require" section.**

No need to use a specific version in the "require-dev" section except for
`drupal/core-dev` to match the core version in "require".

This way it allows to execute `composer update` without the risk to update core
or modules in an unwanted moment. If you need to regenerate the composer.lock
file because of conflicts or other stuff for example. Also it allows to avoid
some side effects with Composer and stability flags.

## List explicitly all Drupal packages

**List explicitly all Drupal packages.**

It avoids:
* to remove an installed module from the source
* unwanted update of dependencies

Example: you require the `drupal/search_api_solr` package, `drupal/search_api`
is automatically downloaded. One day Search API Solr is uninstalled and removed.
Search API is also removed but it has been forgotten to uninstall it.
