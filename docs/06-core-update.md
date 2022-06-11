# Core update

When making a minor/major core update, there are some manual steps to do:
* files to check for, if there has been changes in Drupal core:

| Project's path                                 | Standard's path                              |
|------------------------------------------------|----------------------------------------------|
| conf/drupal/default/development.services.yml   | app/sites/development.services.yml           |
| conf/drupal/default/example.settings.local.php | app/sites/default/default.settings.php       |
| conf/drupal/default/example.settings.local.php | app/sites/example.settings.local.php         |
| conf/drupal/default/services.yml               | app/sites/default/default.services.yml       |
| scripts/quality/spellcheck/.cspell.json        | app/core/.cspell.json                        |
| scripts/quality/stylelint/.stylelintignore     | app/core/.stylelintignore                    |
| scripts/scaffold/private_files.htaccess        | private_files/default/.htaccess              |
| scripts/scaffold/public_files.htaccess         | app/sites/default/files/.htaccess            |
| scripts/tests/phpunit/phpunit.xml.dist         | app/core/phpunit.xml.dist                    |
| .gitattributes                                 | app/core/assets/scaffold/files/gitattributes |
