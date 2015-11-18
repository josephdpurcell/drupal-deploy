<?php

/**
 * @file
 * Contains \Drupal\Tests\deploy\Unit\DeployTest;
 */

namespace Drupal\deploy\Tests;

use Drupal\KernelTests\KernelTestBase;

/**
 * @group deploy
 */
class DeployTest extends KernelTestBase {

  protected $strictConfigSchema = FALSE;

  /**
   * {@inheritdoc}
   */
  public static $modules = ['serialization', 'system', 'rest', 'key_value', 'multiversion', 'relaxed', 'deploy'];

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();
    $this->installConfig(['multiversion', 'relaxed', 'deploy']);
  }

  /**
   * Should always return true.
   */
  public function testDeploy() {
    $response = \Drupal::service('deploy.deploy')->push('http://localhost:8081', 'admin', 'admin', 'default');
    $this->assertTrue($response, "Migration complete");
  }

}
