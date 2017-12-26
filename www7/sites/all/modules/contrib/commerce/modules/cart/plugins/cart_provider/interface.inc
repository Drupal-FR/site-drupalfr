<?php

/**
 * @file
 * Interface for cart providers.
 */
interface CommerceCartProviderInterface {

  /**
   * Deletes either all order IDs or a specific order ID from a carts array.
   *
   * @param $order_id
   *   The order ID to remove from the array or NULL to delete the variable.
   * @param $completed
   *   Boolean indicating whether or not the operation should delete from the
   *   completed orders array instead of the active cart orders array.
   */
  public function cartDelete($order_id = NULL, $completed = FALSE);

  /**
   * Checks to see if any cart order ID or a specific cart order ID exists.
   *
   * @param $order_id
   *   Optionally specify an order ID to look for; defaults to NULL.
   * @param $completed
   *   Boolean indicating whether or not the operation should look in the
   *   completed orders array instead of the active cart orders array.
   *
   * @return
   *   Boolean indicating whether or not a cart order ID exists for the given
   *   parameters.
   */
  public function cartExists($order_id = NULL, $completed = FALSE);

  /**
   * Returns an array of cart order IDs stored.
   *
   * @param $completed
   *   Boolean indicating whether or not the operation should retrieve the
   *   completed orders array instead of the active cart orders array.
   *
   * @return
   *   An array of applicable cart order IDs or an empty array if none exist.
   */
  public function cartOrderIds($completed = FALSE);

  /**
   * Saves an order ID.
   *
   * @param $order_id
   *   The order ID to save to the array.
   * @param $completed
   *   Boolean indicating whether or not the operation should save to the
   *   completed orders array instead of the active cart orders array.
   */
  public function cartSave($order_id, $completed = FALSE);

}
