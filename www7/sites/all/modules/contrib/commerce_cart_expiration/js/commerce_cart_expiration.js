var commerce_cart_expiration_sent = false;

(function ($) {
  Drupal.behaviors.commerce_cart_expiration = {
    attach: function (context, settings) {
      if (context == document) {
        var expire = settings.commerce_cart_expiration.expire_in;
        var url_ajax = settings.commerce_cart_expiration.url_ajax;
        var url_redirect = settings.commerce_cart_expiration.url_redirect;

        // Get all cart expiration blocks.
        var blocks = $('.block-commerce-cart-expiration');

        blocks.each(function(index, block) {
          commerceCartExpirationTick($(this), expire, url_ajax, url_redirect);
        });
      }
    }
  };

  /**
   * Refreshes the expiration time.
   */
  function commerceCartExpirationTick(block, expire, url_ajax, url_redirect) {
    var el = block.find('.time-left');
    el.html(commerceCartExpirationFormatInterval (expire, 2));

    if (expire <= 0) {
      commerceCartExpirationExpired(url_ajax, url_redirect);
      return;
    }
    expire--;

    setTimeout(function() {
      commerceCartExpirationTick(block, expire, url_ajax, url_redirect);
    }, 1000);
  }

  /**
   * Formats a time interval with the requested granularity.
   *
   * Aequivalent to format_interval().
   */
  function commerceCartExpirationFormatInterval (interval, granularity) {
    granularity = typeof granularity !== 'undefined' ? granularity : 2;
    output = '';

    while (granularity > 0) {
      var value = 0;
      if (interval >= 31536000) {
        value = 31536000;
        output += (output.length ? ' ' : '') + Drupal.formatPlural(Math.floor(interval / value), '1 year', '@count years');
      }
      else if (interval >= 2592000) {
        value = 2592000;
        output += (output.length ? ' ' : '') + Drupal.formatPlural(Math.floor(interval / value), '1 month', '@count months');
      }
      else if (interval >= 604800) {
        value = 604800;
        output += (output.length ? ' ' : '') + Drupal.formatPlural(Math.floor(interval / value), '1 week', '@count weeks');
      }
      else if (interval >= 86400) {
        value = 86400;
        output += (output.length ? ' ' : '') + Drupal.formatPlural(Math.floor(interval / value), '1 day', '@count days');
      }
      else if (interval >= 3600) {
        value = 3600;
        output += (output.length ? ' ' : '') + Drupal.formatPlural(Math.floor(interval / value), '1 hour', '@count hours');
      }
      else if (interval >= 60) {
        value = 60;
        output += (output.length ? ' ' : '') + Drupal.formatPlural(Math.floor(interval / value), '1 min', '@count min');
      }
      else if (interval >= 1) {
        value = 1;
        output += (output.length ? ' ' : '') + Drupal.formatPlural(Math.floor(interval / value), '1 sec', '@count sec');
      }

      interval %= value;
      granularity--;
    }

    return output.length ? output : Drupal.t('0 sec');
  }

  /**
   * Calls drupal and tries to delete the order.
   */
  function commerceCartExpirationExpired(url_ajax, url_redirect) {
    if (!commerce_cart_expiration_sent) {
      $.ajax({
        url: url_ajax,
        type: 'GET',
        success: function(response) {
          if (response.status) {
            window.location = url_redirect;
          }
        },
      });
      commerce_cart_expiration_sent = true;
    }
  }
})(jQuery);
