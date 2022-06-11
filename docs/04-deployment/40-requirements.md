# Deployment requirements

When deploying, on the target servers, the user used to connect to the server,
`SSH_USER`, should be able to launch Drush commands as the webserver user.

Example:
```bash
sudo -u www-data drush
```

The deployment user should also be able to create/remove/change files and
symlinks in the folder that matches the `DEPLOYMENT_PATH` variable value.

## Docker

For integration environment by example.

The user used to connect to the server should be added to the Docker group to be
able to launch docker commands.
