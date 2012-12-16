<div class="<?php print $classes; ?>">
<?php foreach($events as $event): ?>
  <div class="drupalfr-event-details drupalfr-event-<?php print $display_mode; ?>">
  <?php if ($display_mode == 'teaser'): ?>
    <p>
      <span class="event-name"><?php print l($event['name'], $event['event_url']); ?></span><br />
      <span class="event-date"><?php print format_date($event['time'] / 1000, 'short'); ?></span>
    </p>
  <?php elseif ($display_mode == 'full'): ?>
      <h2 class="event-name"><?php print l($event['name'], $event['event_url']); ?></h2>
      <div class="event-extra-info">
        <span class="event-date"><?php print format_date($event['time'] / 1000, 'short'); ?></span> //
        <span class="event-attendees"><?php print $event['yes_rsvp_count']; ?> participants</span>
      </div>
      <div class="event-description"><?php print $event['description']; ?></div>
    </p>
  <?php endif; ?>
  </div>
<?php endforeach; ?>
</div>
