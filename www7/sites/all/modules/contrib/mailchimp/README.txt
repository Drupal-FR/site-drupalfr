This module provides integration with the MailChimp email delivery service.
While tools for sending email from your own server, like SimpleNews, are great,
they lack the sophistication and ease of use of dedicated email providers like
MailChimp. Other players in the field are Constant Contact and Campaign Monitor.

The core module provides basic configuration and API integration. Features and
site functionality are provided by a set of submodules that depend upon the core
"mailchimp" module. These are in the "modules" subdirectory: See their
respective README's for more details.

## Features
  * API integration
  * Support for an unlimited number of mailing lists
  * Have anonymous sign up forms to subscribe site visitors to any combination
    of Mailchimp lists
  * Mailchimp list subscription via entity fields, allowing subscription rules
    to be governed by entity controls, permissions, and UI
  * Compatibility with Views Bulk Operations
  * Special VBO function for creating & updating static list segments
  * Allow users to subscribe during registration by adding a field to Users
  * Map Entity field values to your MailChimp merge fields
  * Standalone subscribe and unsubscribe forms
  * Subscriptions can be maintained via cron or in real time
  * Subscription forms can be created as pages or as blocks, with one or more
    list subscriptions on a single form
  * Include merge fields & interest groups on anonymous subscription forms
  * Create & send Mailchimp Campaigns from within Drupal, using Drupal entities
    as content.
  * Display a history of Mailchimp email and subscription activity on a tab for
    any Entity with an email address.

## Upgrade Notes
The 7.x-2.x and 7.x-3.x branches will become unsupported as MailChimp phases out
their API version 2.0 by the end of 2016. We recommend upgrading to 7.x-4.x, the
branch that is using MailChimp’s latest API version 3.0. The upgrade path is
manual at this point, requiring disabling & uninstalling the older branch, then
installing 7.x-4.x.

Worry not! Your lists, subscribers, and campaign data will remain safe and sound
on your MailChimp account (http://mailchimp.com/).

Please note: The major structural change between 7.x-2.x and 7.x-4.x is the
“Lists and Users” tab has been separated into three tabs “Fields”, “Lists” and
“SignUp Forms”. You will find most of the user related configuration by creating
a MailChimp field on the user account, here: admin/config/people/accounts/fields

For more information, please visit the FAQ (https://www.drupal.org/node/2793241)

## Installation Notes
  * You need to have a MailChimp API Key.
  * You need to have at least one list created in MailChimp to use the
    mailchimp_lists module.
  * If you use a drush make workflow, see the example drush makefile:
    mailchimp.make.example.

  * The MailChimp PHP library exist in your Druapl installation either via
    Libraries module or via Composer

    If you are using Libraries module for version 7.x-4.x:

      - Download Composer if you don't already have it installed:
        https://getcomposer.org/download/

      - Download version 1.0.3 of the v3 API library:
        https://github.com/thinkshout/mailchimp-api-php/releases

      - Extract the library archive to libraries/mailchimp

      - Ensure the directory structure looks like this:

        - libraries/
          - mailchimp/
            - src/
              - Mailchimp.php
              - MailchimpAPIException.php
              - MailchimpCampaigns.php
              - MailchimpLists.php
              - MailchimpReports.php
              - MailchimpTemplates.php
            - composer.json
            - README.md

      - In the mailchimp library directory, run:
        composer install

    If you are using Composer Manager for version 7.x-4.x:

      - Download Composer if you don't already have it installed:
        https://getcomposer.org/download/

      - Download and install Composer Manager Modulle:
        https://www.drupal.org/project/composer_manager

      - Install Drush on your system if you haven't already:
        http://www.drush.org/en/master/

      - Run Composer manager with Drush within your Drupal installation:
        drush composer-manager update --no-dev

    For module version 7.x-2.x and 7.x-3.x:

      - Download version 2.0.6 of the v2 API library:
        https://bitbucket.org/mailchimp/mailchimp-api-php/downloads

      - Extract the library archive to libraries/mailchimp

      - Ensure the directory structure looks like this:

        - libraries/
          - mailchimp/
            - docs/
            - src/
              - Mailchimp.php
              - Mailchimp/
            - README.md
            - composer.json

## Configuration
  1. Direct your browser to admin/config/services/mailchimp to configure the
  module.

  2. You will need to put in your Mailchimp API key for your Mailchimp account.
  If you do not have a Mailchimp account, go to
  [http://www.mailchimp.com]([http://www.mailchimp.com) and sign up for a new
  account. Once you have set up your account and are logged in, visit:
  Account Settings -> Extras -> API Keys to generate a key.

  3. Copy your newly created API key and go to the
  [Mailchimp config](http://example.com/admin/config/services/mailchimp) page in
  your Drupal site and paste it into the Mailchimp API Key field.

  4. Batch limit - Maximum number of changes to process in a single cron run.
  Mailchimp suggest keeping this below 10000.

## Submodules
  * mailchimp_signup: Create anonymous signup forms for your Mailchimp Lists,
    and display them as blocks or as standalone pages. Provide multiple-list
    subscription from a single form, include merge variables as desired, and
    optionally include Interest Group selection.
  * mailchimp_lists: Subscribe any entity with an email address to MailChimp
    lists by creating a mailchimp_list field, and allow anyone who can edit such
    an entity to subscribe, unsubscribe, and update member information. Also
    allows other entity fields to be synced to Mailchimp list Merge Fields. Add
    a Mailchimp Subscription field to your User bundle to allow Users to control
    their own subscriptions and subscribe during registration.
  * mailchimp_campaigns: Create and send campaigns directly from Drupal, or just
    create them and use the Mailchimp UI to send them. Embed content from your
    Drupal site by dropping in any Entity with a title and a View Mode
    configured into any area of your email template.
  * mailchimp_activity: Display a tab on any entity with an email address
    showing the email, subscribe, and unsubscribe history for that email address
    on your Mailchimp account.

## Related Modules
### Mandrill
  * Mandrill is MailChimp's transactional email service. The module provides the
    ability to send all site emails through Mandrill with reporting available
    from within Drupal. Please refer to the project page for more details.
  * http://drupal.org/project/mandrill
### MCC, an alternative campaign creation tool.
  * http://drupal.org/project/mcc
