<?php
// $Id$

/**
 * Filter handler to accept a user id to check for nodes that
 * user posted or created a revision on.
 */
class views_handler_argument_node_uid_revision extends views_handler_argument_comment_user_uid {
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    $this->query->add_where(0, "$this->table_alias.uid = %d OR ((SELECT COUNT(*) FROM {node_revisions} nr WHERE nr.uid = %d AND nr.nid = $this->table_alias.nid) > 0)", $this->argument, $this->argument);
  }
}
