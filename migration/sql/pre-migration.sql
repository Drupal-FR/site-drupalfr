UPDATE system SET weight = 0 WHERE weight IS NULL;
ALTER TABLE system CHANGE COLUMN weight weight INT NOT NULL DEFAULT 0;
