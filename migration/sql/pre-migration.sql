
-- The system.weight column in old Drupal databases might be very small,
-- fix it.
UPDATE system SET weight = 0 WHERE weight IS NULL;
ALTER TABLE system CHANGE COLUMN weight weight INT NOT NULL DEFAULT 0;

-- Disable all non core modules.
UPDATE system SET status = 0 WHERE type = 'module' AND filename NOT LIKE 'modules/%';

-- Recreate a UID=1 users.
INSERT INTO users (uid, name, pass, status) VALUES(1, 'admin', MD5('admin'), 1);

-- Change the theme to Garland.
UPDATE system SET status = 1 WHERE name = 'garland';
UPDATE variable SET value = 's:7:"garland";' WHERE name = 'theme_default';

-- Remove all files that have the same case insensitive name
-- NEED TO FIND A REAL SOLUTION
CREATE TABLE tfiles select filepath as filepath from files group by lower(`filepath`) HAVING count(fid) > 1;
DELETE FROM files WHERE filepath in (select filepath from tfiles);
DROP TABLE tfiles;

-- Remove orphaned action
DELETE FROM `actions` WHERE CONVERT(`actions`.`aid` USING utf8) = 'userpoints_action_grant_points' LIMIT 1;
DELETE FROM `actions` WHERE CONVERT(`actions`.`aid` USING utf8) = 'user_block_ip_action' LIMIT 1;

-- Drop profiles tables (no more needed since we using content profile)
-- DROP TABLE profile_fields;
-- DROP TABLE profile_values;
-- fake uninstall of profile module
-- UPDATE system SET schema_version = -1 WHERE name = 'profile';

-- remove internal:links (replaced by the module pathologic).
-- UPDATE node_revisions SET body = REPLACE(body, "internal:", "/");
-- UPDATE node_revisions SET teaser = REPLACE(teaser, "internal:", "/");

