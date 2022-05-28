# PHPStorm configuration

The following configuration has to be adapted to your setup if you have made
customizations.

Documentation done on PHPStorm 2020.3.2

## Folders indexation

Exclude the following folders from indexation:
* app/sites/default/files
* app/sites/simpletest (will exist only when you will launch PHPUnit tests)
* backups
* build (will exist only when you generating a package locally)
* data
* private_files
* www

Option 1:
* make a right click on the folder
* select `Mark Directory as` > `Excluded`

Option 2:
* in File | Settings | Directories
* mark the folders as excluded

![Directory exclusion](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_directory_exclusion.png)

## XDebug

### Configuration

In File | Settings | Languages & Frameworks | PHP | Debug

* `Break at first line in PHP scripts`: unchecked
* `Debug port`: `9000,9003`
* `Force break at first line when no path mapping specified`: unchecked
* `Force break at first line when a script is outside the project`: unchecked
* `Notify if debug session was finished without being paused`: unchecked

Optional:
* you can increase the setting `Max. simultaneous connections` if you want

![Xdebug configuration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_xdebug_configuration.png)

In File | Settings | Languages & Frameworks | PHP | Servers

The first time you will `listen` for Xdebug on a project, a popin will appear asking
you if you want to accept connection from your container. Click on `Accept`.

If you still have not listened debug on a project, you can set a server in advance.

Configure a mapping between the files on your computer and the files in the
container.

By default, the project root is mounted on `/project` in the container, so in
`Absolute path on the server` for the project root folder put `/project`.

![Server path mapping](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_php_path_mapping.png)

Note: you must also set the mapping in the `web` server to have Xdebug breakpoints
working when using CLI (i.e. Drush).

### Macro

To avoid the side effect of a breakpoint placed on a if statement, in which case
the breakpoint is not necessarily working, you can place a breakpoint on an assignment:

`$foo = 'bar';`

Therefore to ease the usage of breakpoints you can create a macro to type this
assignment for you.

* start a macro recording: Edit | Macros | Start Macro Recording
* place your cursor on an empty line and type `$foo = 'bar';`

![Macro breakpoint](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_macro_breakpoint.png)

* click on the left of your editor to add a breakpoint
* stop macro recording: Edit | Macros | Stop Macro Recording
* give the name you want to your macro: `Breakpoint`
* give a shortcut to your macro:
  * in File | Settings | Keymap, find your macro
  * make a right click on the macro name
  * select `Add Keyboard Shortcut`
  * type the keys of your shortcut: `ctrl + maj + ,`

![Macro shortcut](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_macro_shortcut.png)

## Plugins

Install the following plugins:
* Drupal Symfony Bridge
* PHP Annotations
* Symfony Support

Optional but recommended:
* .env files support
* ANSI Highlighter
* CSV Plugin
* Ideolog
* Makefile support

![Plugins](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_plugins.png)

### Drupal integration

In File | Settings | Languages & Frameworks | PHP | Frameworks

* `Enable Drupal integration`: checked
* `Drupal installation path`: select your app folder
* `Version`: select your current Drupal website major version

PHPStorm should then pop messages asking you:
* if you want to use Drupal coding standards for this project: yes
* if you want to fix file association with PHP type: yes

![Drupal integration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_drupal_integration.png)

### Symfony integration

In File | Settings | Languages & Frameworks | PHP | Symfony

* `Enable Plugin for this project (change need restart)`: checked

Optional:
* You can adapt `App Directory` and `Web Directory` to your setup.

![Symfony integration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_symfony_integration.png)

## Coding standards

In File | Settings | Editor | Code Style

* `Hard wrap at`: `80`
* `Visual guides`: `80`

![Code style general](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_code_style_general.png)

In File | Settings | Editor | Code Style | PHP

* click on the `Set from...` link on the right
* select `Drupal`

In File | Settings | Editor | Code Style | JavaScript

* click on the `Set from...` link on the right
* select `Drupal Javascript Style`

In File | Settings | Editor | Code Style | Style Sheets | CSS

* click on the `Set from...` link on the right
* select `Drupal CSS Style`

In:
* File | Settings | Editor | Code Style | Style Sheets | Less
* File | Settings | Editor | Code Style | Style Sheets | Sass
* File | Settings | Editor | Code Style | Style Sheets | SCSS

