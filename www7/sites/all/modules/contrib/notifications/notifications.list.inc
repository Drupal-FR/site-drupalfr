<?php
/**
 * @file
 * Drupal Notifications Framework - Default class file
 */

/**
 * List of subscriptions or subscription types
 */
class Notifications_Subscription_List implements Iterator {
  // The list type will be a unique name identifier, for static caching
  public $type = 'subscriptions';
  public $subscriptions = array();
  // First index will be 1, will save some problems
  protected $index = 0;
  // Template for multiple subscriptions of the same type
  public $template;
  // Header for tableselect
  public $header;
  // Whether instances have been loaded from db
  protected $loaded = FALSE;
  // Array indexed by subscription type
  protected $types = array();
  // Parameters common to all instances
  protected $instance_params = array();
  // Index for Iterator
  private $iterator_index = 0;

  /**
   * Construct with list of subscriptions
   */
  public function __construct($type = 'subscriptions') {
    $this->type = $type;
  }
  /**
   * Add subscription/s to the list
   */
  public function add($items) {
    if (is_array($items)) {
      foreach ($items as $item) {
        $this->add($item);
      }
    }
    elseif (is_object($items)) {
      if (is_a($items, 'Notifications_Subscription_List')) {
        $this->add($items->get_subscriptions());
      }
      elseif (is_a($items, 'Notifications_Subscription')) {
        $this->add_subscription($items);
      }
    }
    return $this;
  }
  /**
   * Get number of subscriptions
   */
  public function count() {
    return count($this->subscriptions);
  }
  /**
   * Load multiple subscriptions into the list
   */
  function load_multiple($sids = array(), $conditions = array()) {
    $this->instance_params = $conditions;
    if ($subs = entity_load('notifications_subscription', $sids, $conditions)) {
      $this->add($subs);
    }
    return $this;
  }

  /**
   * Add single subscription
   */
  function add_subscription($subscription) {
    $this->index++;
    $this->subscriptions[$this->index] = $subscription;
    return $this;
  }
  /**
   * Get array of subscription / subscription types
   */
  function get_subscriptions() {
    return $this->subscriptions;
  }
  /**
   * Get all of the subscriptions that are actual instances
   */
  function get_instances() {
    $this->load_instances();
    $list = array();
    foreach ($this->subscriptions as $index => $subscription) {
      if ($subscription->is_instance()) {
        $list[$index] = $subscription;
      }
      else {
        $list[$index] = $subscription->instance();
      }
    }
    return $list;
  }
  /**
   * Load stored instances for subscriptions in the list
   */
  function load_instances() {
    if (!$this->loaded) {
      foreach ($this->subscriptions as $key => $subscription) {
        if (!$subscription->is_stored() && ($stored = $subscription->get_instance())) {
          // Carry name over to the new one as it may be page specific
          if (!empty($subscription->name)) {
            $stored->name = $subscription->name;
          }
          $this->subscriptions[$key] = $stored;
        }
      }
      $this->loaded = TRUE;
    }
    return $this;
  }
  /**
   * Build instances for all with given parameters
   *
   * @todo Handle multiple instances with the same parameters
   */
  function build_instances($params = array()) {
    $this->instance_params = $params;
    foreach ($this->subscriptions as $index => $subscription) {
      if ($instance = $subscription->get_instance($params)) {
        $this->subscriptions[$index] = $instance;
      }
    }
  }
  /**
   * Set user to all subscriptions
   */
  function set_user($user) {
    $this->instance_params['uid'] = $user->uid;
    foreach ($this->get_subscriptions() as $subscription) {
      $subscription->set_user($user);
    }
    return $this;
  }
  /**
   * Set a property to all of the subscription instances
   */
  function set_all($name, $value) {
    $this->instance_params[$name] = $value;
    foreach ($this->get_subscriptions() as $subscription) {
      $subscription->$name = $value;
    }
    return $this;
  }
  /**
   * Get all subscription links
   *
   * @param $type
   *   - If 'subscribe', 'unsubscribe' will return just that type of links
   *   - If 'subscription' will return all subscriptions with either 'subscribe' or 'unsubscribe'
   *   - Type 'grouped' will return two nested lists: subscribe /unsubscribe
   */
  public function get_links($type = 'subscription', $options = array()) {
    if ($type == 'grouped') {
      $items = array();
      $groups = array('subscribe' => t('Subscribe to:'), 'unsubscribe' => t('Unsubscribe from:'));
      foreach ($groups as $group_type => $title) {
        if ($subscriptions = $this->get_links($group_type)) {
          $items[] = theme('item_list', array('items' => $subscriptions, 'title' => $title));
        }
      }
      return $items;
    }
    if ($type == 'subscribe' || $type == 'unsubscribe') {
      // Filter out the ones that don't match
      $subscriptions = $this->get_stored($type == 'unsubscribe');
    }
    else {
      $subscriptions = $this->get_instances();
    }
    $links = array();
    foreach ($subscriptions as $subscription) {
      $links[] = $subscription->get_link($type, $options);
    }
    return $links;
  }

