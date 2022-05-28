# Gitlab CI

## Email notification

Go to `Settings > Integrations > Pipelines emails` and configure it as you want.

Standard configuration:
* Recipients: project mailing list
* Notify only broken pipelines: checked
* Branch for which notifications are to be sent: Default branch

## Scheduled security scan

Configure a weekly build to have security checkers running.

Go to `CI / CD > Schedules` and add a scheduled pipeline. For the interval, you
can set `0 5 * * 4` to be sure to be executed after the Drupal security updates
release window.

If you want to only execute the security stage, add variables to skip the other
stages. For example:
* SKIP_INSTALLATION: 1
* SKIP_CODE_QUALITY: 1
* SKIP_TESTS: 1
* SKIP_DEPLOY: 1

## CI deployment

* Go to `Settings > CI / CD` and add a variable `CI_SSH_KEY` to allow your CI to
  make deployment.
* Edit the `.gitlab-ci.yml` file to:
  * uncomment the package and deployment tasks you want
  * change the `9.x` branch by your development branch
  * change the environments' URL

### Integration deployment

Go to `CI / CD > Schedules` and add a scheduled pipeline. For the interval, you
can set `0 8 * * 1-5` to deploy each day in the morning.

Select your target branch.

You can only have the deployment tasks as the security, code quality and tests
should have already been executed when doing merge request. For example:
* SKIP_INSTALLATION: 1
* SKIP_SECURITY: 1
* SKIP_CODE_QUALITY: 1
* SKIP_TESTS: 1
