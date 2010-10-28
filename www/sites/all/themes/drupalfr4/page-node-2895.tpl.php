<?php
// $Id: page.tpl.php,v 1.1.2.1 2009/02/24 15:34:45 dvessel Exp $
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">
  <div id="site-header">
    <div class="container-16 clear-block">
      <div id="branding" class="grid-5 clear-block">
        <span id="logo" class="grid-1 alpha"><a href="<?php echo $GLOBALS['base_url'] ?>"><img src="<?php echo $GLOBALS['base_url'] . '/' . path_to_theme() . '/logo.gif' ?>" alt="Drupal france et francophonie"/></a></span>
      <?php if ($linked_site_name): ?>
        <h1 id="site-name" class="grid-3 omega"><?php print $linked_site_name; ?></h1>
      <?php endif; ?>
      <?php if ($site_slogan): ?>
        <div id="site-slogan" class="grid-3 omega"><?php print $site_slogan; ?></div>
      <?php endif; ?>
      </div>

    <?php if ($main_menu_links || $secondary_menu_links): ?>
      <div id="site-menu" class="grid-11">
        <?php print $main_menu_links; ?>
      </div>
    <?php endif; ?>

    </div>
  </div>

  <div id="site-subheader">
    <div id="site-subheader-inner" class="container-16 clear-block">
      <div id="breadcrumb" class="grid-12 clear-block"><?php print $breadcrumb ? $breadcrumb : '<div class="breadcrumb">' . variable_get('site_slogan', '') . '</div>'; ?></div>
      <div id="search-box" class="grid-4"><?php print $search_box; ?></div>
    </div>
  </div>

  <div id="page" class="container-16 clear-block">

    <div id="main" class="column grid-13 push-3">
      <?php if ($tabs): ?>
        <div id="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>
      <?php print $messages; ?>
      <?php print $help; ?>

      <div id="main-content" class="region clear-block">
        <?php print $content; ?>
      </div>

      <?php print $feed_icons; ?>
    </div>

  <?php if ($left): ?>
    <div id="sidebar-left" class="column sidebar region grid-3 pull-13">
      <?php print $left; ?>
    </div>
  <?php endif; ?>

  </div>

  <div id="footer">
    <div id="footer-inner" class="container-16">
    <?php if ($footer_message): ?>
      <div id="footer-message" class="grid-13">
        <?php print $footer_message; ?>
      </div>
    <?php endif; ?>
      <div id="footer-menu" class="grid-3">
        <?php print $secondary_menu_links; ?>
      </div>
    </div>
    <br style="clear: both;"/>
  </div>

  <?php print $closure; ?>
</body>
</html>
