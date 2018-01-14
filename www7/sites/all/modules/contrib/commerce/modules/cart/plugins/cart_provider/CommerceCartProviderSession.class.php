<?php

/**
 * Defines a session array based cart provider.
 */
class CommerceCartProviderSession implements CommerceCartProviderInterface {

  const COMPLETED = 'commerce_cart_completed_orders';
  const CART = 'commerce_cart_orders';

  /**
   * {@inheritdoc}
   */
  public function cartDelete($order_id = NULL, $completed = FALSE) {
    $key = $completed ? self::COMPLETED : self::CART;

    if (!empty($_SESSION[$key])) {
      if (!empty($order_id)) {
        $_SESSION[$key] = array_diff($_SESSION[$key], array($order_id));
      }
      else {
        unset($_SESSION[$key]);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function cartExists($order_id = NULL, $completed = FALSE) {
    $key = $completed ? self::COMPLETED : self::CART;

    // If an order was specified, look for it in the array.
    if (!empty($order_id)) {
      return !empty($_SESSION[$key]) && in_array($order_id, $_SESSION[$key]);
    }
    else {
      // Otherwise look for any value.
      return !empty($_SESSION[$key]);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function cartOrderIds($completed = FALSE) {
    $key = $completed ? self::COMPLETED : self::CART;
    return empty($_SESSION[$key]) ? array() : $_SESSION[$key];
  }

  /**
   * {@inheritdoc}
   */
  public function cartSave($order_id, $completed = FALSE) {
    $key = $completed ? self::COMPLETED : self::CART;

    if (empty($_SESSION[$key])) {
      $_SESSION[$key] = array($order_id);
    }
    elseif (!in_array($order_id, $_SESSION[$key])) {
      $_SESSION[$key][] = $order_id;
    }
  }

}
