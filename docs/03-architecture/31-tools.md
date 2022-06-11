# Tools

This project is using some tools for code quality and testing.

Also it is recommended to use the provided Makefile commands to see what is
provided.

If you don't want to use a tool, simply comment the related lines in
`.gitlab-ci.yml` and `Makefile` files.

**Important notes**: Do not use the tools blindly, its are here to help but
their configuration may needs adjustment and its can be boggy.

## Code quality

* [Shellcheck](https://www.shellcheck.net)
* [PHP Code Sniffer](https://github.com/squizlabs/PHP_CodeSniffer): with Drupal standards and additional rules, see the [configuration file](../../scripts/quality/phpcs/phpcs.xml.dist).
* [PHP Mess Detector](https://phpmd.org): with a default configuration slightly softened for Drupal, see the [configuration file](../../scripts/quality/phpmd/phpmd.xml.dist).
* [PHP Copy/Paste Detector](https://github.com/sebastianbergmann/phpcpd)
* [PHP Magic Number Detector](https://github.com/povils/phpmnd)
* [PHP Static Analysis Tool](https://github.com/phpstan/phpstan): with the [phpstan-drupal](https://github.com/mglaman/phpstan-drupal) configuration slightly overridden, see the [configuration file](../../scripts/quality/phpstan/phpstan.neon.dist).
* [Psalm](https://psalm.dev/): see the [configuration file](../../scripts/quality/psalm/psalm.xml), used for security analysis, based on https://mortenson.coffee/blog/drupal-security-testing-everyone.
* Composer validate: validate your composer.json and a custom script to run it against composer.json files inside custom code.
* [Composer Normalize](https://github.com/ergebnis/composer-normalize): ensure your composer.json follows the [Composer JSON schema](https://getcomposer.org/schema.json) and a custom script to run it against composer.json files inside custom code.
* [Rector](https://github.com/rectorphp/rector): with the [Drupal Rector](https://github.com/palantirnet/drupal-rector) configuration, see the [configuration file](../../scripts/quality/rector/rector.php).
* [PHP Security Checker](https://github.com/fabpot/local-php-security-checker)
* [GrumPHP](https://github.com/phpro/grumphp): add Git hooks to run some tools before committing. See the [configuration file](../../scripts/quality/grumphp/grumphp.yml).
* [Cspell](https://github.com/streetsidesoftware/cspell/tree/master/packages/cspell): validate sentences with Drupal standards.
* [ESLint](https://eslint.org/): lint JS files with Drupal standards. The .eslintignore file is located in the `app` folder, generated with Drupal scaffold.
* [Stylelint](https://stylelint.io/): lint CSS files with Drupal standards. Also used to lint SASS files with the following [configuration](../../scripts/quality/sasslint/.stylelintrc.json).
* [YAML Lint](https://github.com/grasmash/yaml-cli): ensure your YAML files are valid and a custom script to run it.

The following code quality tools require an installed website:
* [Config Inspector](https://www.drupal.org/project/config_inspector)
* [Upgrade Status](https://www.drupal.org/project/upgrade_status)

## Code quality fixers

* [Composer Normalize](https://github.com/ergebnis/composer-normalize): ensure your composer.json follows the [Composer JSON schema](https://getcomposer.org/schema.json) and a custom script to run it against composer.json files inside custom code.
* [Rector](https://github.com/rectorphp/rector): see the [configuration file](../../scripts/quality/rector/rector.php).
* [PHP Coding Standards Fixer](https://github.com/FriendsOfPHP/PHP-CS-Fixer): with the [PHP-Cs-Fixer Drupal Configurations](https://github.com/drupol/phpcsfixer-configs-drupal), see the [configuration file](../../scripts/quality/phpcs_fixer/.php-cs-fixer.dist.php).
* [Psalm](https://psalm.dev/): see the [configuration file](../../scripts/quality/psalm/psalm.xml).
* [ESLint](https://eslint.org/): a command is available in the Makefile.
* [Stylelint](https://stylelint.io/): a command is available in the Makefile. another command is also available for SASS files.

## Testing

* [PHPUnit](https://phpunit.de): tests in sub-modules are not detected automatically, see and adapt the [configuration file](../../scripts/tests/phpunit/phpunit.xml.dist).
* [Behat](https://behat-drupal-extension.readthedocs.io/): see and adapt the [configuration file](../../scripts/tests/behat/behat.yml).
* [Pa11y](https://pa11y.org/): Accessibility tests. Online documentation for [Pa11y](https://github.com/pa11y/pa11y#configuration) and [Pa11y CI](https://github.com/pa11y/pa11y-ci). See and adapt the [configuration file for development environment](../../scripts/tests/pa11y/pa11y-dev.json) and [configuration file for Gitlab CI environment](../../scripts/tests/pa11y/pa11y-gitlab.json).
* [Cypress](https://docs.cypress.io): End to End tests. See and adapt the [configuration file](../../scripts/tests/cypress/cypress.json). Currently, no Gitlab CI integration, see [this issue](https://gitlab.com/florenttorregrosa-drupal/docker-drupal-project/-/issues/104).
