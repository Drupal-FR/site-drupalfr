<?php

class MenutrailbypathConfig {
  const PREFIX = 'menu_trail_by_path_';

  public static function get($variable) {
    switch ($variable) {
      case 'menu_trail_active_on_admin_paths':
        return variable_get(self::PREFIX . $variable, FALSE);
        break;
      case 'menu_trail_active_on_ajax_requests':
        return variable_get(self::PREFIX . $variable, FALSE);
        break;
      case 'breadcrumb_handling':
        return variable_get(self::PREFIX . $variable, TRUE);
        break;
      case 'breadcrumb_active_on_admin_paths':
        return variable_get(self::PREFIX . $variable, FALSE);
        break;
      case 'breadcrumb_active_on_ajax_requests':
        return variable_get(self::PREFIX . $variable, FALSE);
        break;
      case 'breadcrumb_display_current_page':
        return variable_get(self::PREFIX . $variable, 'no');
        break;
    }

    return NULL;
  }
}
