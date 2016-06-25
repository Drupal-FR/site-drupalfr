var gulp = require('gulp'),
    nunjucksRender = require('gulp-nunjucks-render'),
    data = require('gulp-data');

gulp.task('nunjucks', function() {
  return gulp.src('app/pages/**/*.+(html|nunjucks)')
  // Adding data to Nunjucks
    .pipe(data(function() {
      return require('./app/data.json')
    }))
    .pipe(nunjucksRender({
      path: ['app/templates']
    }))
    .pipe(gulp.dest('app'))
});

gulp.task('watch', function () {
  gulp.watch(['./app/data.json', './app/**/*.nunjucks'], ['nunjucks']);
});
