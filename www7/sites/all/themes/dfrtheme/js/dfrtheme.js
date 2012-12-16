(function ($) {

  /** ********************************************************************
   * INIT
   ** ***************************************************************** */

  Drupal.dfrtheme = Drupal.dfrtheme || {}
  Drupal.settings.dfrtheme = Drupal.settings.dfrtheme || {}


  /** ********************************************************************
   * FUNCTIONS
   ** ***************************************************************** */

  Drupal.dfrtheme.externalLinks = function(){
    $('a').each(function() {
     var a = new RegExp('/' + window.location.host + '/');
     if(!a.test(this.href)) {
         $(this).attr('target', '_blank');
      }
    });
  } 

  // Enlarge Your Click Zone : allow link'parent element to be clickable too
  Drupal.dfrtheme.enlargeYourClick = function(selector){
    $(selector).once('enlargeYourClick').click(function(e){
      // don't handle if user click on a link, or if he click with mouse wheel
      if (e.target.tagName != "A" && e.button != 1) {
        var dest = $(this).find('a:first').attr('href');
        if (dest) {window.location = dest};
      }
    })
    .css({cursor: 'pointer'});
  } // Drupal.dfrtheme.enlargeYourClick

  Drupal.dfrtheme.expandPlanetUser = function(user){
    var $this = user;
    $('img', user).click(function(e){
      e.preventDefault();
      user.removeClass('expanded');
      $(this).parents('.field-name-field-planete-user').toggleClass('expanded');
    })
    $(document).mouseup(function (e) {
      if ($this.has(e.target).length === 0) {
        user.removeClass('expanded');
      }
      $(document).mouseup(null);
    });
  }

  /** ********************************************************************
   * BEHAVIORS
   ** ***************************************************************** */

  Drupal.behaviors.dfrtheme = {
    attach: function(context, settings) {
      //$.extend(true, Drupal.settings, settings);

      // <html> js class
      $('html').removeClass('no-js') // addClass('js') is already done in misc/drupal.js

      // Open external links in a new window
      Drupal.dfrtheme.externalLinks();

      Drupal.dfrtheme.expandPlanetUser($('.field-name-field-planete-user'));

    }
  } // Drupal.behaviors.dfrtheme

})(jQuery);