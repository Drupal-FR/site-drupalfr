// $Id: fullcalendar.views.js,v 1.4.2.7 2011/02/10 14:00:21 timplunkett Exp $

/**
 * @file
 * Integrates Views data with the FullCalendar plugin.
 */

(function ($) {

Drupal.behaviors.fullCalendar = {
  attach: function(context) {
    // Process each view and its settings.
    $.each(Drupal.settings.fullcalendar, function(index, settings) {
      // Hide the failover display.
      $(index).find('.fullcalendar-content').hide(); 
      // Use .once() to protect against extra AJAX calls from Colorbox.
      $(index).find('.fullcalendar').once().fullCalendar({
        defaultView: settings.defaultView,
        theme: settings.theme,
        header: {
          left: settings.left,
          center: settings.center,
          right: settings.right
        },
        isRTL: settings.isRTL === '1',
        eventClick: function(calEvent, jsEvent, view) {
          if (settings.colorbox) {
          // Open in colorbox if exists, else open in new window.
            if ($.colorbox) {
              var url = calEvent.url;
              if (settings.colorboxClass !== '') {
                url += ' ' + settings.colorboxClass;
              }
              $.colorbox({
                href: url,
                width: settings.colorboxWidth,
                height: settings.colorboxHeight
              });
            }
          }
          else {
            if (settings.sameWindow) {
              window.open(calEvent.url, _self);
            }
            else {
              window.open(calEvent.url);
            }
          }
          return false;
        },
        year: (settings.year) ? settings.year : undefined,
        month: (settings.month) ? settings.month : undefined,
        day: (settings.day) ? settings.day : undefined,
        timeFormat: {
          agenda: (settings.clock) ? 'HH:mm{ - HH:mm}' : settings.agenda,
          '': (settings.clock) ? 'HH:mm' : 'h(:mm)t'
        },
        axisFormat: (settings.clock) ? 'HH:mm' : 'h(:mm)tt',
        weekMode: settings.weekMode,
        firstDay: settings.firstDay,
        monthNames: settings.monthNames,
        monthNamesShort: settings.monthNamesShort,
        dayNames: settings.dayNames,
        dayNamesShort: settings.dayNamesShort,
        allDayText: settings.allDayText,
        buttonText: {
          today:  settings.todayString,
          day: settings.dayString,
          week: settings.weekString,
          month: settings.monthString
        },
        eventSources: [
          // Add events from Drupal.
          function(start, end, callback) {
            var events = [];

            $(index).find('.fullcalendar-event-details').each(function() {
              events.push({
                field: $(this).attr('field'),
                index: $(this).attr('index'),
                eid: $(this).attr('eid'),
                entity_type: $(this).attr('entity_type'),
                title: $(this).attr('title'),
                start: $(this).attr('start'),
                end: $(this).attr('end'),
                url: $(this).attr('href'),
                allDay: ($(this).attr('allDay') === '1'),
                className: $(this).attr('cn'),
                editable: $(this).attr('editable'),
                dom_id: index
              });
            });

            callback(events);
          },
          // Add events from Google Calendar feeds.
          $.fullCalendar.gcalFeedArray(settings.gcal)
        ],
        eventDrop: function(event, dayDelta, minuteDelta, allDay, revertFunc) {
          $.post(Drupal.settings.basePath + 'fullcalendar/ajax/update/drop/'+ event.eid,
            'field=' + event.field + '&entity_type=' + event.entity_type + '&index=' + event.index + '&day_delta=' + dayDelta + '&minute_delta=' + minuteDelta + '&all_day=' + allDay + '&dom_id=' + event.dom_id,
            fullcalendarUpdate);
          return false;
        },
        eventResize: function(event, dayDelta, minuteDelta, revertFunc) {
          $.post(Drupal.settings.basePath + 'fullcalendar/ajax/update/resize/'+ event.eid,
            'field=' + event.field + '&entity_type=' + event.entity_type + '&index=' + event.index + '&day_delta=' + dayDelta + '&minute_delta=' + minuteDelta + '&dom_id=' + event.dom_id,
            fullcalendarUpdate);
          return false;
        }
      });
    });

    var fullcalendarUpdate = function(result) {
      fcStatus = $(result.dom_id).find('.fullcalendar-status');
      if (fcStatus.text() === '') {
        fcStatus.html(result.msg).slideDown();
      }
      else {
        fcStatus.effect('highlight', {}, 5000);
      }
      return false;
    };

    $('.fullcalendar-status-close').live('click', function() {
      $(this).parent().slideUp();
      return false;
    });

    // Trigger a window resize so that calendar will redraw itself as it loads funny in some browsers occasionally
    $(window).resize();
  }
};

})(jQuery);
