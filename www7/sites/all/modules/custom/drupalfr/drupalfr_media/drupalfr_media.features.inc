<?php
/**
 * @file
 * drupalfr_media.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalfr_media_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function drupalfr_media_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_scald_default_context_types().
 */
function drupalfr_media_scald_default_context_types() {
  $contexts = array();
  $contexts['deleted-audio'] = array(
    'player' => array(
      'audio' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'audio' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['deleted-file'] = array(
    'player' => array(
      'file' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'file' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['file_representation-audio'] = array(
    'player' => array(
      'audio' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'audio' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['file_representation-file'] = array(
    'player' => array(
      'file' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'file' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['full-audio'] = array(
    'player' => array(
      'audio' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'audio' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['full-file'] = array(
    'player' => array(
      'file' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'file' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['invalid-id-audio'] = array(
    'player' => array(
      'audio' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'audio' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['invalid-id-file'] = array(
    'player' => array(
      'file' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'file' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['media-audio'] = array(
    'player' => array(
      'audio' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'audio' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['media-file'] = array(
    'player' => array(
      'file' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'file' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['no-access-audio'] = array(
    'player' => array(
      'audio' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'audio' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['no-access-file'] = array(
    'player' => array(
      'file' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'file' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['title-audio'] = array(
    'player' => array(
      'audio' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'audio' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  $contexts['title-file'] = array(
    'player' => array(
      'file' => array(
        '*' => 'default',
      ),
    ),
    'transcoder' => array(
      'file' => array(
        '*' => 'passthrough',
      ),
    ),
  );
  return $contexts;
}