  /**
   * Get only existing subscription instances (stored)
   */
  function get_stored($stored = TRUE) {
    $result = array();
    foreach ($this->get_instances() as $index => $subscription) {
      if ($subscription->is_stored() == $stored) {
        $result[$index] = $subscription;
      }
    }
    return $result;
  }

  /**
   * Check access for current user to manage multiple subscriptions
   */
  public function check_access($account = NULL) {
    $account = $account ? $account : $GLOBALS['user'];
    if (user_access('administer notifications', $account) || user_access('manage all subscriptions', $account)) {
      return TRUE;
    }
    else {
      // Check all subscriptions belong to the user
      foreach ($this->get_subscriptions() as $subscription) {
        if (isset($subscription->uid) && $subscription->uid != $account->uid) {
          return FALSE;
        }
      }
      return TRUE;
    }
  }

  /**
   * Get options form
   */
  public function get_form($type, $form, &$form_state) {
    $form['type'] = array('#type' => 'value', '#value' => $type);
    $form['subscription_list'] = array('#type' => 'value', '#value' => $this);
    switch ($type) {
      case 'checkboxes':
        // List of subscribe/unsubscribe checkboxes
        $form['operation'] = array('#type' => 'value', '#value' => 'update');
        $form['subscriptions'] = $this->element_checkboxes();
        $form['save'] = array('#type' => 'submit', '#value' => t('Update'));
        $destination = $_GET['q'];
        break;
      case 'unsubscribe':
      case 'delete':
        // Unsubscribe confirm form
        $form['operation'] = array('#type' => 'value', '#value' => 'unsubscribe');
        $form['subscriptions'] = $this->element_items();
        $form = confirm_form($form,
                      t('Are you sure you want to delete these subscriptions?'),
                      variable_get('notifications_frontpage', variable_get('site_frontpage', 'node')),
                      t('This action cannot be undone.'),
                      t('Delete all'), t('Cancel'));
        break;
    }
    return $form;
  }
  /**
   * Build subscription list instance from object or array of subscriptions
   */
  public static function build_list($items) {
    // PHP 5.3.0 only, we may want to construct another class extending this one
    $class = get_called_class();
    if (is_a($items, $class)) {
      return $items;
    }
    else {
      $list = new $class();
      $list->add($items);
      return $list;
    }
  }
  /**
   * Build subscription list from array of sids
   */
  public static function build_sids($sids) {
    // PHP 5.3.0 only, we may want to construct another class extending this one
    $class = get_called_class();
    $list = new $class();
    $list->load_multiple($sids);
    return $list;
  }
  /**
   * Build subscription list instance from form submission
   */
  public static function build_from_submission($form, &$form_state) {
    // The subscription object should be here, it may be a subscription type
    $subscriptions = $form_state['values']['subscription_list'];
    return $subscriptions;
  }
  /**
   * Validate form
   * @todo
   */
  public function form_validate($form, &$form_state) {

  }
  /**
   * Process form submission
   */
  public function form_submit($form, &$form_state) {
    $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
    if ($op == t('Cancel')) {
      $operation = 'cancel';
    }
    else {
      $operation = $form_state['values']['operation'];
    }
    return $this->form_operation($operation, $form, $form_state);
  }
  /**
   * Form operation
   */
  protected function form_operation($operation, $form, &$form_state) {
    switch ($operation) {
      case 'cancel':
        break;
      case 'update':
        $options = $form_state['values']['subscriptions'];
        $subscribe = array_filter($options);
        $created = $deleted = 0;
        foreach ($this->get_subscriptions() as $index => $subscription) {
          $status = in_array($index, $subscribe);
          if (!$subscription->is_stored() && $status) {
            $subscription->save();
            $created++;
          }
          elseif ($subscription->is_stored() && !$status) {
            $subscription->delete();
            $deleted++;
          }
        }
        if ($created || $deleted) {
          drupal_set_message(t('The subscriptions have been updated: created @created, deleted @deleted.', array('@created' => $created, '@deleted' => $deleted)));
        }
        break;
      case 'unsubscribe':
      case 'delete':
        $count = 0;
        foreach ($this->get_instances() as $subscription) {
          $subscription->delete();
          $count++;
        }
        $form_state['redirect'] = variable_get('notifications_frontpage', '<front>');
        drupal_set_message(format_plural($count, 'A subscription has been deleted.', '@count subscriptions have been deleted.'));
        break;
    }
  }
  /**
   * Get elements
   */
  public function get_elements($type) {
    switch ($type) {
      case 'items':
        return $this->element_items();
      case 'checkboxes':
      default:
        return $this->element_checkboxes();
    }
  }
  /**
   * Subform with subscription options so it can be reused for a fieldset on a bigger form
   *
   * Was: notifications_object_options_subform($subscriptions, $buttons = TRUE)
   *
   * @param $subscriptions
   *   List of subscription objects
   * @param $buttons
   *   Whether to add buttons to the fieldset
   */
  function options_subform($buttons = TRUE) {
    $form['subscriptions'] = $this->options_fieldset(TRUE);
    $form['subscriptions'] += array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    if ($buttons) {
      $form['subscriptions']['submit'] = array('#type' => 'submit', '#value' => t('Update'));
      // If full form, redirect so the full page which may have subscription links is updated
      $form['#redirect'] = $_GET['q'];
    }
    $form['#submit'][] = 'notifications_subscriptions_options_form_submit';
    return $form;
  }

