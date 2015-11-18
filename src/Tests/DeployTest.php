<?php

/**
 * @file
 * Contains \Drupal\Tests\deploy\Unit\DeployTest;
 */

namespace Drupal\deploy\Tests;

use Drupal\Tests\UnitTestCase;

/**
 * @group deploy
 */
class DeployTest extends UnitTestCase {

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();
  }

  /**
   * Should always return true.
   */
  public function testAdmin() {
    $this->assertEquals(1, 1, "test");
  }

}
