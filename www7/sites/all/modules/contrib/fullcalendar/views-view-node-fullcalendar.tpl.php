<?php
// $Id: views-view-node-fullcalendar.tpl.php,v 1.6.2.2 2011/02/02 20:33:05 timplunkett Exp $

/**
 * @file
 * View to display the fullcalendar rows (events)
 *
 * Variables available:
 * - $entity: The entity object.
 * - $url: The url for the event
 * - $data['field']: The field that contains the event date and time.
 * - $data['index']: The index of the event date and time (to support multiple values).
 * - $data['allDay']: If the event is all day (does not include hour and minute granularity).
 * - $data['start'] : When the event starts.
 * - $data['end'] : When the event ends.
 * - $className : The node type that the event came from
 *
 * Note that if you use className for the event's className attribute then you'll get weird results from jquery!
 */

?>
<?php if (!empty($data)): ?>
  <h3 class="title"><?php echo $entity->title; ?></h3>
  <?php foreach ($data as $row): ?>
    <div class="fullcalendar-instance">
      <?php print $row; ?>
    </div>
  <?php endforeach; ?>
<?php endif; ?>
