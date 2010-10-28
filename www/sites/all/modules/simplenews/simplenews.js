// $Id: simplenews.js,v 1.1.2.2 2008/09/30 20:07:51 sutharsan Exp $

/**
 * Set text of Save button dependent on the selected send option.
 */
Drupal.behaviors.simplenewsCommandSend = function (context) {
  var simplenewsSendButton = function () {
    switch ($(".simplenews-command-send :radio:checked").val()) {
      case '0':
        $('#edit-submit').attr({value: Drupal.t('Save')});
        break;
      case '1':
        $('#edit-submit').attr({value: Drupal.t('Save and send')});
        break;
      case '2':
        $('#edit-submit').attr({value: Drupal.t('Save and send test')});
        break;
    }
  }
  
  // Update send button at page load and when a send option is selected.
  $(function() { simplenewsSendButton(); });
  $(".simplenews-command-send").click( function() { simplenewsSendButton(); });
  
  
}
