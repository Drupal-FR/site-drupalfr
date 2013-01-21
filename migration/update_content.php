<?php

// Load the homepage node and update its content. 
$homepage = node_load(2895);

$homepage->body[LANGUAGE_NONE][0]['value'] = <<<EOF
<p>
Drupal est un logiciel qui permet aux individus comme aux communautés d'utilisateurs de publier facilement, de gérer et d'organiser un vaste éventail de contenus sur un site web. Des dizaines de milliers de personnes et d'organisations utilisent Drupal pour propulser des sites de toutes tailles et fonctions.
</p>
<h3>En savoir plus</h3>
<ul>
<li class="fbl1"><a href="internal:apropos">A propos de Drupal</a></li>
<li class="fbl2"><a href="internal:documentation/traduction-drupal-français">La traduction</a></li>
<li class="fbl1"><a href="internal:documentation/support">Aide et assistance</a></li>
<li class="fbl2"><a href="internal:communaute">La communauté</a></li>
</ul>
EOF;
$homepage->body[LANGUAGE_NONE][0]['summary'] = '';
$homepage->body[LANGUAGE_NONE][0]['format'] = 1;

node_save($homepage);

// Create the first planet feeds.
$feed_informations = array(
  array(
    'user_id' => 'admin',
    'url' => 'http://drupalfr.org/frontpage/annonces/feed',
  ),
  array(
    'user_id' => 'anavarre',
    'url' => 'http://www.drupalfacile.org/blog.xml',
  ),
  array(
    'user_id' => 'Marc Delnatte',
    'url' => 'http://feeds.feedburner.com/akabia-blog',
  ),
  array(
    'user_id' => 'hellosct1',
    'url' => 'http://blog.hello-design.fr/index.php?feed/category/Drupal/atom',
  ),
  array(
    'user_id' => 'badgones',
    'url' => 'http://helpdrupal.tubaldo.com/rssdrupal.xml',
  ),
  array(
    'user_id' => 'Haza',
    'url' => 'http://blog.haza.fr/taxonomy/term/4/feed',
  ),
  array(
    'user_id' => 'pounard',
    'url' => 'http://blog.processus.org/fr/taxonomy/term/32/feed',
  ),
  array(
    'user_id' => 'Damien Tournoud',
    'url' => 'http://fr.commerceguys.com/blog/articles.xml/fr',
  ),
  array(
    'user_id' => 'GoZ',
    'url' => 'http://blog.fclement.info/taxonomy/term/8/0/feed',
  ),
  array(
    'user_id' => 'JulienD',
    'url' => 'http://juliendubreuil.fr/category/drupal/feed',
  ),
  array(
    'user_id' => 'Artusamak',
    'url' => 'http://juliendubois.fr/category/drupal/feed/',
  ),
  array(
    'user_id' => 'fgm@drupal.org',
    'url' => 'http://www.riff.org/drupal/feed',
  ),
  array(
    'user_id' => 'Yoran',
    'url' => 'http://arnumeral.fr/cat%C3%A9gorie/drupal/feed',
  ),
  array(
    'user_id' => 'j0nathan',
    'url' => 'http://koumbit.org/taxonomy/term/11/0/feed',
  ),
);

foreach ($feed_informations as $feed_information) {
  _migration_create_planet_feed($feed_information);
}


// Update the site mission block content.
$block_content = <<<EOF
<p>Ce site est réalisé par l'<a href="/contact">équipe de Drupalfr.org</a>, son contenu est mis à disposition selon le contrat <a href="http://creativecommons.org/licenses/by-sa/2.0/fr/">Paternité-ShareAlike 2.0 France</a>.</p>
<p>
Drupal est <a href="http://buytaert.net/drupal-trademark-policy-forthcoming">une marque déposée</a> de Dries Buytaert - Consulter les <a href="/mentions-legales">mentions légales</a> - Icones BUEditor fournies par <a href="http://www.famfamfam.com/lab/icons/silk/">FamFamFam</a>.
</p>
EOF;

db_query("UPDATE {block_custom} SET body = :value WHERE bid = 10", array(':value' => $block_content));
db_query("UPDATE {block} SET cache = :value WHERE delta = 10 AND module = 'block' AND theme = 'dfrtheme' ", array(':value' => DRUPAL_CACHE_GLOBAL));

// Create the mentions legales page.

$legal_mentions_content = <<<EOF
<p>
Les informations recueillies sont nécessaires pour votre adhésion.
</p>
<p>
Elles font l’objet d’un traitement informatique et sont destinées au secrétariat de l’association. En application des articles 39 et suivants de la loi du 6 janvier 1978 modifiée, vous bénéficiez d’un droit d’accès et de rectification aux informations qui vous concernent.
</p>
<p>
Si vous souhaitez exercer ce droit et obtenir communication des informations vous concernant, veuillez <a href="/contact">vous adresser à nous</a>.
</p>
EOF;

$node = new stdClass();
$node->nid = NULL;
$node->uid = 1;
$node->language = LANGUAGE_NONE;
$node->created = time();
$node->type = 'page';
$node->title = 'Mentions légales';
$node->cid = 0;
$node->body = array(
  LANGUAGE_NONE => array(
    array(
      'value' => $legal_mentions_content,
      'format' => 1,
    ),
  ),
);
node_save($node);
$path = array(
  'source' => 'node/' . $node->nid,
  'alias' => 'mentions-legales',
); 
path_save($path);

// Create alias for the homepage.
$path = array(
  'source' => 'node/2895',
  'alias' => 'accueil',
);
path_save($path);

function _migration_create_planet_feed ($feed_information) {
  $user_profile = user_load_by_name($feed_information['user_id']);
  $user_profile->field_planete_rss['und'][0]['url'] = $feed_information['url'];
  user_save($user_profile);
}
