<?php if ($images): ?>
  <div class="comment-upload-images">
    <?php foreach ($images as $fid => $image): ?>
      <a href="<?php print $image['url'] ?>"><?php print $image['image'] ?></a>
    <?php endforeach; ?>
  </div>
<?php endif; ?>

<?php if ($attachments): ?>
  <table class="comment-upload-attachments">
  <thead>
  <tr><th><?php print t('Attachment') ?></th><th><?php print t('Size') ?></th></tr>
  </thead>
  <tbody>
  <?php foreach ($attachments as $fid => $attachment): ?>
  <tr class="<?php print $attachment['zebra'] ?>">
  <td class="attachment-description"><a href="<?php print $attachment['url'] ?>"><?php print $attachment['text'] ?></a></td>
  <td class="attachment-size"><?php print $attachment['size'] ?></td>
  </tr>
  <?php endforeach; ?>
  </tbody>
  </table>
<?php endif; ?>