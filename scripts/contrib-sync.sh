#!/usr/bin/env bash

# Script used to rsync sources to make patches.

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh local

echo -e "${COLOR_LIGHT_GREEN}Copy contrib modules.${COLOR_NC}"
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/animated_gif "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/context_profile_role "${APP_PATH}"/modules/custom/
#rm -rf "${APP_PATH}"/modules/contrib/devel_a11y
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/devel_a11y "${APP_PATH}"/modules/custom/
#rm -rf "${APP_PATH}"/modules/contrib/devel_php
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/devel_php "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/drupal/core/* "${APP_PATH}"/core/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/drupal-l10n "${PROJECT_PATH}"/vendor/drupal-composer/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/entity_access_password "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/field_formatter_range "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/file_extractor "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/image_styles_mapping "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/matomo "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/menu_per_role "${APP_PATH}"/modules/custom/
#rm -rf "${APP_PATH}"/modules/contrib/renderviz
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/renderviz "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/select_icons "${APP_PATH}"/modules/custom/
#rm -rf "${APP_PATH}"/modules/contrib/speedboxes
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/speedboxes "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/ui_examples "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/ui_patterns_views_style "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/ui_styles "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/ui_suite_bootstrap "${APP_PATH}"/themes/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/ui_suite_zurb_foundation "${APP_PATH}"/themes/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/views_merge_rows "${APP_PATH}"/modules/custom/
# Smile.
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/cmis "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/contact_form_summary "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/entity_share "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/entity_share_cron "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/entity_visibility_preview "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/img_annotator "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/i18n_sso "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/qwantsearch "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/syslog_watcher "${APP_PATH}"/modules/custom/
#rsync -avzP --delete --exclude=".git" "${PROJECT_PATH}"/contrib/systempay "${APP_PATH}"/modules/custom/

echo -e "${COLOR_LIGHT_GREEN}Rebuild Drupal paranoia.${COLOR_NC}"
composer drupal:paranoia --working-dir="${PROJECT_PATH}"
