// $Id: active_tags.js,v 1.1.2.30 2010/05/10 14:38:25 dragonwize Exp $

/**
 * @file
 * Changes taxonomy tags fields to Active Tags style widgets.
 */

/**
 * Fills the suggestion popup with any matches received.
 *
 * Overwriting Drupal core's autocomplete.js found function to call
 * Drupal.attachBehaviors().
 */
if (Drupal.jsAC) {
  Drupal.jsAC.prototype.found = function (matches) {
    // If no value in the textfield, do not show the popup.
    if (!this.input.value.length) {
      return false;
    }

    // Prepare matches
    var ul = document.createElement('ul');
    var ac = this;
    for (key in matches) {
      var li = document.createElement('li');
      $(li)
        .html('<div>'+ matches[key] +'</div>')
        .mousedown(function () { ac.select(this); })
        .mouseover(function () { ac.highlight(this); })
        .mouseout(function () { ac.unhighlight(this); });
      li.autocompleteValue = key;
      $(ul).append(li);
    }

    // Show popup with matches, if any
    if (this.popup) {
      if (ul.childNodes.length > 0) {
        $(this.popup).empty().append(ul).show();
      }
      else {
        $(this.popup).css({visibility: 'hidden'});
        this.hidePopup();
      }
    }

    // Attach behaviors to new DOM content.
    Drupal.attachBehaviors(this.popup);
  };
}

