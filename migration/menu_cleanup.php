<?php

// Remove the fantom link about user listing and unwanted links.
_menu_delete_item(menu_link_load(1209), 1);
_menu_delete_item(menu_link_load(1954), 1);

// Move the ressources link in the navigation menu.
$ressources_mlid = db_query("SELECT mlid FROM menu_links WHERE link_title = 'Ressources'")->fetchAssoc();
if ($ressources_mlid) {
  $item = menu_link_load($ressources_mlid["mlid"]);
  $item["menu_name"] = "navigation";
  $item["p1"] = $ressources_mlid["mlid"];
  menu_link_save($item);
}
