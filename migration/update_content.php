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
<li class="fbl2"><a href="internal:traduction">La traduction</a></li>
<li class="fbl1"><a href="internal:support">Aide et assistance</a></li>
<li class="fbl2"><a href="internal:communaute">La communauté</a></li>
</ul>
EOF;
$homepage->body[LANGUAGE_NONE][0]['summary'] = '';
$homepage->body[LANGUAGE_NONE][0]['format'] = 1;

node_save($homepage);
