# Deployment structure

## Structure

When deployed, folders will have the following structure (with some folders name
adapted to your project):

/var/www/ddp9:
```bash
├── backups
│   ├── site1
│   │   └── DATE
│   │       ├── files-ENV-site1
│   │       ├── private_files-ENV-site1
│   │       └── ENV-site1.sql.gz
│   ├── site2
│   │   └── ...
│   └── ...
├── releases
│   ├── BRANCH_NAME-DATE
│   │   ├── app
│   │   │   └── sites
│   │   │       ├── site1
│   │   │       │   └── files -> /var/www/ddp9/shared/app/sites/site1/files
│   │   │       ├── site2
│   │   │       │   └── files -> /var/www/ddp9/shared/app/sites/site2/files
│   │   │       └── ...
│   │   ├── conf
│   │   ├── drush
│   │   ├── private_files
│   │   │   ├── site1 -> /var/www/ddp9/shared/private_files/site1
│   │   │   └── site2 -> /var/www/ddp9/shared/private_files/site2
│   │   └── ...
│   ├── TAG
│   └── ...
├── shared
│   ├── app
│   │   └── sites
│   │       ├── site1/files
│   │       ├── site2/files
│   │       └── ...
│   └── private_files
│       ├── site1
│       ├── site2
│       └── ...
└── sites
    ├── site1
    │   ├── current -> /var/www/ddp9/releases/TAG
    │   └── disabled_cron (when doing install or update)
    └── site2
        └── current -> /var/www/ddp9/releases/TAG2
```
