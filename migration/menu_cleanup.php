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
  $item["menu_name"] = "user-menu";
  $item["p1"] = $ressources_mlid["mlid"];
  menu_link_save($item);
}

// Move the Drupalcamp link in the navigation menu.
$drupalcamp_mlid = db_query("SELECT mlid FROM menu_links WHERE link_title = 'Drupalcamp' AND menu_name = 'main-menu'")->fetchAssoc();
if ($drupalcamp_mlid) {
  $item = menu_link_load($drupalcamp_mlid["mlid"]);
  $item["menu_name"] = "user-menu";
  $item["p1"] = $drupalcamp_mlid["mlid"];
  menu_link_save($item);
}

// Reset some menu_items to have them translated
menu_reset_item(menu_link_load(22));
menu_reset_item(menu_link_load(39));
menu_reset_item(menu_link_load(1213));
menu_reset_item(menu_link_load(1214));

// Reset some menu items from the navigation menu.
// Recent message.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'navigation' AND link_title = 'Messages récents'")->fetchAssoc();
menu_reset_item(menu_link_load($mlid['mlid']));

// Recent posts.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'navigation' AND link_title = 'Recent posts'")->fetchAssoc();
menu_reset_item(menu_link_load($mlid['mlid']));

// Log out.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'navigation' AND link_title = 'Log out'")->fetchAssoc();
menu_reset_item(menu_link_load($mlid['mlid']));

// Clear the menu caches.
menu_cache_clear_all();

// Build the user menu for navigation.
// 1. Log out.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'user-menu' AND link_title = 'Log out'")->fetchAssoc();
if ($mlid) {
  $item = menu_link_load($mlid["mlid"]);
  $item["weight"] = -15;
  $item["p1"] = $mlid["mlid"];
  menu_link_save($item);
}

// 2. My account.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'user-menu' AND link_title = 'User account'")->fetchAssoc();
if ($mlid) {
  $item = menu_link_load($mlid["mlid"]);
  $item["weight"] = -14;
  $item["p1"] = $mlid["mlid"];
  menu_link_save($item);
}

// 3. Drupalcamp.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'user-menu' AND link_title = 'Drupalcamp'")->fetchAssoc();
if ($mlid) {
  $item = menu_link_load($mlid["mlid"]);
  $item["weight"] = -13;
  $item["p1"] = $mlid["mlid"];
  menu_link_save($item);
}

// 4. Ressources.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'user-menu' AND link_title = 'Ressources'")->fetchAssoc();
if ($mlid) {
  $item = menu_link_load($mlid["mlid"]);
  $item["weight"] = -12;
  $item["p1"] = $mlid["mlid"];
  menu_link_save($item);
}

// 5. Add content.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'navigation' AND link_title = 'Add content'")->fetchAssoc();
if ($mlid) {
  $item = menu_link_load($mlid["mlid"]);
  $item["weight"] = -11;
  $item['menu_name'] = 'user-menu';
  $item["p1"] = $mlid["mlid"];
  menu_link_save($item);
}

// 6. Recent messages.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'navigation' AND link_title = 'Recent posts'")->fetchAssoc();
if ($mlid) {
  $item = menu_link_load($mlid["mlid"]);
  $item["weight"] = -10;
  $item['menu_name'] = 'user-menu';
  $item["p1"] = $mlid["mlid"];
  menu_link_save($item);
}

// 7. Job offers moderation.
$mlid = db_query("SELECT mlid FROM menu_links WHERE menu_name = 'navigation' AND link_title = 'Modération des offres'")->fetchAssoc();
if ($mlid) {
  $item = menu_link_load($mlid["mlid"]);
  $item["weight"] = -9;
  $item['menu_name'] = 'user-menu';
  $item["p1"] = $mlid["mlid"];
  menu_link_save($item);
}

menu_cache_clear();
