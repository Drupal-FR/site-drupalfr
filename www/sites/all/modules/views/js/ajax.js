/**
 * @file ajax_admin.js
 *
 * Handles AJAX submission and response in Views UI.
 */

Drupal.Views.Ajax = Drupal.Views.Ajax || {};

/**
 * Handles the simple process of setting the ajax form area with new data.
 */
Drupal.Views.Ajax.setForm = function(title, output) {
  $(Drupal.settings.views.ajax.title).html(title);
  $(Drupal.settings.views.ajax.id).html(output);
}

/**
 * An ajax responder that accepts a packet of JSON data and acts appropriately.
 *
 * The following fields control behavior.
 * - 'display': Display the associated data in the form area; bind the new
 *   form to 'url' if appropriate. The 'title' field may also be used.
 * - 'add': This is a keyed array of HTML output to add via append. The key is
 *   the id to append via $(key).append(value)
 * - 'replace': This is a keyed array of HTML output to add via replace. The key is
 *   the id to append via $(key).html(value)
 *
 */
Drupal.Views.Ajax.ajaxResponse = function(data) {
  $('a.views-throbbing').removeClass('views-throbbing');
  $('span.views-throbbing').remove();

  if (data.debug) {
    alert(data.debug);
  }

  // See if we have any settings to extend. Do this first so that behaviors
  // can access the new settings easily.

  if (Drupal.settings.viewsAjax) {
    Drupal.settings.viewsAjax = {};
  }
  if (data.js) {
    $.extend(Drupal.settings, data.js);
  }

  // Check the 'display' for data.
  if (data.display) {
    Drupal.Views.Ajax.setForm(data.title, data.display);

    // if a URL was supplied, bind the form to it.
    if (data.url) {
      var ajax_area = Drupal.settings.views.ajax.id;
      var ajax_title = Drupal.settings.views.ajax.title;

      // Bind a click to the button to set the value for the button.
      $('input[type=submit], button', ajax_area).unbind('click');
      $('input[type=submit], button', ajax_area).click(function() {
        $('form', ajax_area).append('<input type="hidden" name="'
          + $(this).attr('name') + '" value="' + $(this).val() + '">');
        $(this).after('<span class="views-throbbing">&nbsp</span>');
      });

      // Bind forms to ajax submit.
      $('form', ajax_area).unbind('submit'); // be safe here.
      $('form', ajax_area).submit(function(arg) {
        $(this).ajaxSubmit({
          url: data.url,
          data: { 'js': 1 },
          type: 'POST',
          success: Drupal.Views.Ajax.ajaxResponse,
          error: function(xhr) { $('span.views-throbbing').remove(); Drupal.Views.Ajax.handleErrors(xhr, data.url); },
          dataType: 'json'
        });
        return false;
      });
    }

    Drupal.attachBehaviors(ajax_area);
  }
  else if (!data.tab) {
    // If no display, reset the form.
    Drupal.Views.Ajax.setForm('', Drupal.settings.views.ajax.defaultForm);
    //Enable the save and delete button.
    $('#edit-save').removeAttr('disabled');
    $('#edit-delete').removeAttr('disabled');
    // Trigger an update for the live preview when we reach this state:
    if ($('#views-ui-preview-form input#edit-live-preview').is(':checked')) {
      $('#views-ui-preview-form').trigger('submit');
    }
  }

  // Go through the 'add' array and add any new content we're instructed to add.
  if (data.add) {
    for (id in data.add) {
      var newContent = $(id).append(data.add[id]);
      Drupal.attachBehaviors(newContent);
    }
  }

  // Go through the 'replace' array and replace any content we're instructed to.
  if (data.replace) {
    for (id in data.replace) {
      $(id).html(data.replace[id]);
      Drupal.attachBehaviors(id);
    }
  }

  // Go through and add any requested tabs
  if (data.tab) {
    for (id in data.tab) {
      // Retrieve the tabset instance by stored ID.
      var instance = Drupal.Views.Tabs.instances[$('#views-tabset').data('UI_TABS_UUID')];
      instance.add(id, data.tab[id]['title'], 0);
      instance.click(instance.$tabs.length);

      $(id).html(data.tab[id]['body']);
      $(id).addClass('views-tab');

      // Update the preview widget to preview the new tab.
      var display_id = id.replace('#views-tab-', '');
      $("#preview-display-id").append('<option selected="selected" value="' + display_id + '">' + data.tab[id]['title'] + '</option>');

      Drupal.attachBehaviors(id);
    }
  }

  if (data.hilite) {
    $('.hilited').removeClass('hilited');
    $(data.hilite).addClass('hilited');
  }

  if (data.changed) {
    $('div.views-basic-info').addClass('changed');
  }
}

/**
 * An ajax responder that accepts a packet of JSON data and acts appropriately.
 * This one specifically responds to the Views live preview area, so it's
 * hardcoded and specialized.
 */