function activeTagsParseCsv(sep, string) {
  for (var result = string.split(sep = sep || ","), x = result.length - 1, tl; x >= 0; x--) {
    if (result[x].replace(/"\s+$/, '"').charAt(result[x].length - 1) == '"') {
      if ((tl = result[x].replace(/^\s+"/, '"')).length > 1 && tl.charAt(0) == '"') {
        result[x] = result[x].replace(/^\s*"|"\s*$/g, '').replace(/""/g, '"');
      }
      else if (x) {
        result.splice(x - 1, 2, [result[x - 1], result[x]].join(sep));
      }
      else {
        result = result.shift().split(sep).concat(result);
      }
    }
    else {
      result[x].replace(/""/g, '"');
    }
  }
  return result;
}

function activeTagsActivate(context, index) {
  var wrapper = $(context);
  if (wrapper.length == 1) {
    var tagarea = activeTagsWidget(context, index);
    wrapper.before(tagarea);
    Drupal.behaviors.autocomplete(document);
  }
  $('.add-tag:not(.tag-processed)').click(function () {
    var tag = $(this).prev().val().replace(/["]/g, '');
    if (jQuery.trim(tag) != '') {
      activeTagsAdd(context, tag);
    }
    activeTagsUpdate(context);
    $(this).prev().val('');
  }).addClass('tag-processed');

  if ($.browser.mozilla) {
    $('.tag-entry:not(.tag-processed)').keypress(activeTagsCheckEnter).addClass('tag-processed');
  }
  else {
    $('.tag-entry:not(.tag-processed)').keydown(activeTagsCheckEnter).addClass('tag-processed');
  }

  jQuery.each(activeTagsParseCsv(',', wrapper.find('input.form-text').attr('value')), function(i, v) {
    if (jQuery.trim(v) != '') {
      activeTagsAdd(context, v);
    }
  });

  wrapper.hide();
}

function activeTagsCheckEnter(event) {
  if (event.keyCode == 13) {
    $('#autocomplete').each(function () {
      this.owner.hidePopup();
    });
    $(this).next().click();
    event.preventDefault();
    return false;
  }
}

function activeTagsAdd(context, tag) {
  $('#autocomplete').each(function () {
    this.owner.hidePopup();
  });

  // Removing all HTML tags. Need to wrap in tags for text() to work correctly.
  tag = $('<div>' + tag + '</div>').text();
  tag = Drupal.checkPlain(tag);
  tag = jQuery.trim(tag);
  if (tag != '') {
    $(context).prev().find('.tag-holder').append(Drupal.theme('activeTagsTerm', tag));
    $('.remove-tag:not(.tag-processed)').click(function () {
      $(this).parent().remove();
      activeTagsUpdate(context);
    }).addClass('tag-processed');
  }
}

function activeTagsUpdate(context) {
  var wrapper = $(context);
  var textFields = wrapper.children('input.form-text');
  textFields.val('');
  wrapper.prev().find('.tag-holder .tag-text').each(function (i) {
    // Get tag and revome quotes to prevent doubling
    var tag = $(this).text().replace(/["]/g, '');

    // Wrap in quotes if tag contains a comma.
    if (tag.search(',') != -1) {
      tag = '"' + tag + '"';
    }

    if (i == 0) {
      textFields.val(tag);
    }
    else {
      textFields.val(textFields.val() + ', ' + tag);
    }
  });
}

function activeTagsWidget(context, index) {
  var vid = context.substring(20, context.lastIndexOf('-'));
  return Drupal.theme('activeTagsWidget', context, vid, index);
}

function activeTagsAddTagOnSubmit() {
  $('.add-tag').click();
}

/**
 * Theme a selected term.
 */
Drupal.theme.prototype.activeTagsTerm = function (value) {
  return '<div class="tag-tag"><span class="tag-text">' + value + '</span><span class="remove-tag">x</span></div> ';
};

/**
 * Theme Active Tags widget.
 */
Drupal.theme.prototype.activeTagsWidget = function (context, vid, index) {
  var wrapper = $(context);
  var cleanId = context.replace('#', '');

  // Change default taxonomy description to reflect AT style workflow.
  var desc    = wrapper.find('.description').html();
  var coreStr = Drupal.t('A comma-separated list of terms describing this content. Example: funny, bungee jumping, "Company, Inc.".');
  var atStr   = Drupal.t('Enter one(1) term at a time. A comma will be included in the term and will NOT seperate terms.');
  desc = desc ? desc.replace(coreStr, atStr) : '';

  // Check if the field has an error class to add.
  var error = wrapper.find('input').hasClass('error') ? 'error ' : '';
  return '<div id="' + cleanId + '-activetags" class="form-item">' +
    '<label for="active-tag-edit0' + vid + '-' + index + '">' + wrapper.find('label').html() + '</label>' +
    '<div class="tag-holder"></div>' +
    '<input type="text" class="' + error + 'tag-entry form-autocomplete" size="30" id="active-tag-edit0' + vid + '-' + index + '" />' +
    '<input type="button" value="' + Drupal.t('Add') + '" class="add-tag" />' +
    '<input class="autocomplete" type="hidden" id="active-tag-edit0' + vid + '-' + index + '-autocomplete" ' +
    'value="' + $(context.replace('-wrapper', '-autocomplete')).val() + '" disabled="disabled" />' +
    '<div class="description">' + desc + '</div>' +
  '</div>';
};

Drupal.behaviors.activeTagsWidget = function (context) {
  jQuery.each(Drupal.settings['active_tags'], function (i, v) {
    var wrapper = $(v);
    if (wrapper.length == 1 && !wrapper.hasClass('active-tags-processed')) {
      activeTagsActivate(v, i);
      wrapper.addClass('active-tags-processed');
    }
  });
}

Drupal.behaviors.activeTagsAutocomplete = function (context) {
  $('li:not(.activeTagsAutocomplete-processed)', context)
    .addClass('activeTagsAutocomplete-processed')
    .each(function () {
      var li = this;
      $(li).focus(function () {
        $('#autocomplete').each(function () {
          this.owner.input.value = $(li).text();
        });
      }).mousedown(function () {
        $('input.add-tag').click();
        $('input.add-tag').prev().val('');
      });
  });
}

$(window).load(function () {
  // Setup tags to be added on form submit.
  $('#node-form').submit(activeTagsAddTagOnSubmit);
});
