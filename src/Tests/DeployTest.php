<?php

/**
 * @file
 * Contains \Drupal\Tests\deploy\Unit\DeployTest;
 */

namespace Drupal\deploy\Tests;

use Drupal\KernelTests\KernelTestBase;
use Doctrine\CouchDB\CouchDBClient;

/**
 * @group deploy
 */
class DeployTest extends KernelTestBase {

  protected $strictConfigSchema = FALSE;

  /**
   * {@inheritdoc}
   */
  public static $modules = ['serialization', 'system', 'rest', 'key_value', 'multiversion', 'relaxed', 'deploy'];

  protected $deploy;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();
    $this->installConfig(['multiversion', 'relaxed', 'deploy']);
    $this->deploy = \Drupal::service('deploy.deploy');

  }

  /**
   * Should always return true.
   */
  public function testDeployCouchDB() {
    $source = $this->deploy->createSource('http://localhost:5984/source');
    $target = $this->deploy->createTarget('http://localhost:5984/target');
    $result = $this->deploy->push($source, $target);

    $this->assertTrue(!isset($result['error']), 'Successful migration.');
  }

}
