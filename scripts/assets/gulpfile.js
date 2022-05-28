////////////////////////////////////////////////////////////////////////////////
// Per project variables.
////////////////////////////////////////////////////////////////////////////////

const themeName = "myproject_theme";

////////////////////////////////////////////////////////////////////////////////
// Gulp initialization.
////////////////////////////////////////////////////////////////////////////////

const gulp = require('gulp');
const autoprefixer = require('gulp-autoprefixer');
const babel = require('gulp-babel');
const clean = require('gulp-clean');
const imagemin = require('gulp-imagemin');
const mode = require('gulp-mode')({
  modes: ['production', 'development'],
  default: 'development',
  verbose: false
});
const rename = require('gulp-rename');
const sass = require('gulp-sass')(require('sass'));
const sassGlob = require('gulp-sass-glob');
const sourcemaps = require('gulp-sourcemaps');
const uglify = require('gulp-uglify');
const spritesmith = require('gulp.spritesmith');
const merge = require('merge-stream');
const browserSync = require('browser-sync').create();

////////////////////////////////////////////////////////////////////////////////
// Options and variables.
////////////////////////////////////////////////////////////////////////////////

const srcCleanOptions = {
  read: false
};
const cleanOptions = {
  force: true
};
const babelOptions = {
  presets: ['@babel/env']
};
const imageMinSvgoOptions = {
  plugins: [
    { removeUselessDefs: false },
    { cleanupIDs: false},
    { removeViewBox: false}
  ]
};
const sassOptions = {
  outputStyle: (mode.production() ? 'compressed' : 'expanded')
};
const uglifyOptions = {
  compress: {
    drop_debugger: !!mode.production()
  }
};
const browsersyncOptions = {
  open: false,
  proxy: {
    target: 'http://web'
  },
  ui: false,
};

const customModulesPath = '../../app/modules/custom';
const customThemesPath = '../../app/themes/custom';
const webThemesPath = '/themes/custom';
const projectThemePath = customThemesPath + '/' + themeName;
const webProjectThemePath = webThemesPath + '/' + themeName;

const paths = {
  myproject_theme: {
    styles: {
      src: projectThemePath + '/assets/scss/**/*.{scss,sass}',
      dest: projectThemePath + '/assets/css'
    },
    patternsStyles: {
      src: projectThemePath + '/templates/patterns/**/styles/*.{scss,sass}'
    },
    scripts: {
      src: projectThemePath + '/assets/js/**/*.es6.js'
    },
    patternsScripts: {
      src: projectThemePath + '/templates/patterns/**/js/*.es6.js'
    },
    images: {
      clean: projectThemePath + '/assets/images/optimized/**/*.{png,jpg,gif,svg}',
      src: projectThemePath + '/assets/images/source/**/*.{png,jpg,gif,svg}',
      dest: projectThemePath + '/assets/images/optimized'
    },
    sprite: {
      src: projectThemePath + '/assets/images/source/sprite/*.png',
      imgFilename: 'sprite.png',
      imgDest: projectThemePath + '/assets/images/source/',
      cssFilename: '_sprite.scss',
      cssDest: projectThemePath + '/assets/scss/',
      cssPath: webProjectThemePath + '/assets/images/optimized/sprite.png'
    }
  },
  myproject_modules: {
    styles: {
      src: customModulesPath + '/**/assets/styles/**/*.{scss,sass}',
    },
    patternsStyles: {
      src: customModulesPath + '/**/templates/patterns/**/styles/*.{scss,sass}'
    },
    scripts: {
      src: customModulesPath + '/**/assets/js/**/*.es6.js'
    },
    patternsScripts: {
      src: customModulesPath + '/**/templates/patterns/**/js/*.es6.js'
    }
  }
};

////////////////////////////////////////////////////////////////////////////////
// Task definitions.
////////////////////////////////////////////////////////////////////////////////

// Theme tasks
gulp.task('styles_theme', () => compileSass(paths['myproject_theme'].styles));
gulp.task('pattern_styles_theme', () => compileSass(paths['myproject_theme'].patternsStyles));
gulp.task('es6scripts_theme', () => compileES6(paths['myproject_theme'].scripts));
gulp.task('pattern_es6scripts_theme', () => compileES6(paths['myproject_theme'].patternsScripts));
gulp.task('clean_images_theme', () => themeCleanImages('myproject_theme'));
gulp.task('build_sprite_theme', () => themeBuildSprite('myproject_theme'));
gulp.task('build_images_theme', () => themeBuildImages('myproject_theme'));

