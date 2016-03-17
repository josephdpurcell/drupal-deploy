<?php

/**
 * @file
 * Contains \Drupal\deploy\Tests\ReplicationEntityTest.
 */

namespace Drupal\deploy\Tests;

use Drupal\multiversion\Entity\Workspace;
use Drupal\simpletest\WebTestBase;

/**
 * Test the replication entity.
 *
 * @group deploy
 */
class ReplicationEntityTest extends WebTestBase {

  protected $strictConfigSchema = FALSE;

  public static $modules = ['deploy'];

  public function setUp() {
    parent::setUp();
    Workspace::create(['type' => 'basic', 'label' => 'Development', 'machine_name' => 'development'])->save();
  }

  public function testSpecialCharacters() {
    $this->webUser = $this->drupalCreateUser([
      'administer deployments',
    ]);

    $this->drupalLogin($this->webUser);
    $this->drupalGet('admin/structure/deployment/add');
    $deployment = [
      'name[0][value]' => 'Test Deployment',
      'source' => '1',
      'target' => '2',
    ];
    $this->drupalPostForm('admin/structure/deployment/add', $deployment, t('Review'));

    $this->drupalGet('admin/structure/deployment');
    $this->assertText($deployment['name[0][value]'], 'Deployment found in list of deployments');
  }
}