* click on the `Set from...` link on the right
* select `CSS`

In:
* File | Settings | Editor | Code Style | HTML
* File | Settings | Editor | Code Style | JSON
* File | Settings | Editor | Code Style | Twig
* File | Settings | Editor | Code Style | YAML

* click on the `Set from...` link on the right
* select `PHP`

## Tools (live inspection)

**Optional as it may consumes a lot of resources.**

There is GrumPHP, Make commands and Gitlab CI to ensure that code is ok.

### PHP CLI interpreter

If you have PHP installed on your computer, you can use it as CLI interpreter.

Or you can use PHP in a Docker container, this will ensure that you are executing
the PHP of your project. Here are the steps for that:

* in File | Settings | Languages & Frameworks | PHP
* click on `...` at the right of the `CLI Interpreter` line
* click on the `+` to add a new interpreter
* select `From Docker, Vagrant, VM, WSL, Remote...`
* select `Docker Compose`
* select a `Server`, add a new one if needed
  * `Name`: `Docker`
  * `Connect to Docker daemon with`: `Unix socket`
  * save this server
* `Service`: `web` (the service containing PHP)
* save this remote PHP interpreter
* `Lifecycle`: `Always start a new container ('docker-compose run')`. The other
  option does not work. PHPStorm will create a container named `phpstorm_helpers_PS-*`

![Remote interpreter type selection](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_cli_interpreter_1.png)
![Docker daemon](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_cli_interpreter_2.png)
![Docker remote PHP interpreter](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_cli_interpreter_3.png)
![CLI interpreter](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_cli_interpreter_4.png)

### PHPCS

In File | Settings | Languages & Frameworks | PHP | Quality Tools

* in the `PHP_CodeSniffer` section, click on `...` at the right of the `Configuration`
* click on the `+` to add a new executable
* `CLI interpreter`: select the previously configured CLI interpreter
* `PHP_CodeSniffer path`: `/project/vendor/bin/phpcs` (the path in the container)
* `Path to phpcbf`: `/project/vendor/bin/phpcbf` (the path in the container)
* save this executable
* select it

![PHPCS executable](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_tool_phpcs_1.png)
![PHPCS executable configuration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_tool_phpcs_2.png)

In File | Settings | Editor | Inspections

PHP > Quality tools > PHP_CodeSniffer validation
* `Check files with extensions`: use the list of extensions configured in [phpcs.xml.dist](../scripts/quality/phpcs/phpcs.xml.dist)
* `Coding standard`: `Custom`
* click on `...` at the right of the `Coding standard`
* select the path to the `phpcs.xml.dist` file on your computer, it is located in `scripts/quality/phpcs/phpcs.xml.dist`

![PHPCS inspection configuration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_inspections_phpcs.png)

### PHPMD

In File | Settings | Languages & Frameworks | PHP | Quality Tools

* in the `Mess Detector` section, click on `...` at the right of the `Configuration`
* click on the `+` to add a new executable
* `CLI interpreter`: select the previously configured CLI interpreter
* `PHP Mess Detector path`: `/project/vendor/bin/phpmd` (the path in the container)
* save this executable
* select it

![PHPMD executable configuration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_tool_phpmd.png)

In File | Settings | Editor | Inspections

PHP > Quality tools > PHP Mess Detector validation
* `Custom rulesets`: select the path to the `phpmd.xml.dist` file on your computer, it is located in `scripts/quality/phpmd/phpmd.xml.dist`

![PHPMD inspection configuration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_inspections_phpmd.png)

### PHPStan

In File | Settings | Languages & Frameworks | PHP | Quality Tools

* in the `PHPStan` section, click on `...` at the right of the `Configuration`
* click on the `+` to add a new executable
* `CLI interpreter`: select the previously configured CLI interpreter
* `PHPStan path`: `/project/vendor/bin/phpstan` (the path in the container)
* save this executable
* select it

![PHPStan executable configuration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_tool_phpstan.png)

In File | Settings | Editor | Inspections

PHP > Quality tools > PHPStan validation
* `Level`: `8`
* `Configuration file`: `/project/scripts/quality/phpstan/phpstan.neon.dist` (the path in the container)

![PHPStan inspection configuration](https://florent-torregrosa.fr/sites/default/files/2021-03/phpstorm_inspections_phpstan.png)
