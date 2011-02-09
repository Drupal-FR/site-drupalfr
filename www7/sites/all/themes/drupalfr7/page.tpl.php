<?php
// $Id: page.tpl.php,v 1.1.2.2.4.2 2011/01/11 01:08:49 dvessel Exp $
?>

<div id="site-header">

  <div id="header" class="container-16 clearfix">
    <div id="branding" class="grid-4 clearfix">
      <?php if ($linked_logo_img): ?>
      <span id="logo" class="grid-1 alpha"><?php print $linked_logo_img; ?></span>
      <?php endif; ?>
      <?php if ($linked_site_name): ?>
      <h1 id="site-name" class="grid-3 omega"><?php print $linked_site_name; ?></h1>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
      <div id="site-slogan" class="grid-3 omega"><?php print $site_slogan; ?></div>
      <?php endif; ?>
    </div>

    <?php if ($page['header']): ?>
    <div id="header-region" class="region grid-12 clearfix">
    <?php print render($page['header']); ?>
    </div>
  <?php endif; ?>
  </div>
</div>

<div id="site-subheader">

  <div id="subheader" class="container-16 clearfix">

    <?php if ($breadcrumb): ?>
    <div id="breadcrumb" class="grid-12 clearfix">
    <?php print $breadcrumb; ?>
    </div>
    <?php endif; ?>


    <?php if ($page['subheader']): ?>
    <div id="subheader-region" class="clearfix <?php print ns('grid-16', $breadcrumb, 12); ?>">
    <?php print render($page['subheader']); ?>
    </div>
    <?php endif; ?>

  </div>

</div>

<div id="page" class="container-16 clearfix">

  <div id="main" class="column <?php print ns('grid-16', $page['sidebar_first'], 3, $page['sidebar_second'], 3) . ' ' . ns('push-3', !$page['sidebar_first'], 3); ?> clearfix">
    <?php print $breadcrumb; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>      
    <?php if ($tabs): ?>
      <div class="tabs"><?php print render($tabs); ?></div>
    <?php endif; ?>
    <?php print $messages; ?>
    <?php print render($page['help']); ?>

    <div id="main-content" class="region clearfix">
      <?php print render($page['content']); ?>
    </div>

    <?php print $feed_icons; ?>
  </div>

<?php if ($page['sidebar_first']): ?>
  <div id="sidebar-left" class="column sidebar region grid-3 <?php print ns('pull-13', $page['sidebar_second'], 3); ?> clearfix">
    <?php print render($page['sidebar_first']); ?>
  </div>
<?php endif; ?>

<?php if ($page['sidebar_second']): ?>
  <div id="sidebar-right" class="column sidebar region grid-3 clearfix">
    <?php print render($page['sidebar_second']); ?>
  </div>
<?php endif; ?>

</div>

<div id="footer" class="prefix-1 suffix-1 clearfix">
  <?php if ($page['footer']): ?>
    <div id="footer-region" class="region container-16 clearfix">
      <?php print render($page['footer']); ?>
    </div>
  <?php endif; ?>
</div>
