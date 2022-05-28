# Front tips

## Avoid Drupal paranoia

Edit the following line in `conf/httpd/extra/httpd-vhosts.conf`:

```apacheconf
  DocumentRoot /project/www
  <Directory /project/www>
    ...
  </Directory>
```

into

```apacheconf
  DocumentRoot /project/app
  <Directory /project/app>
    ...
  </Directory>
```

Do NOT commit this change!
