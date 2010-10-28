// Original text hint code from http://remysharp.com/2007/01/25/jquery-tutorial-text-box-hints/

jQuery.fn.texthint = function (title) {
  return this.each(function () {
    // get jQuery version of 'this'
    var t = jQuery(this);
    // only apply logic if the element has the attribute
    if (title) {
      t.attr('size', '16');
      t.css('color', '#999');
      // on blur, set value to title attr if text is blank
      t.blur(function () {
        if (t.val() == '') {
          t.val(title);
          t.css('color', '#999');
        }
      });
      // on focus, set value to blank if current value
      // matches title attr
      t.focus(function () {
        if (t.val() == title) {
          t.val('');
          t.css('color', '#000');
        }
      });

      // clear the pre-defined text when form is submitted
      t.parents('form:first()').submit(function() {
        if (t.val() == title) {
          t.val('');
          t.css('color', '#000');
        }
      });

      // now change all inputs to title
      t.blur();
    }
  });
}

// Global killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('input#edit-search-theme-form-1, input#edit-search-block-form-keys').texthint('<rechercher>');
    // Hide the search button.
    if ($('#search').length > 0) {
      $('#search #edit-submit').hide();
    }
  });
}

