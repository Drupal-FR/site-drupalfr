// Override gulp options here rather than in gulpfile.js.

// Set a drush alias if required to run locally, i.e.:
// '@multisite.local --uri=multisitename'
var drush_alias = '';

module.exports = {
  // set 'enabled: true' to run drush commands as a part of 'gulp watch'.
  drush: {
    enabled: false,
    alias: {
      css_js: 'drush ' + drush_alias + ' cc css-js',
      cr: 'drush ' + drush_alias + ' cr'
    }
  },

  // Override sass compile options.
  /*
  scss: {
    outputStyle: 'expanded',
    lintIgnore: '',
  },
  */

  // If your files are on a network share, you may want to turn on polling for
  // Gulp watch. Since polling is less efficient, we disable polling by default.
  /*
  gulpWatchOptions: {
    interval: 1000,
    mode: 'poll'
  },
  */

};