  /**
   * Build fieldset with subscription options
   *
   * Was: notifications_object_options_fieldset($subscriptions, $title = FALSE)
   */
  function element_checkboxes($element = array()) {
    $options = $defaults = array();
    // Note: the options array cannot have a 0 key, but we shouldn't have a 0 subscription either
    // http://api.drupal.org/api/drupal/developer--topics--forms_api_reference.html/6#checkboxes
    foreach ($this->get_instances() as $index => $subscription) {
      $options[$index] = $subscription->get_name();
      if ($subscription->is_stored()) {
        $defaults[] = $index;
      }
    }
    return $element + array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $defaults,
    );
  }
  /**
   * Build fieldset with subscription names
   */
  function element_items($element = array()) {
    foreach ($this->get_instances() as $index => $subscription) {
      $items[] = $subscription->get_name();
    }
    return $element + array(
      '#title' => t('Subscription list'),
      '#theme' => 'item_list',
      '#items' => $items,
    );
  }
  /**
   * Filter out subscription types for a given option
   */
  public function filter_option($option, $value = TRUE) {
    foreach ($this->subscriptions as $index => $subscription) {
      if (notifications_subscription_type($subscription->type, $option) != $value) {
        unset($this->subscriptions[$index]);
      }
    }
    return $this;
  }

  /**
   * Implementation of iterator interface
   */
  public function rewind() {
    $this->iterator_index = 0;
  }
  public function current() {
    $k = array_keys($this->subscriptions);
    return $this->subscriptions[$k[$this->iterator_index]];
  }
  public function key() {
    $k = array_keys($this->subscriptions);
    return $k[$this->iterator_index];
  }
  public function next() {
    $k = array_keys($this->subscriptions);
    if (isset($k[++$this->iterator_index])) {
      return $this->subscriptions[$k[$this->iterator_index]];
    }
    else {
      return FALSE;
    }
  }
  public function valid() {
    $k = array_keys($this->subscriptions);
    return isset($k[$this->iterator_index]);
  }
}

/**
 * Loadable collection of subscriptions for tablesel ect
 */