Drupal.Views.Ajax.previewResponse = function(data) {
  $('a.views-throbbing').removeClass('views-throbbing');
  $('span.views-throbbing').remove();

  if (data.debug) {
    alert(data.debug);
  }

  // See if we have any settings to extend. Do this first so that behaviors
  // can access the new settings easily.

  // Clear any previous viewsAjax settings.
  if (Drupal.settings.viewsAjax) {
    Drupal.settings.viewsAjax = {};
  }
  if (data.js) {
    $.extend(Drupal.settings, data.js);
  }

  // Check the 'display' for data.
  if (data.display) {
    var ajax_area = 'div#views-live-preview';
    $(ajax_area).html(data.display);

    var url = $(ajax_area, 'form').attr('action');

    // if a URL was supplied, bind the form to it.
    if (url) {
      // Bind a click to the button to set the value for the button.
      $('input[type=submit], button', ajax_area).unbind('click');
      $('input[type=submit], button', ajax_area).click(function() {
        $('form', ajax_area).append('<input type="hidden" name="'
          + $(this).attr('name') + '" value="' + $(this).val() + '">');
        $(this).after('<span class="views-throbbing">&nbsp</span>');
      });

      // Bind forms to ajax submit.
      $('form', ajax_area).unbind('submit'); // be safe here.
      $('form', ajax_area).submit(function() {
        $(this).ajaxSubmit({
          url: url,
          data: { 'js': 1 },
          type: 'POST',
          success: Drupal.Views.Ajax.previewResponse,
          error: function(xhr) { $('span.views-throbbing').remove(); Drupal.Views.Ajax.handleErrors(xhr, url); },
          dataType: 'json'
        });
        return false;
      });
    }

    Drupal.attachBehaviors(ajax_area);
  }
}

Drupal.Views.updatePreviewForm = function() {
  var url = $(this).attr('action');
  url = url.replace('nojs', 'ajax');

  $('input[type=submit], button', this).after('<span class="views-throbbing">&nbsp</span>');
  $(this).ajaxSubmit({
    url: url,
    data: { 'js': 1 },
    type: 'POST',
    success: Drupal.Views.Ajax.previewResponse,
    error: function(xhr) { $('span.views-throbbing').remove(); Drupal.Views.Ajax.handleErrors(xhr, url); },
    dataType: 'json'
  });

  return false;
}

Drupal.Views.updatePreviewFilterForm = function() {
  var url = $(this).attr('action');
  url = url.replace('nojs', 'ajax');

  $('input[name=q]', this).remove(); // remove 'q' for live preview.
  $(this).ajaxSubmit({
    url: url,
    data: { 'js': 1 },
    type: 'GET',
    success: Drupal.Views.Ajax.previewResponse,
    error: function(xhr) { $('span.views-throbbing').remove(); Drupal.Views.Ajax.handleErrors(xhr, url); },
    dataType: 'json'
  });

  return false;
}

Drupal.Views.updatePreviewLink = function() {
  var url = $(this).attr('href');
  url = url.replace('nojs', 'ajax');
  var intern_url = Drupal.Views.getPath(url);

  if (intern_url.substring(0, 17) != 'admin/build/views') {
    return true;
  }

  $(this).addClass('views-throbbing');
  $.ajax({
    url: url,
    data: 'js=1',
    type: 'POST',
    success: Drupal.Views.Ajax.previewResponse,
    error: function(xhr) { $(this).removeClass('views-throbbing'); Drupal.Views.Ajax.handleErrors(xhr, url); },
    dataType: 'json'
  });

  return false;
}

Drupal.behaviors.ViewsAjaxLinks = function() {
  // Make specified links ajaxy.
  $('a.views-ajax-link:not(.views-processed)').addClass('views-processed').click(function() {
    // Translate the href on the link to the ajax href. That way this degrades
    // into a nice, normal link.
    var url = $(this).attr('href');
    url = url.replace('nojs', 'ajax');

    // Turn on the hilite to indicate this is in use.
    $(this).addClass('hilite');

    // Disable the save and delete button.
    $('#edit-save').attr('disabled', 'true');
    $('#edit-delete').attr('disabled', 'true');

    $(this).addClass('views-throbbing');
    $.ajax({
      type: "POST",
      url: url,
      data: 'js=1',
      success: Drupal.Views.Ajax.ajaxResponse,
      error: function(xhr) { $(this).removeClass('views-throbbing'); Drupal.Views.Ajax.handleErrors(xhr, url); },
      dataType: 'json'
    });

    return false;
  });

  $('form.views-ajax-form:not(.views-processed)').addClass('views-processed').submit(function(arg) {
    // Translate the href on the link to the ajax href. That way this degrades
    // into a nice, normal link.
    var url = $(this).attr('action');
    url = url.replace('nojs', 'ajax');

//    $('input[@type=submit]', this).after('<span class="views-throbbing">&nbsp</span>');
    $(this).ajaxSubmit({
      url: url,
      data: { 'js': 1 },
      type: 'POST',
      success: Drupal.Views.Ajax.ajaxResponse,
      error: function(xhr) { $('span.views-throbbing').remove(); Drupal.Views.Ajax.handleErrors(xhr, url); },
      dataType: 'json'
    });

    return false;
  });

  // Bind the live preview to where it's supposed to go.

  $('form#views-ui-preview-form:not(.views-processed)')
    .addClass('views-processed')
    .submit(Drupal.Views.updatePreviewForm);

  $('div#views-live-preview form:not(.views-processed)')
    .addClass('views-processed')
    .submit(Drupal.Views.updatePreviewFilterForm)
    .find('input[type=submit], button').click(function() {
      $(this).after('<span class="views-throbbing">&nbsp</span>');
      // We have to actually tell it what button got clicked if we want
      // anything to be sent:
      this.form.clk = this;
    });

  $('div#views-live-preview a:not(.views-processed)')
    .addClass('views-processed')
    .click(Drupal.Views.updatePreviewLink);
}

