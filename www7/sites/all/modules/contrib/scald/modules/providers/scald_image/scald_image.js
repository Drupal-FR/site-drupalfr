(function ($) {
  Drupal.behaviors.scaldImage = {
    attach: function (context, settings) {
      if (typeof Drupal.dndck4 !== 'undefined') {
        Drupal.dndck4.addOption('txtLink', 'image', 'atom', 'scald_image', function (infoTab, dialogDefinition) {
          infoTab.add({
            id: 'txtLink',
            type: 'text',
            label: Drupal.t('Link'),
            // "Link" edits the 'link' property in the options JSON string.
            setup: function (widget) {
              var options = JSON.parse(widget.data.options);
              if (options.link) {
                this.setValue(decodeURIComponent(options.link));
              }
            },
            commit: function (widget) {
              // Copy the current options into a new object,
              var options = JSON.parse(widget.data.options);
              var value = this.getValue();
              if (value != '') {
                options.link = encodeURIComponent(value);
              }
              else {
                delete options.link;
              }
              widget.setData('options', JSON.stringify(options));
            }
          });
        });
      }
    }
  };
})(jQuery);
