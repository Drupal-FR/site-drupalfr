
-- Munge emails for security.
UPDATE users                    SET mail = CONCAT(name, '@localhost.local'), init = CONCAT('http://drupalfr.org/user/', uid, '/edit'), pass = '', login = 280299600, access = 280299600;
UPDATE comment                  SET mail = CONCAT(name, '@localhost.local'), hostname = '';
UPDATE authmap                  SET authname             = CONCAT(aid, '@localhost.local');
UPDATE client                   SET mail                 = CONCAT(cid, '@localhost.local');
UPDATE simplenews_subscriptions SET mail                 = CONCAT(snid, '@localhost.local');
UPDATE content_type_annuaire    SET field_mail_email     = CONCAT(nid, '@localhost.local') WHERE field_mail_email     IS NOT NULL;
UPDATE content_type_profile     SET field_msn_email      = CONCAT(nid, '@localhost.local') WHERE field_msn_email      IS NOT NULL;
UPDATE content_type_profile     SET field_icq_value      = nid                             WHERE field_icq_value      IS NOT NULL;
UPDATE content_type_profile     SET field_jabber_value   = nid                             WHERE field_jabber_value   IS NOT NULL;
UPDATE content_type_profile     SET field_link_url       = 'http://drupalfr.org'           WHERE field_link_url       IS NOT NULL;
UPDATE content_type_profile     SET field_realname_value = 'REAL NAME'                     WHERE field_realname_value IS NOT NULL;

-- User fields.
UPDATE field_data_field_msn         SET field_msn_email         = CONCAT(entity_id, '@localhost.local') WHERE field_msn_email         IS NOT NULL;
UPDATE field_data_field_mail        SET field_mail_email        = CONCAT(entity_id, '@localhost.local') WHERE field_mail_email        IS NOT NULL;
UPDATE field_data_field_icq         SET field_icq_value         = entity_id                             WHERE field_icq_value         IS NOT NULL;
UPDATE field_data_field_jabber      SET field_jabber_value      = entity_id                             WHERE field_jabber_value      IS NOT NULL;
UPDATE field_data_field_facebook    SET field_facebook_value    = entity_id                             WHERE field_facebook_value    IS NOT NULL;
UPDATE field_data_field_twitter     SET field_twitter_value     = entity_id                             WHERE field_twitter_value     IS NOT NULL;
UPDATE field_data_field_linkedin    SET field_linkedin_value    = entity_id                             WHERE field_linkedin_value    IS NOT NULL;
UPDATE field_data_field_viadeo      SET field_viadeo_value      = entity_id                             WHERE field_viadeo_value      IS NOT NULL;
UPDATE field_data_field_google_plus SET field_google_plus_value = entity_id                             WHERE field_google_plus_value IS NOT NULL;
UPDATE field_data_field_drupal_org  SET field_drupal_org_value  = entity_id                             WHERE field_drupal_org_value  IS NOT NULL;
UPDATE field_data_field_link        SET field_link_url          = 'http://drupalfr.org'                 WHERE field_link_url          IS NOT NULL;
UPDATE field_data_field_realname    SET field_realname_value    = 'REAL NAME'                           WHERE field_realname_value    IS NOT NULL;
UPDATE field_data_field_nickname    SET field_nickname_value    = 'NICK NAME'                           WHERE field_nickname_value    IS NOT NULL;
UPDATE field_data_field_biography   SET field_biography_value   = 'Biography'                           WHERE field_biography_value   IS NOT NULL;
UPDATE field_data_field_website     SET field_website_url       = 'http://drupalfr.org'                 WHERE field_website_url       IS NOT NULL;
UPDATE field_data_field_website     SET field_website_title     = 'Drupal France'                       WHERE field_website_title     IS NOT NULL;
UPDATE field_data_field_membership_number   SET field_membership_number_value = '1'                     WHERE field_membership_number_value     IS NOT NULL;
-- field_planete_rss?