gulp.task('images', gulp.series(
  'clean_images_theme',
  'build_sprite_theme',
  'build_images_theme'
));

// Custom modules tasks.
gulp.task('styles_modules', () => compileSass(paths['myproject_modules'].styles));
gulp.task('pattern_styles_modules', () => compileSass(paths['myproject_modules'].patternsStyles));
gulp.task('es6scripts_modules', () => compileES6(paths['myproject_modules'].scripts));
gulp.task('pattern_es6scripts_modules', () => compileES6(paths['myproject_modules'].patternsScripts));

// Watch.
gulp.task('watch', function (done) {
  if (mode.production()) {
    return done();
  }

  browserSync.init(browsersyncOptions);

  gulp.watch(paths['myproject_theme'].styles.src, gulp.series('styles_theme'));
  gulp.watch(paths['myproject_theme'].patternsStyles.src, gulp.series('pattern_styles_theme'));
  gulp.watch(paths['myproject_theme'].scripts.src, gulp.series('es6scripts_theme'));
  gulp.watch(paths['myproject_theme'].patternsScripts.src, gulp.series('pattern_es6scripts_theme'));
  gulp.watch(paths['myproject_theme'].images.src, gulp.series(
    'images',
    'styles_theme',
    'pattern_styles_theme'
  ));
  gulp.watch(paths['myproject_modules'].styles.src, gulp.series('styles_modules'));
  gulp.watch(paths['myproject_modules'].patternsStyles.src, gulp.series('pattern_styles_modules'));
  gulp.watch(paths['myproject_modules'].scripts.src, gulp.series('es6scripts_modules'));
  gulp.watch(paths['myproject_modules'].patternsScripts.src, gulp.series('pattern_es6scripts_modules'));
});

// Default task.
gulp.task('default', gulp.series(
  'images',
  'styles_theme',
  'pattern_styles_theme',
  'es6scripts_theme',
  'pattern_es6scripts_theme',
  'styles_modules',
  'pattern_styles_modules',
  'es6scripts_modules',
  'pattern_es6scripts_modules',
  'watch'
));

////////////////////////////////////////////////////////////////////////////////
// Functions.
////////////////////////////////////////////////////////////////////////////////

// Sass.
function compileSass(dir) {
  return gulp.src(dir.src)
    .pipe((mode.development(sourcemaps.init({}))))
    .pipe(sassGlob())
    .pipe((mode.production(sass(sassOptions))))
    .pipe((mode.development(sass(sassOptions).on('error', sass.logError))))
    .pipe(autoprefixer())
    .pipe((mode.development(sourcemaps.write('.'))))
    .pipe(gulp.dest(dir.dest ? dir.dest : (file) => file.base))
    .pipe((mode.development(browserSync.stream())));
}

// ES6.
function compileES6(dir) {
  return gulp.src(dir.src)
    .pipe((mode.development(sourcemaps.init())))
    .pipe(babel(babelOptions))
    .pipe(uglify(uglifyOptions))
    .pipe(rename(path => {
      path.basename = path.basename.replace('.es6', '');
    }))
    .pipe((mode.development(sourcemaps.write('.'))))
    .pipe(gulp.dest(file => file.base))
    .pipe((mode.development(browserSync.stream())));
}

// Images.
function themeCleanImages(themePathKey) {
  return gulp.src(paths[themePathKey].images.clean, srcCleanOptions)
    .pipe(clean(cleanOptions));
}
function themeBuildImages(themePathKey) {
  return gulp.src(paths[themePathKey].images.src)
    .pipe(imagemin([
      imagemin.svgo(imageMinSvgoOptions)
    ]))
    .pipe(gulp.dest(paths[themePathKey].images.dest))
    .pipe((mode.development(browserSync.stream())));
}
function themeBuildSprite(themePathKey) {
  var spriteData = gulp.src(paths[themePathKey].sprite.src)
    .pipe(spritesmith({
      imgName: paths[themePathKey].sprite.imgFilename,
      cssName: paths[themePathKey].sprite.cssFilename,
      imgPath: paths[themePathKey].sprite.cssPath,
      padding: 20
    }));

  var imgStream = spriteData.img
    .pipe(gulp.dest(paths[themePathKey].sprite.imgDest))
    .pipe((mode.development(browserSync.stream())));

  var cssStream = spriteData.css
    .pipe(gulp.dest(paths[themePathKey].sprite.cssDest))
    .pipe((mode.development(browserSync.stream())));

  return merge(imgStream, cssStream);
}
