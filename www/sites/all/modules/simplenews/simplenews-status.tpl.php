<?php
// $Id: simplenews-status.tpl.php,v 1.2.2.1 2009/01/02 11:59:33 sutharsan Exp $

/**
 * @file
 * Default theme implementation to display the simplenews newsletter status.
 *
 * Available variables:
 * - $image: status image
 * - $alt: 'alt' message
 * - $title: 'title' message
 *
 * @see template_preprocess_simplenews_status()
 */
?>
    <img src="<?php print $image; ?>" width="15" height="15" alt="<?php print $alt; ?>" border="0" title="<?php print $title; ?>" />