/**
 * Sync preview display.
 */
Drupal.behaviors.syncPreviewDisplay = function() {
  $("#views-tabset a").click(function() {
    var href = $(this).attr('href');
    // Cut of #views-tabset.
    var display_id = href.substr(11);
    // Set the form element.
    $("#views-live-preview #preview-display-id").val(display_id);
  });
}

/**
 * Get rid of irritating tabledrag messages
 */
Drupal.theme.tableDragChangedWarning = function () {
  return '<div></div>';
}

/**
 * Display error in a more fashion way
 */
Drupal.Views.Ajax.handleErrors = function (xhr, path) {
  var error_text = '';

  if ((xhr.status == 500 && xhr.responseText) || xhr.status == 200) {
    error_text = xhr.responseText;

    // Replace all &lt; and &gt; by < and >
    error_text = error_text.replace("/&(lt|gt);/g", function (m, p) {
      return (p == "lt")? "<" : ">";
    });

    // Now, replace all html tags by empty spaces
    error_text = error_text.replace(/<("[^"]*"|'[^']*'|[^'">])*>/gi,"");

    // Fix end lines
    error_text = error_text.replace(/[\n]+\s+/g,"\n");
  }
  else if (xhr.status == 500) {
    error_text = xhr.status + ': ' + Drupal.t("Internal server error. Please see server or PHP logs for error information.");
  }
  else {
    error_text = xhr.status + ': ' + xhr.statusText;
  }

  alert(Drupal.t("An error occurred at @path.\n\nError Description: @error", {'@path': path, '@error': error_text}));
}


Drupal.behaviors.ViewsGroupedTableDrag = function(context) {
  var table_id = 'arrange';
  var table = $('table#arrange');
  var tableDrag = Drupal.tableDrag[table_id];

  if (tableDrag) {
    // Add a handler for when a row is swapped, update empty regions.
    tableDrag.row.prototype.onSwap = function(swappedRow) {
      checkEmptyRegions(table, this);
    };

    $('a.views-groups-remove-link')
      .addClass('views-processed')
      .click(function() {
        var id = $(this).attr('id').replace('views-remove-link-', '');
        var $row = $('#views-row-' + id);
        $row.hide().removeClass('draggable');
        $('#views-removed-' + id).attr('checked', true);
        tableDrag.rowObject = new tableDrag.row($row.get(0), 'mouse', tableDrag.indentEnabled, tableDrag.maxDepth, true);
        // If there is a draggable row after the row we just removed, swap us
        // down by one so that the empty region check does not see this row
        // and think that the region is empty.
        if ($row.next('tr').is('.draggable')) {
          tableDrag.rowObject.swap('after', $row.next('tr').get(0));
        }
        checkEmptyRegions(table, tableDrag.rowObject);
        return false;
      });

    // Add a handler so when a row is dropped, update fields dropped into new group.
    tableDrag.onDrop = function() {
      dragObject = this;
      // If this occurs row is in an empty group or its is the first of the group
      if ($(dragObject.rowObject.element).prev('tr').is('.group-message')) {
        // Get the previous group, this contains the group id
        var regionRow = $(dragObject.rowObject.element).prev('tr').get(0);
        var groupId = regionRow.className.replace(/([^ ]+[ ]+)*group-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
        // Then, update the select group value
        var selectGroupField = $('select.views-group-select', dragObject.rowObject.element);
        selectGroupField.val(groupId);
      }
    }

    var checkEmptyRegions = function(table, rowObject) {
      $('tr.group-message', table).each(function() {
        // If the dragged row is in this region, but above the message row, swap it down one space.
        if ($(this).prev('tr').get(0) == rowObject.element) {
          // Prevent a recursion problem when using the keyboard to move rows up.
          if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
            rowObject.swap('after', this);
          }
        }
        // This region has become empty
        if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) {
          $(this).removeClass('group-populated').addClass('group-empty');
        }
        // This region has become populated.
        else if ($(this).is('.group-empty')) {
          $(this).removeClass('group-empty').addClass('group-populated');
        }
      });
    };
  }
}
