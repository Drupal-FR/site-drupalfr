<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <!-- SORRY PLEASE DONT KILL ME AHAHA!!!! -->
  <div class="content"<?php print $content_attributes; ?> style="float: right; margin-left: 220px;">

    <?php print render($title_prefix); ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php print render($title_suffix); ?>

    <?php if ($display_submitted): ?>    
    <p class="submitted">
      <?php print $submitted; ?>
      <time pubdate datetime="<?php print $submitted_pubdate; ?>">
      <?php print $submitted_date; ?>
      </time>
    </p>
    <?php endif; ?>

    <?php
      // We hide the comments, tags and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      hide($content['field_tags']);
      hide($content['field_planete_user']);
      print render($content);
    ?>
  </div><!-- /.content -->

  <header style="width: 200px;">
    <?php print render($content['field_planete_user']); ?>
  </header>

  <?php if (!empty($content['field_tags']) || !empty($content['links'])): ?>
    <footer>
      <?php print render($content['field_tags']); ?>
      <?php print render($content['links']); ?>
    </footer>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

</article><!-- /.node -->
