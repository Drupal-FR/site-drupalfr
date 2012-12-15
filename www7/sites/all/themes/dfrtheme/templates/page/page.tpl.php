<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/garland.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content_top']: Items for the header region.
 * - $page['content']: The main content of the current page.
 * - $page['content_bottom']: Items for the header region.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>
<div id="page-wrapper">

	<div id="page">
		
		<header id="header" role="banner" class="clearfix"><div class="inner">

				<div class="branding">
					<?php if ($logo): ?>
						<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
							<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
						</a>
					<?php endif; ?>
				
					<?php if ($site_name || $site_slogan): ?>
						<?php if ($site_name): ?>
							<div id="site-name">
								<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
									<?php print $site_name; ?>
								</a>
							</div><!-- /#site-name -->
						<?php endif; ?>

						<?php if ($site_slogan): ?>
							<p id="site-slogan"><?php print $site_slogan; ?></p>
						<?php endif; ?>
					<?php endif; ?>
				</div><!-- /.branding -->

				<?php if ($page['menu']): ?>
					<?php print render($page['menu']); ?>
				<?php endif; ?>

		</div></header><!-- /#header -->

		<?php if ($page['header_bottom']): ?>
		<aside id="header-bottom" role="complementary"><div class="inner">
				<?php print render($page['header_bottom']); ?>
		</div></aside><!-- /#header-bottom -->
		<?php endif; ?>

		<div id="main-wrapper"><div class="inner">
		
			<?php print render($page['content_top']); ?>

			<?php if ($breadcrumb && !$is_front): ?>
				<div id="breadcrumb" class="clearfix"><?php print $breadcrumb; ?></div>
			<?php endif; ?>
			
			<div id="main" class="clearfix">
				  
				<div id="content" class="column" role="main">
					<div class="section">

						<?php if ($messages): ?>
							<div id="messages"><div class="section clearfix">
								<?php print $messages; ?>
							</div></div> <!-- /.section, /#messages -->
						<?php endif; ?>

						<?php print render($title_prefix); ?>
						<?php if ($title): ?>
							<h1 class="title" id="page-title"><?php print $title; ?></h1>
						<?php endif; ?>
						<?php print render($title_suffix); ?>

						<?php if ($tabs): ?>
							<div class="tabs"><?php print render($tabs); ?></div>
						<?php endif; ?>

						<?php print render($page['help']); ?>

						<?php if ($action_links): ?>
							<ul class="action-links"><?php print render($action_links); ?></ul>
						<?php endif; ?>

						<?php print render($page['content']); ?>

					</div><!-- /.section -->
				</div><!-- /#content -->

				<?php if ($page['sidebar_first']): ?>
					<aside id="sidebar-first" class="column sidebar" role="complementary">
						<div class="section">
							<?php print render($page['sidebar_first']); ?>
						</div><!-- /.section -->
					</aside><!-- /#sidebar-first -->
				<?php endif; ?>

			</div><!-- /#main -->
			
			<?php if ($page['content_bottom']): ?>
				<div class="content-bottom">
					<div class="content-bottom-wrap clearfix">
						<?php print render($page['content_bottom']); ?>
					</div>
				</div><!-- /.section -->
			<?php endif; ?>
			
		</div></div><!-- /#main-wrapper -->
		
		<?php if ($page['footer_top']): ?>
		<aside id="footer-top" role="complementary"><div class="inner">
			<?php print render($page['footer_top']); ?>
		</div></aside>
		<?php endif; ?>

		<?php if ($page['footer']): ?>
		<footer id="footer" role="contentinfo"><div class="inner">
			<?php print render($page['footer']); ?>
		</div></footer><!-- /#footer -->
		<?php endif; ?>

	</div><!-- /#page -->
</div><!-- /#page-wrapper -->