-- User field revisions.
UPDATE field_revision_field_msn         SET field_msn_email         = CONCAT(entity_id, '@localhost.local') WHERE field_msn_email         IS NOT NULL;
UPDATE field_revision_field_mail        SET field_mail_email        = CONCAT(entity_id, '@localhost.local') WHERE field_mail_email        IS NOT NULL;
UPDATE field_revision_field_icq         SET field_icq_value         = entity_id                             WHERE field_icq_value         IS NOT NULL;
UPDATE field_revision_field_jabber      SET field_jabber_value      = entity_id                             WHERE field_jabber_value      IS NOT NULL;
UPDATE field_revision_field_facebook    SET field_facebook_value    = entity_id                             WHERE field_facebook_value    IS NOT NULL;
UPDATE field_revision_field_twitter     SET field_twitter_value     = entity_id                             WHERE field_twitter_value     IS NOT NULL;
UPDATE field_revision_field_linkedin    SET field_linkedin_value    = entity_id                             WHERE field_linkedin_value    IS NOT NULL;
UPDATE field_revision_field_viadeo      SET field_viadeo_value      = entity_id                             WHERE field_viadeo_value      IS NOT NULL;
UPDATE field_revision_field_google_plus SET field_google_plus_value = entity_id                             WHERE field_google_plus_value IS NOT NULL;
UPDATE field_revision_field_drupal_org  SET field_drupal_org_value  = entity_id                             WHERE field_drupal_org_value  IS NOT NULL;
UPDATE field_revision_field_link        SET field_link_url          = 'http://drupalfr.org'                 WHERE field_link_url          IS NOT NULL;
UPDATE field_revision_field_realname    SET field_realname_value    = 'REAL NAME'                           WHERE field_realname_value    IS NOT NULL;
UPDATE field_revision_field_nickname    SET field_nickname_value    = 'NICK NAME'                           WHERE field_nickname_value    IS NOT NULL;
UPDATE field_revision_field_biography   SET field_biography_value   = 'Biography'                           WHERE field_biography_value   IS NOT NULL;
UPDATE field_revision_field_website     SET field_website_url       = 'http://drupalfr.org'                 WHERE field_website_url       IS NOT NULL;
UPDATE field_revision_field_website     SET field_website_title     = 'Drupal France'                       WHERE field_website_title     IS NOT NULL;
UPDATE field_revision_field_membership_number   SET field_membership_number_value = '1'                     WHERE field_membership_number_value     IS NOT NULL;

-- User commerce address.
UPDATE field_data_commerce_customer_address SET
  commerce_customer_address_country = 'FR',
  commerce_customer_address_administrative_area = '',
  commerce_customer_address_sub_administrative_area = '',
  commerce_customer_address_locality = 'Paris',
  commerce_customer_address_dependent_locality = '',
  commerce_customer_address_postal_code = '75003',
  commerce_customer_address_thoroughfare = '21 Place de la République',
  commerce_customer_address_premise = '',
  commerce_customer_address_sub_premise = '',
  commerce_customer_address_organisation_name = '',
  commerce_customer_address_name_line = 'Association Drupal France et Francophonie',
  commerce_customer_address_first_name = 'Association',
  commerce_customer_address_last_name = 'Drupal France et Francophonie',
  commerce_customer_address_data = '';
UPDATE field_revision_commerce_customer_address SET
  commerce_customer_address_country = 'FR',
  commerce_customer_address_administrative_area = '',
  commerce_customer_address_sub_administrative_area = '',
  commerce_customer_address_locality = 'Paris',
  commerce_customer_address_dependent_locality = '',
  commerce_customer_address_postal_code = '75003',
  commerce_customer_address_thoroughfare = '21 Place de la République',
  commerce_customer_address_premise = '',
  commerce_customer_address_sub_premise = '',
  commerce_customer_address_organisation_name = '',
  commerce_customer_address_name_line = 'Association Drupal France et Francophonie',
  commerce_customer_address_first_name = 'Association',
  commerce_customer_address_last_name = 'Drupal France et Francophonie',
  commerce_customer_address_data = '';

-- Remove sensitive variables.
DELETE FROM variable WHERE name = 'drupal_private_key';
DELETE FROM variable WHERE name LIKE '%key%';
DELETE FROM variable WHERE name = 'cron_semaphore';

-- Get rid of unpublished/blocked nodes, users, comments and related data in other tables.
DELETE FROM node    WHERE status <> 1;
DELETE FROM comment WHERE status <> 0;
DELETE FROM users   WHERE status <> 1 AND uid <> 0;
DELETE node          FROM node          LEFT JOIN users      ON node.uid = users.uid         WHERE users.uid IS NULL;
DELETE node_revision FROM node_revision LEFT JOIN node       ON node.nid = node_revision.nid WHERE node.nid  IS NULL;
DELETE comment       FROM comment       LEFT JOIN node       ON node.nid = comment.nid       WHERE node.nid  IS NULL;
DELETE comment       FROM comment       LEFT JOIN users      ON comment.uid = users.uid      WHERE users.uid IS NULL;
DELETE comment       FROM comment       LEFT JOIN comment c2 ON comment.pid = c2.cid         WHERE c2.cid    IS NULL AND comment.pid <> 0;
