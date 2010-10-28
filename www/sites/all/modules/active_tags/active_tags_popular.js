 // $Id: active_tags_popular.js,v 1.1.2.11 2010/02/22 13:04:15 dragonwize Exp $

Drupal.behaviors.taggerPopular = function(context) {
  jQuery.each(Drupal.settings['active_tags_popular'], function(i, v) {
    var wrapper = $(v);
    if (wrapper.length == 1 && !wrapper.hasClass('active-tags-pop-processed')) {
      activeTagsPopularActivate(v);
      wrapper.addClass('active-tags-pop-processed');
    }
  });
}

function activeTagsPopularActivate(context) {
  var vid = context.substring(20,context.lastIndexOf('-'));
  var wrapper = $(context);
  $.ajax({
    type: "GET",
    url: Drupal.settings['active_tags_popular_callback'] + '/' + vid,
    dataType: 'json',
    success: function (matches) {
      var tagArea = Drupal.theme('activeTagPopular', context, matches);
      wrapper.after(tagArea);
      var str = wrapper.find('input.form-text').val();
      var popTags = wrapper.next().children('.tag-popular');
      popTags.children().filter(function(index) {
        return str.indexOf($(this).text()) >= 0;
      }).parent().remove();
      popTags.children('.add-tag-popular').click(function() {
        activeTagsAdd(context, $(this).prev().text());
        activeTagsUpdate(context);
        $(this).parent().remove();
      });
    },
    error: function(xmlhttp) {
      alert(Drupal.ahahError(xmlhttp, Drupal.settings['active_tags_popular_callback']));
    }
  });
}

/**
 * Theme a popular tag.
 */
Drupal.theme.prototype.activeTagPopular = function(context, tags) {
  var content = '';

  if (tags.length) {
    content = '<div class="pop-tags">' + Drupal.t('Add popular tags: ');
    jQuery.each(tags, function(i, v) {
      var tagitem = '<div class="tag-popular"><span class="tag-text">' + v + '</span><span class="add-tag-popular">+</span></div> ';
      content += tagitem;
    });
    content += '</div>';
  }

  return content;
};
