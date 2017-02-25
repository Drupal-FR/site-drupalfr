# Drupal.fr Front theme

## Requirements

* npm
* gulp

## Installation

In the theme folder (`PROJECT/www/profiles/drupalfr/themes/drupalfr_theme`) 

Execute:

`$ npm init`

`$ gulp`

## Troubleshooting

If you experience a issue involving the *gaze* module, using GNU/Linux check this [URL](https://discourse.roots.io/t/gulp-watch-error-on-ubuntu-14-04-solved/3453/3)
  
 Or simply increase your inotify max memory :

 `echo fs.inotify.max_user_watches=582222 | sudo tee -a /etc/sysctl.conf && sudo sysctl -p`
