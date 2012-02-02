// $Id: gcal.js,v 1.1.2.1 2011/02/09 19:20:23 timplunkett Exp $

/**
 * @file
 * Provides Google Calendar intergration with the FullCalendar plugin.
 */

/*
 * FullCalendar v1.4.10 Google Calendar Extension
 *
 * Copyright (c) 2010 Adam Shaw
 * Dual licensed under the MIT and GPL licenses.
 *
 * Date: Sat Jan 1 23:46:27 2011 -0800
 *
 */

(function($) {

$.fullCalendar.gcalFeedArray = function(feedArray) {

	return function(start, end, callback) {
		var events = [];
		var feedCount = 0;

		if (feedArray.length == 0) {
			callback(events);
		}

		$.each(feedArray, function(i, feedArrayEntry) {
			feedUrl = feedArrayEntry[0];
			feedUrl = feedUrl.replace(/\/basic$/, '/full');
			options = feedArrayEntry[1] || {};

			var params = {
				'start-min': $.fullCalendar.formatDate(start, 'u'),
				'start-max': $.fullCalendar.formatDate(end, 'u'),
				'singleevents': true,
				'events' : events,
				'max-results': 9999
			};
			var ctz = options.currentTimezone;
			if (ctz) {
				params.ctz = ctz = ctz.replace(' ', '_');
			}
			$.getJSON(feedUrl + "?alt=json-in-script&callback=?", params, function(data) {
				if (data.feed.entry) {
					$.each(data.feed.entry, function(i, entry) {
						var startStr = entry['gd$when'][0]['startTime'],
							start = $.fullCalendar.parseISO8601(startStr, true),
							end = $.fullCalendar.parseISO8601(entry['gd$when'][0]['endTime'], true),
							allDay = startStr.indexOf('T') == -1,
							url;
						$.each(entry.link, function() {
							if (this.type == 'text/html') {
								url = this.href;
								if (ctz) {
									url += (url.indexOf('?') == -1 ? '?' : '&') + 'ctz=' + ctz;
								}
							}
						});
						if (allDay) {
							$.fullCalendar.addDays(end, -1); // make inclusive
						}
						events.push({
							id: entry['gCal$uid']['value'],
							title: entry['title']['$t'],
							url: url,
							start: start,
							end: end,
							allDay: allDay,
							location: entry['gd$where'][0]['valueString'],
							description: entry['content']['$t'],
							className: options.className,
							editable: options.editable || false
						});
					});
				}

			feedCount++;
			if (feedCount === feedArray.length) {
				callback(events);
			}
			});
		});
	}
}

})(jQuery);
