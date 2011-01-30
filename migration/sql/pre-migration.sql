
-- The system.weight column in old Drupal databases might be very small,
-- fix it.
UPDATE system SET weight = 0 WHERE weight IS NULL;
ALTER TABLE system CHANGE COLUMN weight weight INT NOT NULL DEFAULT 0;

-- Disable all non core modules.
UPDATE system SET status = 0 WHERE type = 'module' AND filename NOT LIKE 'modules/%';