class Notifications_Subscription_Table extends Notifications_Subscription_List {
  public $conditions = array();
  public $base_path = 'notifications/subscription';
  public $pager = 50;
  /**
   * Set conditions and remove fields if in the header
   */
  function set_conditions($conditions = array()) {
    $this->conditions = $conditions;
    foreach (array_keys($conditions) as $id) {
      if (isset($this->header[$id])) {
        unset($this->header[$id]);
      }
    }
    return $this;
  }
  /**
   * Produce a select table with all these subscriptions
   */
  function table_select() {
    $options = array();
    foreach ($this->get_subscriptions() as $subscription) {
      $options[$subscription->sid] = $this->subscription_fields($subscription);
    }
    $table = array(
      '#type' => 'tableselect',
      '#header' => $this->get_header(),
      '#options' => $options,
      '#empty' => t('No subscriptions available.'),
    );
    return $table;
  }
  /**
   * Produce simple table with all these subscriptions
   */
  function table_list() {
    $options = array();
    foreach ($this->get_subscriptions() as $subscription) {
      $options[$subscription->sid] = $this->subscription_fields($subscription);
    }
    $table = array(
      '#theme' => 'table',
      '#header' => $this->get_header(),
      '#rows' => $options,
      '#empty' => t('No subscriptions available.'),
    );
    return $table;
  }
  /**
   * Fill in fields for a subscription
   */
  function subscription_fields($subs) {
    $send_methods = messaging_method_info(NULL, 'name');
    $send_intervals = notifications_send_intervals();
    $send_intervals[-1] = t('Scheduled');
    $fields = array();
    foreach (array_keys($this->header) as $index) {
      switch ($index) {
        case 'sid':
          $value = array(
            'data' => array(
              '#type' => 'link',
              '#title' => $subs->sid,
              '#href' => $this->base_path . '/' . $subs->sid,
            ),
          );
          break;
        case 'title':
          $value = $subs->get_title();
          break;
        case 'name':
          $value = $subs->get_name();
          break;
        case 'type':
          $type_list = notifications_subscription_type(NULL, 'title');
          $value = $type_list[$subs->type];
          break;
        case 'status':
          $status_list = Notifications_Subscription::status_list();
          $value = $status_list[$subs->status];
          break;
        case 'created':
          $value  = format_date($subs->created, 'short');
          break;
        case 'uid': // User
          $value = theme('username', array('account' => $subs->get_user()));
          break;
        case 'operations':
          $value = $this->field_operations($subs);
          break;
        case 'send_method':
          $value = isset($send_methods[$subs->send_method]) ? $send_methods[$subs->send_method] : $subs->send_method;
          break;
        case 'send_interval':
          $value = isset($send_intervals[$subs->send_interval]) ? $send_intervals[$subs->send_interval] : $subs->send_interval;
          break;
        default:
          $value = isset($subs->$index) ? check_plain($subs->$index) : '--';
          break;
      }
      $fields[$index] = $value;
    }
    return $fields;
  }
  /**
   * Print username or destination for anonymous users
   *
   * @todo Check the part for
   */
  function field_destination($subs) {
    if ($user = $subs->get_user()) {
      $username = theme('username', array('account' => $user));
    }
    else {
      // Anonymous subscription, print destination instead
      $dest = $sub->get_destination();
      $username = $dest->address_name() . ' ' . l($dest->format_address(FALSE), 'notifications/destination/' . $sub->mdid . '/manage') ;
    }
  }
  /**
   * Field operations
   */
  function field_operations($subs) {
    $destination = drupal_get_destination();
    // Build a list of all the accessible operations for the current subscription.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' =>  $this->base_path . '/' . $subs->sid . '/edit',
      'query' => $destination,
    );

    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => $this->base_path . '/' . $subs->sid . '/delete',
      'query' => $destination,
    );

    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      return array(
        'data' => array(
          '#theme' => 'links',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      return array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }
  }
  /**
   * Get query for selection
   */
  function query_select() {
    // Query data with $conditions and filters
    $query = db_select('notifications_subscription', 's')->extend('PagerDefault')->extend('TableSort');
    foreach ($this->conditions as $field => $value) {
      $query->condition('s.' . $field, $value);
    }
    $query
      ->fields('s', array('sid'))
      ->limit(50)
      ->orderByHeader($this->header);
    return $query;
  }
  /**
   * Actually load all the subscriptions
   */
  function query_load() {
    if ($sids = $this->query_select()->execute()->fetchCol()) {
      $subscriptions = entity_load('notifications_subscription', $sids);
      $this->add($subscriptions);
    }
    return $this;
  }

  /**
   * Set fields as header
   */
  function set_header($header = NULL) {
    if (isset($header)) {
      $this->header = $header;
    }
    else {
      $this->header = $this->get_header();
    }
    return $this;
  }
  /**
   * Set default header
   */
  function get_header() {
    return isset($this->header) ? $this->header : array(
      'sid' => array('data' => t('Id'), 'field' => 's.sid'),
      'type' => array('data' => t('Type'), 'field' => 's.type'),
      'status' => array('data' => t('Status'), 'field' => 's.status'),
      'uid' => array('data' => t('User'), 'field' => 's.uid'),
      'name' => t('Description'),
      'created' => array('data' => t('Created'), 'field' => 's.created', 'sort' => 'desc'),
    );
  }

}
