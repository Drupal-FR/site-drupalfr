ALTER TABLE system CHANGE COLUMN weight weight INT;
UPDATE system SET weight = 0 WHERE weight IS NULL;
