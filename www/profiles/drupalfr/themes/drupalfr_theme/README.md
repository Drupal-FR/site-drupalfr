# Drupal France Theme

Below are instructions on how to  Bootstrap sub-theme using a Sass
preprocessor.

## Prerequisites
- [Node.js](https://nodejs.org/en/download/) 5.x+
- [npm] (npm 3.x.x should de included in the Node.js installer)

## Folder Structure 
- assets
  - **bootstrap**  
      Bootstrap Framework Source Files **DO NOT MODIFY**  
  - **css**  
      Compiled CSS files destination folder.  
      These files will be called by the theme libraries.
  - **images**  
      Non contrib images (backgrounds, logos, icons...)
  - **node_module**  
      npm installed modules (Gulp and co)
  - **scss**  
      Theme source files. **Here we work**. 

## Assets Processing
We use [Sass](http://sass-lang.com/) with the [SCSS](http://sass-lang.com/documentation/file.SASS_REFERENCE.html#syntax) `.scss` syntax.  
Are pre-installed (in the `node_module` folder) :
- [Gulp](http://gulpjs.com/)
- [Gulp Sass](https://www.npmjs.com/package/gulp-sass)
- [Gulp Sourcemaps](https://www.npmjs.com/package/gulp-sourcemaps)
- [Gulp Watch](https://www.npmjs.com/package/gulp-watch)

When developing, start the gulp watcher in the assets folder, it will recompile your SCSS files each time you modify them.

```(bash)
$ cd ./assets/
$ npm init
$ gulp
```

## Troubleshooting

If you experience a issue involving the *gaze* module, using GNU/Linux check this [URL](https://discourse.roots.io/t/gulp-watch-error-on-ubuntu-14-04-solved/3453/3)    
Or simply increase your inotify max memory :

 `echo fs.inotify.max_user_watches=582222 | sudo tee -a /etc/sysctl.conf && sudo sysctl -p`
 
## Question ?
Feel free to send an email to [floris.moriceau@komuneco.org](floris.moriceau@komuneco.org)
