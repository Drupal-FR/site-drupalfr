
-- Munge emails for security.
UPDATE users SET mail = CONCAT(name, '@localhost'), init = CONCAT('http://drupalfr.org/user/', uid, '/edit'), pass = MD5(CONCAT('drupalfr', name)), login = 280299600, access = 280299600;
UPDATE comments SET mail = CONCAT(name, '@localhost');
UPDATE authmap SET authname = CONCAT(aid, '@localhost');
UPDATE client SET mail = CONCAT(cid, '@localhost');
UPDATE simplenews_subscriptions SET mail = CONCAT(snid, '@localhost');
UPDATE content_type_annuaire SET field_mail_email = CONCAT(nid, '@localhost');
UPDATE content_type_profile SET field_icq_value = nid WHERE field_icq_value IS NOT NULL;
UPDATE content_type_profile SET field_jabber_value = nid WHERE field_jabber_value IS NOT NULL;
UPDATE content_type_profile SET field_link_url = 'http://drupalfr.org' WHERE field_link_url IS NOT NULL;
UPDATE content_type_profile SET field_msn_email = CONCAT(nid, '@localhost') WHERE field_msn_email IS NOT NULL;
UPDATE content_type_profile SET field_realname_value = 'REAL NAME' WHERE field_realname_value IS NOT NULL;

-- Remove sensitive variables
DELETE FROM variable WHERE name = 'drupal_private_key';
DELETE FROM variable WHERE name LIKE '%key%';
DELETE FROM variable WHERE name = 'cron_semaphore';

-- Get rid of unpublished/blocked nodes, users, comments and related data in other tables.
DELETE FROM node WHERE status <> 1;
DELETE FROM comments WHERE status <> 0;
DELETE FROM users WHERE status <> 1 AND uid <> 0;
DELETE node FROM node LEFT JOIN users ON node.uid = users.uid WHERE users.uid IS NULL;
DELETE node_revisions FROM node_revisions LEFT JOIN node ON node.nid = node_revisions.nid WHERE node.nid IS NULL;
DELETE comments FROM comments LEFT JOIN node ON node.nid = comments.nid WHERE node.nid IS NULL;
DELETE comments FROM comments LEFT JOIN users ON comments.uid = users.uid WHERE users.uid IS NULL;
DELETE comments FROM comments LEFT JOIN comments c2 ON comments.pid = c2.cid WHERE c2.cid IS NULL AND comments.pid <> 0;

