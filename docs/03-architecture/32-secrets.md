# Secrets

The [secrets replacement script](../../scripts/secrets-replacement.sh) allows to
inject secrets into generated packages and during scripts execution.

The list of variables to handle is listed in the [script parameters file](../../scripts/script-parameters.sh)
in the `ENV_VARIABLES_WITH_SECRETS` variable.

If no detection pattern is detected the value in the .env file will be used.

## Gitlab CI

When executed in the context of Gitlab CI, you need to configure variables in
your Gitlab CI's BO. The variables should be named with the pattern
`[VAR]_[SELECTED_ENV]` where `VAR` is the environment variable you want to
replace the value of and `SELECTED_ENV` is target env for example `PROD`.

Example: for `DRUPAL_SITE_DEFAULT_ACCOUNT_PASS`, configure
`DRUPAL_SITE_DEFAULT_ACCOUNT_PASS_INTEG`,
`DRUPAL_SITE_DEFAULT_ACCOUNT_PASS_PROD`, etc. in Gitlab CI.

## Bitwarden

Bitwarden is implemented. In the .env file the variable value should be
`bw:BITWARDEN_ID`.

Before launching Make commands, you need to retrieve a session key as documented
on https://bitwarden.com/help/article/cli/

```bash
bw login
```

Then read the return of the command to export the session.

## Other secrets management tool

You need to implement it in the [secrets replacement script](../../scripts/secrets-replacement.sh).
