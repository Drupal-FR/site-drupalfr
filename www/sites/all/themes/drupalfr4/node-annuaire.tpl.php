<?php if ($page): ?>
  <span class="submitted"><?php print theme('username', $node) ?> - <?php print format_date($node->created) ?></span>
  <table class="annuaire">
    <?php if ($node->field_logo[0]['filepath'] != "" || $node->field_description[0]['value'] != ""): ?>
      <tr class="first">
        <th>
          <?php if ($node->field_logo[0]['filepath'] != ""): ?>
            <?php print theme('image', $node->field_logo[0]['filepath']) ?>
          <?php else: ?>
            Présentation
          <?php endif ?>
        </th>
        <td><?php print $node->field_description[0]['view'] ?></td>
      </tr>
    <?php endif ?>
    <tr>
      <th>Type de services</th>
      <td>
        <ul>
          <?php foreach ($node->field_type_de_service as $service): ?>
            <li><?php print $service['view'] ?></li>
          <?php endforeach ?>
        </ul>
      </td>
    </tr>
    <?php if ($node->field_region[0]['value'] != ""): ?>
      <tr>
        <th>Implantation géographique</th>
        <td>
          <ul>
            <?php foreach ($node->field_region as $region): ?>
              <li><?php print $region['view'] ?></li>
            <?php endforeach ?>
          </ul>
        </td>
      </tr>
    <?php endif ?>
    <tr>
      <th>Forme juridique</th>
      <td><?php print $node->field_forme_juridique[0]['view'] ?></td>
    </tr>
    <?php if ($node->field_url[0]['url'] != ""): ?>
      <tr>
        <th>Site Internet</th>
        <td><?php print $node->field_url[0]['view'] ?></td>
      </tr>
    <?php endif ?>
    <tr>
      <th>Adresse e-mail</th>
      <td><?php print $node->field_mail[0]['view'] ?></td>
    </tr>
    <?php if ($node->field_telephone[0]['value'] != ""): ?>
      <tr>
        <th>Téléphone</th>
        <td><?php print $node->field_telephone[0]['view'] ?></td>
      </tr>
    <?php endif ?>
    <?php if ($node->field_adresse[0]['value'] != ""): ?>
      <tr>
        <th>Adresse</th>
        <td><?php print nl2br($node->field_adresse[0]['value']) ?></td>
      </tr>
    <?php endif ?>
    <tr>
      <th>Effectif</th>
      <td><?php print $node->field_taille[0]['view'] ?></td>
    </tr>
    <tr>
      <th>Membre de l'<a href="http://association.drupal.org/">association Drupal</a></th>
      <td><?php print $node->field_member[0]['view'] ?></td>
    </tr>
  </table>
  <?php if ($links) { ?><div class="links">&raquo; <?php print $links?></div><?php }; ?>
<?php endif ?>
