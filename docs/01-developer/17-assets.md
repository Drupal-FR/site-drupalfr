# Assets

## Gulpfile preparation (Only on theme initialization)

Edit `./scripts/assets/gulpfile.js` and replace `themeName` with your project
theme name.

## File structure

### Custom theme

Sass: `${APP_PATH}/themes/custom/${themeName}/assets/scss/**/*.{scss,sass}`

**Compiled in '${APP_PATH}/themes/custom/${themeName}/assets/css/...'**

JS: `${APP_PATH}/themes/custom/${themeName}/assets/js/**/*.es6.js`

Patterns Sass: `${APP_PATH}/themes/custom/${themeName}/templates/patterns/**/styles/*.{scss,sass}`

Patterns JS: `${APP_PATH}/themes/custom/${themeName}/templates/patterns/**/js/*.es6.js`

> Except for Sass, all compiled files will be in the same directory as the source file.

### Custom modules

Sass: `${APP_PATH}/modules/custom/**/assets/styles/**/*.{scss,sass}`

JS: `${APP_PATH}/modules/custom/**/assets/js/**/*.es6.js`

Patterns Sass: `${APP_PATH}/modules/custom/**/templates/patterns/**/styles/*.{scss,sass}`

Patterns JS: `${APP_PATH}/modules/custom/**/templates/patterns/**/js/*.es6.js`

> All compiled files will be in the same directory as the source file.

## Assets compilation

### With docker (recommended)

Depending on the target environment:

```bash
make docker-compile-assets
```

or

```bash
make docker-compile-assets-production
```

### With yarn

While being in your project directory:

```bash
yarn --cwd ./scripts/assets install
```

And depending on the target environment:

```bash
yarn --cwd ./scripts/assets run gulp-dev
```

or

```bash
yarn --cwd ./scripts/assets run gulp-prod
```

### Test compilation

You can edit `scripts/tests/gulp/test-gulp-compilation.sh` with all expected
compiled files then run

```bash
make tests-gulp
```

## Browsersync

A basic Browsersync support is provided.

Browsersync will output the URL to access it, like:
```bash
[Browsersync] Proxying: http://web
[Browsersync] Access URLs:
 ----------------------------------
    Local: http://localhost:3000
 External: http://192.168.96.2:3000
 ----------------------------------
```

Use the `External URL` to access Browsersync.

Edit the proxy target in `scripts/assets/gulpfile.js` if you need to target a
different website.
