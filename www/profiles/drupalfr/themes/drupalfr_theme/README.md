# Drupal France Theme

Below are instructions on how to  Bootstrap sub-theme using a Sass
preprocessor.

## Prerequisites
- [Node.js](https://nodejs.org/en/download/) 5.x+
- [npm] (npm 3.x.x should de included in the Node.js installer)

## Install / Initialization 
We use [Sass](http://sass-lang.com/) with the [SCSS](http://sass-lang.com/documentation/file.SASS_REFERENCE.html#syntax) `.scss` syntax.  
We need to install the following packages :
- [Gulp](http://gulpjs.com/)
- [Gulp Sass](https://www.npmjs.com/package/gulp-sass)
- [Gulp Sourcemaps](https://www.npmjs.com/package/gulp-sourcemaps)
- [Gulp Watch](https://www.npmjs.com/package/gulp-watch)

```(bash)
$ cd ./assets
$ npm install gulp --save-dev
$ npm install gulp-sass --save-dev
$ npm install gulp-sourcemaps --save-dev
$ npm install gulp-watch --save-dev
```


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
When developing, start the gulp watcher in the assets folder, it will recompile your SCSS files each time you modify them.

```(bash)
$ cd ./assets/
$ gulp
```

## Troubleshooting

If you experience a issue involving the *gaze* module, using GNU/Linux check this [URL](https://discourse.roots.io/t/gulp-watch-error-on-ubuntu-14-04-solved/3453/3)    
Or simply increase your inotify max memory :

 `echo fs.inotify.max_user_watches=582222 | sudo tee -a /etc/sysctl.conf && sudo sysctl -p`

If you have an error about a missing bracket with the sourcemaps package, check you node.js version  
 ```
 $ node -v
 ```
May be your node was installed from your official OS repository and its version is 0.10.x  
Just update it.


## Question ?
Feel free to send an email to [floris.moriceau@komuneco.org](floris.moriceau@komuneco.org)
