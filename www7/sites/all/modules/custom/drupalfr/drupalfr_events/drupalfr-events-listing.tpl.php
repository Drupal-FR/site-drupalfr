<?php
/**
 * @file
 * Template for meetups.
 */
?>
<div class="<?php print $classes; ?>">
<?php foreach($events as $event): ?>
  <div class="drupalfr-event-details drupalfr-event-<?php print $display_mode; ?>">
  <?php if ($display_mode == 'teaser'): ?>
    <p>
      <span class="event-name"><?php print l($event['name'], $event['event_url']); ?></span><br />
      <span class="event-date"><?php print format_date($event['time'] / 1000, 'medium_with_at'); ?> -</span>
      <span class="event-attendees"><?php print format_plural($event['drupalfr_inscription_count'], $event['drupalfr_inscription_count'] . ' inscrit', $event['drupalfr_inscription_count'] . ' inscrits'); ?></span>
      <?php if (isset($event['rsvp_limit'])): ?>
        <span class="event-limit">sur <?php print $event['rsvp_limit']; ?> places</span>
      <?php endif; ?>
    </p>
  <?php elseif ($display_mode == 'full'): ?>
      <h2 class="event-name"><?php print l($event['name'], $event['event_url']); ?></h2>
      <div class="event-extra-info">
        <span class="event-date"><?php print format_date($event['time'] / 1000, 'medium_with_at'); ?> -</span>
        <span class="event-attendees"><?php print format_plural($event['drupalfr_inscription_count'], $event['drupalfr_inscription_count'] . ' inscrit', $event['drupalfr_inscription_count'] . ' inscrits'); ?></span>
        <?php if (isset($event['rsvp_limit'])): ?>
          <span class="event-limit">pour <?php print $event['rsvp_limit']; ?> places</span>
        <?php endif; ?>
      </div>
      <?php if (isset($event['description'])): ?>
        <div class="event-description"><?php print text_summary($event['description'], NULL, 500); ?></div>
      <?php endif; ?>
  <?php endif; ?>
  </div>
<?php endforeach; ?>
</div>
