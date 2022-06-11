<?php

/**
 * This file is included very early. See autoload.files in composer.json and
 * https://getcomposer.org/doc/04-schema.md#files
 */

use Dotenv\Dotenv;

/**
 * Load any .env file. See /.env.example.
 */
// Load using createMutable to ensure .env file is parsed again. Otherwise
// Docker Composer environment variables could integer quotes or double quotes
// where we want strings.
$dotenv = Dotenv::createMutable(__DIR__);
$dotenv->safeLoad();
