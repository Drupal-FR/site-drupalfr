#!/usr/bin/env bash
# shellcheck disable=SC2029

usage() {
  printf "restore-backup.sh [integ|staging|preprod|prod] [all|default]\n"
}

environment=''

if [ -n "$1" ]; then
  environment=$1
fi

# Check that all required parameters are present.
if [ -z "${environment}" ]; then
  echo "Missing target environment parameter."
  usage
  exit 1
elif [ "${environment}" = "local" ]; then
  echo "This script is not meant to be launched for local environment."
  usage
  exit 1
fi

# shellcheck source=scripts/script-parameters.sh
. "$(dirname "${BASH_SOURCE[0]}")"/script-parameters.sh "${environment}"

# shellcheck source=scripts/selection-site.sh
. "$(dirname "${BASH_SOURCE[0]}")"/selection-site.sh "$2"

# Check that the main front server is reachable.
if ! ping -c 1 "${FRONT_MAIN_SERVER_HOST}"; then
  echo -e "${COLOR_LIGHT_RED}Server ${FRONT_MAIN_SERVER_HOST}: Impossible to ping the server.${COLOR_NC}"
  exit 1
fi

for RESTORE_DRUPAL_SITE in "${DRUPAL_SITES_LIST[@]}"
do
  FOLDER_NAME="DRUPAL_SITE_${RESTORE_DRUPAL_SITE^^}_FOLDER_NAME"
  RESTORE_FILES="DRUPAL_SITE_${RESTORE_DRUPAL_SITE^^}_RESTORE_FILES"
  DRUSH_ALIAS="DRUPAL_SITE_${RESTORE_DRUPAL_SITE^^}_DRUSH_ALIAS"
  DRUSH="${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/current/vendor/bin/drush"

  # Test if the "current" symlink exists for this website.
  if $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "test -e ${DEPLOYMENT_PATH}/sites/${!FOLDER_NAME}/current"; then
    echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Select a backup to restore from.${COLOR_NC}"
    select backup_to_use in $($SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "ls -d ${DEPLOYMENT_PATH}/backups/${!FOLDER_NAME}/*")
    do
      if [ -n "$backup_to_use" ]; then
        if [ "${!RESTORE_FILES}" = "true" ]; then
          # Test if the public files backup folder exists.
          if $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "test -e $backup_to_use/files-${environment}-${!FOLDER_NAME}"; then
            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Delete shared public files folder.${COLOR_NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} rm -rf ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files"

            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Recreate shared public files folder.${COLOR_NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} mkdir -p ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files"

            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Copy public files backup.${NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} cp -R $backup_to_use/files-${environment}-${!FOLDER_NAME}/* ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files"

            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Set permissions for public files folder.${COLOR_NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chmod g+w -R ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chown ${PROJECT_USER}:${WEBSERVER_GROUP} -R ${DEPLOYMENT_PATH}/shared/app/sites/${!FOLDER_NAME}/files"
          fi

          # Test if the private files backup folder exists.
          if $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "test -e $backup_to_use/private_files-${environment}-${!FOLDER_NAME}"; then
            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Delete shared private files folder.${COLOR_NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} rm -rf ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}"

            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Recreate shared private files folder.${COLOR_NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} mkdir -p ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}"

            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Copy private files backup.${NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} cp -R $backup_to_use/private_files-${environment}-${!FOLDER_NAME}/* ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}"

            echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Set permissions for private files folder.${COLOR_NC}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chmod g+w -R ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}"
            $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo chown ${PROJECT_USER}:${WEBSERVER_GROUP} -R ${DEPLOYMENT_PATH}/shared/private_files/${!FOLDER_NAME}"
          fi
        fi

        # Database restoration.
        echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Delete extracted database backup in case one is already present.${COLOR_NC}"
        $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} rm -f $backup_to_use/${environment}-${!FOLDER_NAME}.sql"

        echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Extract database backup.${NC}"
        $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} gunzip -k $backup_to_use/${environment}-${!FOLDER_NAME}.sql.gz"

        # Test if there is a dump to inject.
        if $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "test -e $backup_to_use/${environment}-${!FOLDER_NAME}.sql"; then
          echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Drop existing database.${COLOR_NC}"
          $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} $DRUSH ${!DRUSH_ALIAS} sql:drop -y"

          echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Inject database backup.${COLOR_NC}"
          $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} $DRUSH ${!DRUSH_ALIAS} sql:cli < $backup_to_use/${environment}-${!FOLDER_NAME}.sql"

          echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Delete extracted database backup.${COLOR_NC}"
          $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${PROJECT_USER} rm -f $backup_to_use/${environment}-${!FOLDER_NAME}.sql"
        else
          echo -e "${COLOR_LIGHT_RED}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: No database backup to inject.${COLOR_NC}"
        fi
      fi

      # shellcheck source=scripts/update.sh
      . "$(dirname "${BASH_SOURCE[0]}")"/update.sh "${environment}" "${RESTORE_DRUPAL_SITE}"

      echo -e "${COLOR_BROWN_ORANGE}Enable or remove Search API indexes purge.${COLOR_NC}"
#      echo -e "${COLOR_LIGHT_GREEN}Server ${FRONT_MAIN_SERVER_HOST}: ${DRUPAL_SITE}: Purge Search API indexes.${COLOR_NC}"
#      $SSH "${SSH_USER}"@"${FRONT_MAIN_SERVER_HOST}" "sudo -u ${WEBSERVER_USER} $DRUSH ${!DRUSH_ALIAS} search-api:clear"
      break
    done
  else
    echo -e "${COLOR_BROWN_ORANGE}Server ${FRONT_MAIN_SERVER_HOST}: ${RESTORE_DRUPAL_SITE}: Is not enabled yet (no current symlink).${COLOR_NC}"
  fi
done
