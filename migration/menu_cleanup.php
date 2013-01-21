<?php

// Remove the fantom link about user listing and unwanted links.
_menu_delete_item(menu_link_load(1209), 1);
_menu_delete_item(menu_link_load(1954), 1);

// Reorder the main menu items. Extract the planete mlid first.
$planete_mlid = db_query("SELECT mlid FROM menu_links WHERE router_path = 'Planète' AND menu_name = 'main-menu';")->fetchAssoc();
$i = 0;
foreach (array(457, 27, $planete_mlid['mlid'], 1125, 17) as $mlid) {
  $item = menu_link_load($mlid);
  $item['weight'] = $i++;
  menu_link_save($item);
}

// Move the ressources link in the navigation menu.
$ressources_mlid = db_query("SELECT mlid FROM menu_links WHERE link_title = 'Ressources'")->fetchAssoc();
if ($ressources_mlid) {
  $item = menu_link_load($ressources_mlid["mlid"]);
  $item["menu_name"] = "navigation";
  $item["p1"] = $ressources_mlid["mlid"];
  menu_link_save($item);
}

// Move the Drupalcamp link in the navigation menu.
$drupalcamp_mlid = db_query("SELECT mlid FROM menu_links WHERE link_title = 'Drupalcamp' AND menu_name = 'main-menu'")->fetchAssoc();
if ($drupalcamp_mlid) {
  $item = menu_link_load($drupalcamp_mlid["mlid"]);
  $item["menu_name"] = "navigation";
  $item["p1"] = $drupalcamp_mlid["mlid"];
  menu_link_save($item);
}

// Reset some menu_items to have them translated
menu_reset_item(menu_link_load(22));
menu_reset_item(menu_link_load(39));
menu_reset_item(menu_link_load(1213));
menu_reset_item(menu_link_load(1214));

// Clear the menu caches.
menu_cache_clear_all();

// Resort the menu items in the navigation menu.
// Logout link.
$logout_mlid = db_query("SELECT mlid FROM menu_links WHERE link_title = 'Log out' AND menu_name = 'navigation'")->fetchAssoc();
if ($logout_mlid) {
  $item = menu_link_load($logout_mlid["mlid"]);
  $item["weight"] = 0;
  menu_link_save($item);
}

// Content creation link.
$content_mlid = db_query("SELECT mlid FROM menu_links WHERE link_path = 'node/add' AND menu_name = 'navigation';")->fetchAssoc();
if ($content_mlid) {
  $item = menu_link_load($content_mlid["mlid"]);
  $item["weight"] = 1;
  menu_link_save($item);
}

