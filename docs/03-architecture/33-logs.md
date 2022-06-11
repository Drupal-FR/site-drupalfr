# Logs

For classic server, not Docker.

Using Syslog, these are some recommended basic configuration.

## Drupal configuration

On the logs configuration page, in `Syslog identity` put your website folder
name, `default` if your website's folder is `default`.

## Syslog

Create a configuration file /etc/rsyslog.d/drupal_default.conf, one per site in
the case of a multi-sites installation, here default is the website folder
name:
```bash
if $syslogtag == 'drupal_default:' then /var/log/drupal_default.log
&~
```

## Logrotate

Create a configuration file /etc/logrotate.d/drupal.conf:
```bash
/var/log/drupal*.log
{
    daily
    rotate 365
    missingok
    notifempty
    compress
    delaycompress
    copytruncate
    dateext
}
```
