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
    'data' => array(
      'audio' => array(),
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
    'data' => array(
      'file' => array(),
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
    'data' => array(
      'audio' => array(),
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
    'data' => array(
      'file' => array(),
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
    'data' => array(
      'audio' => array(),
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
    'data' => array(
      'file' => array(),
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
    'data' => array(
      'audio' => array(),
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
    'data' => array(
      'file' => array(),
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
    'data' => array(
      'audio' => array(),
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
    'data' => array(
      'file' => array(),
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
    'data' => array(
      'audio' => array(),
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
    'data' => array(
      'file' => array(),
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
    'data' => array(
      'audio' => array(),
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
    'data' => array(
      'file' => array(),
    ),
  );
  return $contexts;
}
