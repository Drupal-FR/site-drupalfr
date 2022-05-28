include .env

default: help

.PHONY: help
help:  ## Display this help.
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m<target>\033[0m\n\nTargets:\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  \033[36m%-10s\033[0m %s\n", $$1, $$2 }' $(MAKEFILE_LIST)

.PHONY: project-init-dev
project-init-dev: ## Launch init script for development environment.
	@echo "$(COLOR_LIGHT_GREEN)Launch init.sh script for dev environment...$(COLOR_NC)"
	./scripts/init.sh -e dev

.PHONY: project-init-prod
project-init-prod: ## Launch init script for production environment.
	@echo "$(COLOR_LIGHT_GREEN)Launch init.sh script for prod environment...$(COLOR_NC)"
	./scripts/init.sh -e prod

################################################################################
## Docker.
################################################################################

.PHONY: docker-pull
docker-pull: ## Update container images.
	@echo "$(COLOR_LIGHT_GREEN)Update containers images for $(COMPOSE_PROJECT_NAME)...$(COLOR_NC)"
	@docker-compose pull

.PHONY: docker-up
docker-up: ## Start containers.
	@echo "$(COLOR_LIGHT_GREEN)Starting up containers for $(COMPOSE_PROJECT_NAME)...$(COLOR_NC)"
	@docker-compose up --remove-orphans

.PHONY: docker-upd
docker-upd: ## Start containers in detached mode.
	@echo "$(COLOR_LIGHT_GREEN)Starting up containers for $(COMPOSE_PROJECT_NAME)...$(COLOR_NC)"
	@docker-compose up -d --remove-orphans

.PHONY: docker-stop
docker-stop: ## Stop containers.
	@echo "$(COLOR_LIGHT_GREEN)Stopping containers for $(COMPOSE_PROJECT_NAME)...$(COLOR_NC)"
	@docker-compose stop

.PHONY: docker-down
docker-down: ## Remove containers.
	@echo "$(COLOR_LIGHT_GREEN)Removing containers for $(COMPOSE_PROJECT_NAME)...$(COLOR_NC)"
	@docker-compose down

.PHONY: docker-prune
docker-prune: ## Remove containers, volumes and images.
	@echo "$(COLOR_LIGHT_GREEN)Removing containers, volumes and images for $(COMPOSE_PROJECT_NAME)...$(COLOR_NC)"
	@docker-compose down -v --rmi all

.PHONY: docker-shell-web
docker-shell-web: ## Open a command line in the web container.
	@docker-compose exec -e LINES=$(tput lines) -e COLUMNS=$(tput cols) web bash

.PHONY: docker-shell-web-cmd
docker-shell-web-cmd: ## Execute the entered command. Example: make docker-shell-web-cmd "ls -l"
	@docker-compose exec -T web bash -c "$(filter-out $@,$(MAKECMDGOALS))"

.PHONY: docker-drush
docker-drush: ## Execute the entered Drush command. Example: make docker-drush cr
	@docker-compose exec web bash -c "$(DRUSH) $(filter-out $@,$(MAKECMDGOALS))"

.PHONY: docker-site-install
docker-site-install: ## Install Drupal website. Example: make docker-site-install SELECTED_SITE=all
	${MAKE} docker-disable-crontab
	@docker-compose exec web bash -c "cd $(PROJECT_PATH) && ./scripts/install-dev.sh ${SELECTED_SITE}"
	${MAKE} docker-fix-permissions
	${MAKE} docker-enable-crontab

.PHONY: docker-site-update
docker-site-update: ## Update Drupal website. Example: make docker-site-update SELECTED_SITE=all
	${MAKE} docker-disable-crontab
	@docker-compose exec -T web bash -c "cd $(PROJECT_PATH) && ./scripts/update-dev.sh ${SELECTED_SITE}"
	${MAKE} docker-fix-permissions
	${MAKE} docker-enable-crontab

.PHONY: docker-contrib-update
docker-contrib-update: ## Update contrib. Example: make docker-contrib-update SELECTED_SITE=all
	${MAKE} docker-disable-crontab
	@docker-compose exec web bash -c "cd $(PROJECT_PATH) && ./scripts/contrib-update.sh ${SELECTED_SITE}"
	${MAKE} docker-fix-permissions
	${MAKE} docker-enable-crontab

.PHONY: docker-enable-crontab
docker-enable-crontab: ## Reload the crontab config file.
	@docker-compose exec -T drupal_cron bash -c "touch $(DRUPAL_CRON_LOG_FILE_PATH)"
	@docker-compose exec -T drupal_cron bash -c "chown $(WEBSERVER_USER):$(WEBSERVER_GROUP) $(DRUPAL_CRON_LOG_FILE_PATH)"
	@docker-compose exec -T drupal_cron bash -c "chmod 0644 $(DRUPAL_CRON_LOG_FILE_PATH)"
	@docker-compose exec -T drupal_cron bash -c "crontab -u root /etc/crontabs/drupal_cron"

.PHONY: docker-disable-crontab
docker-disable-crontab: ## Disable the crontab.
	@docker-compose exec -T drupal_cron bash -c "crontab -u root /etc/crontabs/empty_cron"

.PHONY: docker-compile-assets
docker-compile-assets: ## Compile assets.
	@echo "$(COLOR_LIGHT_RED)If you are adding/compiling new files, remember to execute 'make docker-drupal-paranoia'.$(COLOR_NC)"
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${PROJECT_PATH}/scripts/assets install && \
		yarn --cwd ${PROJECT_PATH}/scripts/assets run gulp-dev'

.PHONY: docker-compile-assets-production
docker-compile-assets-production: ## Compile assets for production.
	@echo "$(COLOR_LIGHT_RED)If you are adding/compiling new files, remember to execute 'make docker-drupal-paranoia'.$(COLOR_NC)"
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${PROJECT_PATH}/scripts/assets install && \
		yarn --cwd ${PROJECT_PATH}/scripts/assets run gulp-prod'

.PHONY: docker-drupal-paranoia
docker-drupal-paranoia: ## Execute composer drupal:paranoia.
	${MAKE} docker-shell-web-cmd 'cd $(PROJECT_PATH) && composer drupal:paranoia'

.PHONY: docker-fix-permissions
docker-fix-permissions: ## Fix directories permissions.
	@docker-compose exec -T web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/fix-permissions-dev.sh all "$(shell id -u)" "$(shell id -g)"'

################################################################################
## Deployment and remote commands.
################################################################################

.PHONY: generate-package
generate-package: ## Generate a package. Example: make generate-package TARGET_ENVIRONMENT=prod VERSION_TYPE=branch TARGET_VERSION=9.x
	@docker-compose exec web bash -c "cd ${PROJECT_PATH} && rm -rf ./build"
	mkdir -p ./build/${TARGET_VERSION}

	# Do git command on host machine for SSH keys access.
	cd /tmp && git archive --remote=${GIT_REPOSITORY_URL} -o ${TARGET_VERSION}.tar.gz ${TARGET_VERSION}
	tar -xf /tmp/${TARGET_VERSION}.tar.gz -C ./build/${TARGET_VERSION}

	# Launch Composer and Yarn in Makefile because it requires different containers. The rest of the packaging can then be handled in a script.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/package-composer-install.sh ${TARGET_ENVIRONMENT} ${TARGET_VERSION}'
	@docker-compose exec node /bin/sh -c 'cd ${PROJECT_PATH} && \
		yarn --cwd ${PROJECT_PATH}/build/${TARGET_VERSION}/scripts/assets install && \
		yarn --cwd ${PROJECT_PATH}/build/${TARGET_VERSION}/scripts/assets run gulp-prod'
	@docker-compose exec web bash -c 'composer drupal:paranoia --working-dir="${PROJECT_PATH}/build/${TARGET_VERSION}"'

	# Fix permissions otherwise impossible to delete some files.
	# Impossible to use the docker-fix-permissions command directly.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/fix-permissions-dev.sh all "$(shell id -u)" "$(shell id -g)"'

	./scripts/package-finalize.sh ${TARGET_ENVIRONMENT} ${VERSION_TYPE} ${TARGET_VERSION}

.PHONY: deploy-package
deploy-package: ## Deploy a package. Example: make deploy-package TARGET_ENVIRONMENT=prod PACKAGE_NAME=9.x-2021-04-23-10h07m23s
	./scripts/deploy-package.sh ${TARGET_ENVIRONMENT} ${PACKAGE_NAME}
	./scripts/fix-permissions.sh ${TARGET_ENVIRONMENT} ${PACKAGE_NAME}

.PHONY: enable-release
enable-release: ## Enable a release. Example: make enable-release TARGET_ENVIRONMENT=prod PACKAGE_NAME=9.x-2021-04-23-10h07m23s SELECTED_SITE=all
	${MAKE} make-backup TARGET_ENVIRONMENT=${TARGET_ENVIRONMENT} SELECTED_SITE=${SELECTED_SITE}
	${MAKE} make-symlink TARGET_ENVIRONMENT=${TARGET_ENVIRONMENT} PACKAGE_NAME=${PACKAGE_NAME} SELECTED_SITE=${SELECTED_SITE}
	./scripts/update.sh ${TARGET_ENVIRONMENT} ${SELECTED_SITE}

.PHONY: site-install
site-install: ## Install the selected site. Example: make site-install TARGET_ENVIRONMENT=prod SELECTED_SITE=all
	./scripts/install.sh ${TARGET_ENVIRONMENT} ${SELECTED_SITE}

.PHONY: make-symlink
make-symlink: ## Make a symlink to a release. Example: make make-symlink TARGET_ENVIRONMENT=prod PACKAGE_NAME=9.x-2021-04-23-10h07m23s SELECTED_SITE=all
	./scripts/make-symlink.sh ${TARGET_ENVIRONMENT} ${PACKAGE_NAME} ${SELECTED_SITE}

.PHONY: make-backup
make-backup: ## Make a backup. Example: make make-backup TARGET_ENVIRONMENT=prod SELECTED_SITE=all
	./scripts/make-backup.sh ${TARGET_ENVIRONMENT} ${SELECTED_SITE}

.PHONY: restore-backup
restore-backup: ## Restore a backup. Example: make restore-backup TARGET_ENVIRONMENT=prod SELECTED_SITE=all
	./scripts/restore-backup.sh ${TARGET_ENVIRONMENT} ${SELECTED_SITE}

.PHONY: clear-backup
clear-backup: ## Select a backup to clear. Example: make clear-backup TARGET_ENVIRONMENT=prod SELECTED_SITE=all
	./scripts/clear-backup.sh ${TARGET_ENVIRONMENT} ${SELECTED_SITE}

################################################################################
## Security checks.
################################################################################

.PHONY: security-php
security-php: ## Launch PHP Security Checker on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		$(DRUSH) pm:security-php'

.PHONY: security-drupal
security-drupal: ## Launch Drupal Security Checker on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		$(DRUSH) pm:security'

.PHONY: security-all
security-all: ## Launch all security analysis tools of the project.
	# Use '|| true' to not stop if validation is failing.
	${MAKE} security-php || true
	${MAKE} security-drupal

################################################################################
## Code quality analysis.
################################################################################

.PHONY: quality-shellcheck
quality-shellcheck: ## Launch Shellcheck on project sources.
	docker run --rm -v "$(PWD):/mnt" koalaman/shellcheck:stable scripts/*.sh scripts/**/*.sh

.PHONY: quality-phpcs
quality-phpcs: ## Launch PHP CS on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
 		./vendor/bin/phpcs \
		--standard=./scripts/quality/phpcs/phpcs.xml.dist'

.PHONY: quality-phpmd
quality-phpmd: ## Launch PHP MD on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/phpmd \
		./app/modules/custom,./app/profiles/custom,./app/themes/custom \
		ansi \
		./scripts/quality/phpmd/phpmd.xml.dist \
		--suffixes inc,info,install,module,php,profile,test,theme'

.PHONY: quality-phpcpd
quality-phpcpd: ## Launch PHP CPD on project sources.
	# Scan index.php to avoid error if there is no file to scan.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/phpcpd \
		--min-lines=5 \
		--min-tokens=70 \
		--suffix=".inc,.info,.install,.module,.php,.profile,.test,.theme" \
		./app/index.php \
		./app/modules/custom \
		./app/profiles/custom \
		./app/themes/custom'

.PHONY: quality-phpmnd
quality-phpmnd: ## Launch PHP MND on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/phpmnd \
		--extensions=all \
		--hint \
		--non-zero-exit-on-violation \
		--suffixes="inc,info,install,module,php,profile,test,theme" \
		--exclude=tests \
		--ignore-funcs=intval,floatval,strval \
		./app/modules/custom \
		./app/profiles/custom \
		./app/themes/custom'

.PHONY: quality-phpstan
quality-phpstan: ## Launch PHPStan on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/phpstan \
		analyse \
		--configuration ./scripts/quality/phpstan/phpstan.neon.dist'

.PHONY: quality-psalm
quality-psalm: ## Launch Psalm on project sources.
	@docker-compose exec web bash -c 'cd ${APP_PATH} && \
		cp -f ${PROJECT_PATH}/scripts/quality/psalm/psalm.xml ./psalm.xml && \
		php ../vendor/mortenson/psalm-plugin-drupal/scripts/dump_script.php && \
		../vendor/bin/psalm || true && \
		rm -f ./DrupalContainerDump.xml && \
		rm -f ./psalm.xml && \
		rm -f ./psalm_drupal_entrypoint.module'

.PHONY: quality-composer-validate
quality-composer-validate: ## Launch Composer validate on root composer.json.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		composer validate'

.PHONY: quality-composer-validate-custom
quality-composer-validate-custom: ## Launch Composer validate on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/quality/composer_validate/composer-validate-custom.sh'

.PHONY: quality-composer-normalize
quality-composer-normalize: ## Launch Composer normalize on root composer.json.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		composer-normalize \
		--dry-run \
		--indent-size=4 \
		--indent-style=space'

.PHONY: quality-composer-normalize-custom
quality-composer-normalize-custom: ## Launch Composer normalize on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/quality/composer_normalize/composer-normalize-custom.sh'

.PHONY: quality-config-inspector
quality-config-inspector: ## Launch Config Inspector analysis. Require the website to be installed. Example: make quality-config-inspector SELECTED_SITE=all
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/quality/config_inspector/config-inspector.sh ${SELECTED_SITE}'

.PHONY: quality-css
quality-css: ## Launch Stylelint.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		${APP_PATH}/core/node_modules/.bin/stylelint \
			--config ${APP_PATH}/core/.stylelintrc.json \
			--formatter verbose \
			--ignore-path ./scripts/quality/stylelint/.stylelintignore \
			--allow-empty-input \
			"${APP_PATH}/**/custom/**/*.css"'

.PHONY: quality-sass
quality-sass: ## Launch Stylelint on sass/scss code.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		${APP_PATH}/core/node_modules/.bin/stylelint \
			--config ${PROJECT_PATH}/scripts/quality/sasslint/.stylelintrc.json \
			--config-basedir ${APP_PATH}/core/ \
			--formatter verbose \
			--ignore-path ./scripts/quality/sasslint/.stylelintignore \
			--allow-empty-input \
			"${APP_PATH}/**/custom/**/*.{scss,sass}"'

.PHONY: quality-js
quality-js: ## Launch ESLint for non-compiled JS.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		cd ${APP_PATH} && \
		${APP_PATH}/core/node_modules/.bin/eslint \
			--config ${APP_PATH}/core/.eslintrc.legacy.json \
			--no-error-on-unmatched-pattern \
			--ignore-path "$(PROJECT_PATH)/scripts/quality/eslint/.eslintignore" \
			"${APP_PATH}/modules/custom/**/*.js" \
			"${APP_PATH}/profiles/custom/**/*.js" \
			"${APP_PATH}/themes/custom/**/*.js"'

.PHONY: quality-js-es6
quality-js-es6: ## Launch ESLint for ES6 JS.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		cd ${APP_PATH} && \
		${APP_PATH}/core/node_modules/.bin/eslint \
			--config ${APP_PATH}/core/.eslintrc.json \
			--no-error-on-unmatched-pattern \
			--ignore-path "$(PROJECT_PATH)/scripts/quality/eslint_es6/.eslintignore" \
			"${APP_PATH}/modules/custom/**/*.es6.js" \
			"${APP_PATH}/profiles/custom/**/*.es6.js" \
			"${APP_PATH}/themes/custom/**/*.es6.js"'

.PHONY: quality-rector
quality-rector: ## Launch Rector.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/rector \
		process \
		app/modules/custom \
		app/profiles/custom \
		app/themes/custom \
		--dry-run \
		--config=./scripts/quality/rector/rector.php'

.PHONY: quality-php-cs-fixer
quality-php-cs-fixer: ## Launch PHP CS Fixer on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/php-cs-fixer fix \
		--dry-run \
		--config="./scripts/quality/phpcs_fixer/.php-cs-fixer.dist.php" \
		./app/modules/custom \
		./app/profiles/custom \
		./app/themes/custom'

.PHONY: quality-upgrade-status
quality-upgrade-status: ## Launch Upgrade Status analysis. Require the website to be installed.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		$(DRUSH) pm:install upgrade_status && \
		$(DRUSH) upgrade_status:analyze --all'

.PHONY: quality-yaml
quality-yaml: ## Launch ESLint for YAML.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		cd ${APP_PATH}/core && \
		./node_modules/.bin/eslint \
			--config ./.eslintrc.json \
			--no-error-on-unmatched-pattern \
			--ext .yml \
			"${PROJECT_PATH}/app/modules/custom" \
			"${PROJECT_PATH}/app/profiles/custom" \
			"${PROJECT_PATH}/app/themes/custom" \
			"${PROJECT_PATH}/conf/drupal" \
			"${PROJECT_PATH}/drush" \
			"${PROJECT_PATH}/scripts"'

.PHONY: quality-spellcheck
quality-spellcheck: ## Launch cspell.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		${APP_PATH}/core/node_modules/.bin/cspell \
			--config ${PROJECT_PATH}/scripts/quality/spellcheck/.cspell.json \
			--no-must-find-files \
			--no-progress \
			"${APP_PATH}/**/custom/**/*"'

.PHONY: quality-spellcheck-clean-dictionaries
quality-spellcheck-clean-dictionaries: ## Clean spellcheck dictionaries.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/quality/spellcheck/clean-dictionaries.sh'

.PHONY: quality-all
quality-all: ## Launch all code quality tools of the project.
	# Use '|| true' to not stop if validation is failing.
	${MAKE} quality-shellcheck || true
	${MAKE} quality-phpcs || true
	${MAKE} quality-phpmd || true
	${MAKE} quality-phpcpd || true
	${MAKE} quality-phpmnd || true
	${MAKE} quality-phpstan || true
	${MAKE} quality-psalm || true
	${MAKE} quality-composer-validate || true
	${MAKE} quality-composer-validate-custom || true
	${MAKE} quality-composer-normalize || true
	${MAKE} quality-composer-normalize-custom || true
	${MAKE} quality-config-inspector SELECTED_SITE=all || true
	${MAKE} quality-css || true
	${MAKE} quality-sass || true
	${MAKE} quality-js || true
	${MAKE} quality-js-es6 || true
	${MAKE} quality-php-cs-fixer || true
	${MAKE} quality-rector || true
	${MAKE} quality-spellcheck || true
	${MAKE} quality-yaml

################################################################################
## Code quality fixers.
################################################################################

.PHONY: quality-fix-composer-normalize
quality-fix-composer-normalize: ## Launch Composer normalize on root composer.json.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		composer-normalize \
		--indent-size=4 \
		--indent-style=space'

.PHONY: quality-fix-composer-normalize-custom
quality-fix-composer-normalize-custom: ## Launch Composer normalize on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/quality/composer_normalize/composer-normalize-custom.sh fix'

.PHONY: quality-fix-rector
quality-fix-rector: ## Launch Rector.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/rector \
		process \
		app/modules/custom \
		app/profiles/custom \
		app/themes/custom \
		--config=./scripts/quality/rector/rector.php'

.PHONY: quality-fix-php-cs-fixer
quality-fix-php-cs-fixer: ## Launch PHP CS Fixer on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./vendor/bin/php-cs-fixer fix \
		--config="./scripts/quality/phpcs_fixer/.php-cs-fixer.dist.php" \
		./app/modules/custom \
		./app/profiles/custom \
		./app/themes/custom'

.PHONY: quality-fix-css
quality-fix-css: ## Launch Stylelint fixer.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		${APP_PATH}/core/node_modules/.bin/stylelint \
			--config ${APP_PATH}/core/.stylelintrc.json \
			--formatter verbose \
			--ignore-path ./scripts/quality/stylelint/.stylelintignore \
			--fix \
			--allow-empty-input \
			"${APP_PATH}/**/custom/**/*.css"'

.PHONY: quality-fix-sass
quality-fix-sass: ## Launch Stylelint fixer on sass/scss code.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		${APP_PATH}/core/node_modules/.bin/stylelint \
			--config ${PROJECT_PATH}/scripts/quality/sasslint/.stylelintrc.json \
			--config-basedir ${APP_PATH}/core/ \
			--formatter verbose \
			--ignore-path ./scripts/quality/sasslint/.stylelintignore \
			--fix \
			--allow-empty-input \
			"${APP_PATH}/**/custom/**/*.{scss,sass}"'

.PHONY: quality-fix-js
quality-fix-js: ## Launch ESLint for non-compiled JS.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		cd ${APP_PATH} && \
		${APP_PATH}/core/node_modules/.bin/eslint \
			--config ${APP_PATH}/core/.eslintrc.legacy.json \
			--fix \
			--no-error-on-unmatched-pattern \
			--ignore-path "$(PROJECT_PATH)/scripts/quality/eslint/.eslintignore" \
			"${APP_PATH}/modules/custom/**/*.js" \
			"${APP_PATH}/profiles/custom/**/*.js" \
			"${APP_PATH}/themes/custom/**/*.js"'

.PHONY: quality-fix-js-es6
quality-fix-js-es6: ## Launch ESLint for ES6 JS.
	@docker-compose exec node /bin/sh -c 'cd $(PROJECT_PATH) && \
		yarn --cwd ${APP_PATH}/core install && \
		cd ${APP_PATH} && \
		${APP_PATH}/core/node_modules/.bin/eslint \
			--config ${APP_PATH}/core/.eslintrc.json \
			--fix \
			--no-error-on-unmatched-pattern \
			--ignore-path "$(PROJECT_PATH)/scripts/quality/eslint_es6/.eslintignore" \
			"${APP_PATH}/modules/custom/**/*.es6.js" \
			"${APP_PATH}/profiles/custom/**/*.es6.js" \
			"${APP_PATH}/themes/custom/**/*.es6.js"'

.PHONY: quality-fix-all
quality-fix-all: ## Launch all code quality fix of the project.
	# Use '|| true' to not stop if validation is failing.
	${MAKE} quality-fix-composer-normalize || true
	${MAKE} quality-fix-composer-normalize-custom || true
	# Run Rector before PHP CS in case Rector provokes a coding standard error.
	${MAKE} quality-fix-rector || true
	${MAKE} quality-fix-php-cs-fixer || true
	${MAKE} quality-fix-js || true
	${MAKE} quality-fix-js-es6 || true
	${MAKE} quality-fix-css || true
	${MAKE} quality-fix-sass

################################################################################
## Tests.
################################################################################

.PHONY: tests-behat
tests-behat: ## Launch Behat tests on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/tests/behat/run-tests-behat.sh'

.PHONY: tests-pa11y
tests-pa11y: ## Launch Pa11y scan.
	@docker-compose run --rm pa11y_ci /bin/sh -c "pa11y-ci --config /workspace/config.json"

.PHONY: tests-phpunit
tests-phpunit: ## Launch PHPUnit tests on project sources.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/tests/phpunit/run-tests-phpunit.sh'

.PHONY: tests-cypress-electron
tests-cypress-electron: ## Launch Cypress tests with Electron on project sources.
	@docker-compose run --rm cypress cypress run

.PHONY: tests-cypress-chrome
tests-cypress-chrome: ## Launch Cypress tests with Chrome on project sources.
	@docker-compose run --rm cypress cypress run --browser chrome

.PHONY: tests-cypress-firefox
tests-cypress-firefox: ## Launch Cypress tests with Firefox on project sources.
	@docker-compose run --rm cypress cypress run --browser firefox --config video=false

.PHONY: tests-gulp
tests-gulp: ## Check that Gulp can compile files.
	@docker-compose exec web bash -c 'cd $(PROJECT_PATH) && \
		./scripts/tests/gulp/test-gulp-compilation.sh'

# https://stackoverflow.com/a/6273809/1826109
%:
	@:
