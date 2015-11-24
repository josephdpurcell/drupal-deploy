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
   * Test deploying from CouchDB to CouchDB.
   */
  public function testDeployCouchDB() {
    $source = $this->deploy->createSource('http://localhost:5984/source');
    $target = $this->deploy->createTarget('http://localhost:5984/target');

    // Create the source and the target databases.
    $source->createDatabase('source');
    $target->createDatabase('target');

    // Add three docs to the source db.
    for ($i = 0; $i < 3; $i++) {
      list($id, $rev) = $source->putDocument(
          array("foo" => "bar" . var_export($i, true)),
          'id' . var_export($i, true)
      );
    }

    $result = $this->deploy->push($source, $target);

    $this->assertTrue(!isset($result['error']), 'Successful migration.');
  }

  /**
   * Test deploying from Drupal to Drupal.
   */
  public function testDeployDrupal() {
    $source = $this->deploy->createSource('http://localhost:8080/relaxed/default');
    $target = $this->deploy->createTarget('http://localhost:8081/relaxed/default');

    $result = $this->deploy->push($source, $target);

    $this->assertTrue(!isset($result['error']), 'Successful migration.');
  }

}
