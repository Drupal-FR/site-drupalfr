(function ($) {

  /** ********************************************************************
   * INIT
   ** ***************************************************************** */

  Drupal.dfrtheme = Drupal.dfrtheme || {}
  Drupal.settings.dfrtheme = Drupal.settings.dfrtheme || {}


  /** ********************************************************************
   * FUNCTIONS
   ** ***************************************************************** */

  // Enlarge Your Click Zone : allow link'parent element to be clickable too.
  Drupal.dfrtheme.enlargeYourClick = function(selector) {
    $(selector).once('enlargeYourClick').click(function(e) {
      // Don't handle if user click on a link, or if he click with mouse wheel.
      if (e.target.tagName != "A" && e.button != 1) {
        var dest = $(this).find('a:first').attr('href');
        if (dest) {
          window.location = dest;
        }
      }
    })
    .css({cursor: 'pointer'});
  }; // Drupal.dfrtheme.enlargeYourClick

  Drupal.dfrtheme.expandPlanetUser = function(user) {
    var $this = user;
    $('img', user).click(function(e) {
      e.preventDefault();
      user.removeClass('expanded');
      $(this).parents('.field-name-field-planete-user').toggleClass('expanded');
    });
    $(document).mouseup(function (e) {
      if ($this.has(e.target).length === 0) {
        user.removeClass('expanded');
      }
      $(document).mouseup(null);
    });
  };

  Drupal.dfrtheme.emploiCarousel = function(wrapper) {
    // Var.
    var pos = 0,
        container = wrapper.find('.view-content').css('position', 'relative'),
        eltNum = container.children().length,
        eltVisible = 3,
        posUpdate = function(p) {
          $('.nav', wrapper).removeClass('inactive');
          if (p <= 0) {
            p = 0;
            $('.nav-prev', wrapper).addClass('inactive');
          }
          if (p >= (eltNum - eltVisible)) {
            p = eltNum - eltVisible;
            $('.nav-next', wrapper).addClass('inactive');
          }
          return p;
        };
    Drupal.settings.dfrtheme.emploiCarouselEltWidth = container.children().eq(1).outerWidth(1);

    if (eltNum <= eltVisible) {
      return;
    }

    // Add nav links.
    wrapper.prepend('<div class="nav-links"><span class="nav nav-prev"></span><span class="nav nav-next"></span></div>');

    // Init.
    posUpdate(pos);

    // Handle nav click.
    $('.nav', wrapper).click(function() {
      // Get direction.
      ($(this).hasClass('nav-prev')) ? pos-- : pos++;
      // Update position index.
      pos = posUpdate(pos);
      // Animate content.
      container.animate({marginLeft: -(pos * Drupal.settings.dfrtheme.emploiCarouselEltWidth)})
    })
  };


  /** ********************************************************************
   * BEHAVIORS
   ** ***************************************************************** */

  Drupal.behaviors.dfrtheme = {
    attach: function(context, settings) {
      /*
        var kKeys = [];
        function Kpress(e) {
          kKeys.push(e.keyCode);
          if (kKeys.toString().indexOf("38,38,40,40,37,39,37,39,66,65") >= 0) {
            jQuery(this).unbind('keydown', Kpress);
            kExec();
          }
        }
        function kExec() {
          window.location = "http://hsmaker.com/harlemshake.asp?url=http%3A%2F%2Fdrupalfr.org";
        }
        jQuery(document).keydown(Kpress);
      */

      // <html> js class.
      $('html').removeClass('no-js'); // addClass('js') is already done in misc/drupal.js.

      Drupal.dfrtheme.expandPlanetUser($('.field-name-field-planete-user'));

      // Emploi carousel.
      $('#block-views-offres-block-1').once('emploi-carousel', function() {
        Drupal.dfrtheme.emploiCarousel($(this));
      });
      $(window).resize(function() {
        // Because item width is fluid, we need to recalculate on widow resize.
        Drupal.settings.dfrtheme.emploiCarouselEltWidth = $('#block-views-offres-block-1 .view-content').children().eq(1).outerWidth(1)
      });
    }
  }; // Drupal.behaviors.dfrtheme

})(jQuery);
