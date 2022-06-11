<?php

// phpcs:ignoreFile

declare(strict_types = 1);

use DrupalFinder\DrupalFinder;
use Rector\Core\Configuration\Option;
use Rector\PHPUnit\Rector\Class_\AddProphecyTraitRector;
use Rector\PHPUnit\Rector\ClassMethod\AddDoesNotPerformAssertionToNonAssertingTestRector;
use Symfony\Component\DependencyInjection\Loader\Configurator\ContainerConfigurator;

return static function (ContainerConfigurator $containerConfigurator): void {
  $containerConfigurator->import('../../../vendor/palantirnet/drupal-rector/config/drupal-8/drupal-8-all-deprecations.php');
  $containerConfigurator->import('../../../vendor/palantirnet/drupal-rector/config/drupal-9/drupal-9-all-deprecations.php');

  $parameters = $containerConfigurator->parameters();

  $drupalFinder = new DrupalFinder();
  $drupalFinder->locateRoot(__DIR__);
  $drupalRoot = $drupalFinder->getDrupalRoot();
  $parameters->set(Option::AUTOLOAD_PATHS, [
    $drupalRoot . '/core',
    $drupalRoot . '/modules',
    $drupalRoot . '/profiles',
    $drupalRoot . '/themes',
  ]);

  $parameters->set(Option::SKIP, [
    AddDoesNotPerformAssertionToNonAssertingTestRector::class,
    AddProphecyTraitRector::class,
    // This path is used by the upgrade_status module.
    '*/upgrade_status/tests/modules/*',
    // If you would like to skip test directories, uncomment the following lines:
    // '*/tests/*',
    // '*/Tests/*',
  ]);

  $parameters->set(Option::FILE_EXTENSIONS, [
    'engine',
    'inc',
    'install',
    'module',
    'php',
    'profile',
    'theme',
  ]);

  // Create `use` statements.
  $parameters->set(Option::AUTO_IMPORT_NAMES, FALSE);
  // Do not convert `\Drupal` to `Drupal`, etc.
  $parameters->set(Option::IMPORT_SHORT_CLASSES, FALSE);

  // This will add comments to call out edge cases in replacements.
  $parameters->set('drupal_rector_notices_as_comments', TRUE);
};
