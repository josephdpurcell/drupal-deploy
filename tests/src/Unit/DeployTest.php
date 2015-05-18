<?php

/**
 * @file
 * Contains \Drupal\Tests\deploy\Unit\DeployTest;
 */

namespace Drupal\Tests\deploy\Unit;

use Drupal\Tests\UnitTestCase;

/**
 * @coversDefaultClass \Drupal\deploy\Controller\DeployController
 *
 * @group deploy
 */
class DeployTest extends UnitTestCase {

  /**
   * Should always return true.
   *
   * @covers ::admin()
   */
  public function testAdmin() {
    $this->assertTrue(true);
  }

}
