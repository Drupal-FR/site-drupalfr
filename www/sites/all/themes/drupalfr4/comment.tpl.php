<?php
// $Id: comment.tpl.php,v 1.4.2.1 2008/03/21 21:58:28 goba Exp $

/**
 * @file comment.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
?>
<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status ?> <?php print $comment_class ?> clear-block">
  <div class="comment-right">
    <div class="right-box">
      <a class="anchor" href="<?php echo url($_GET['q'], array('fragment' => "comment-$comment->cid")) ?>"><?php print "#" ?></a>
      <?php if ($comment->new) { ?>
        <a id="new"></a>
        <span class="new"><?php print $new ?></span>
      <?php } ?>
    </div>
    <div class="content">
      <?php print $content ?>
      <?php if ($signature): ?>
        <div class="author-signature"><?php print check_markup($signature); ?></div>
      <?php endif ?>
    </div>
    <div class="footer clear-block">
    <div class="tools clear-block">
      <div class="post-date"><?php print drupalfr4_format_date($comment->timestamp) ?></div>
      <div class="links"><?php print $links ?></div>
    </div>
    <?php print drupalfr4_user_badge($comment) ?>
    </div>
  </div>

</div>
