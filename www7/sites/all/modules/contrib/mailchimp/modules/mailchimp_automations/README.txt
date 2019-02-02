Integrate your Drupal entities with Mailchimp's workflow automation endpoints.

## Installation

1. Enable the Mailchimp Automation module
2. Make sure you have a recent version of the Mailchimp PHP API library, which includes the MailchimpAutomations API service.

## Usage

1. Define which entity types you want to show campaign activity for at
/admin/config/services/mailchimp/automations.
  * Select a Drupal entity type.
  * Select a bundle.
  * Select the email entity property.
  * Select the appropriate Mailchimp List
  * Select the appropriate Mailchimp Workflow
  * Select the appropriate Mailchimp Workflow Email
2. Configure permissions for managing Mailchimp Automations

## Notes

1. The "Import mailchimp automation entity" button on the Automations admin tab will
throw a PHP error due to a bug in Entity API. You can prevent this error by
applying the patch in https://drupal.org/comment/8648215#comment-8648215 to
the entity module.
2. See additional options in the mailchimp_automations.api.php file, such as passing merge variables to Mailchimp.
