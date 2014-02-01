Bienvenue sur le dépôt de Drupalfr.org.

Structure des dossiers :

/scripts/ scripts to handle drupalfr exploitation
/www7/ drupal sources

To deploy localy, create a settings.local.php file in /www7/sites/default/

With the following data:

<?php
$databases['default']['default'] = array(
  'driver' => 'mysql',
  'host' => 'localhost',
  'database' => '<databasename>',
  'username' => '<db_username>',
  'password' => '<db_password>',
  'prefix' => '',
);
$conf['cron_key'] = 'zMT41f2ar4tCNxVS_SN1mcWKNlQbIz9_se7KdzQlZHo';
$conf['dfr_emploi_rewrite_urls'] = FALSE;
$conf['drupalfr_planete_rewrite_urls'] = FALSE;

// Change cookie domain, example: .drupalfr.org .
$cookie_domain = '';

// If devel is enabled, uncomment this line to catch the emails. 
// $conf['mail_system'] = array('default-system' => 'DevelMailLog');


// SOLR configuration if you want to play with search.
$conf['drupalfr_solr_server_options'] = array(
  'host' => 'localhost',
  'port' => '8983',
  'path' => '/solr',
);

