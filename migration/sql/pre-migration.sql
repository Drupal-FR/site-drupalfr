
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
