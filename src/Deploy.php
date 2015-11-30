<?php

namespace Drupal\deploy;

use Relaxed\Replicator\ReplicationTask;
use Relaxed\Replicator\Replication;
use Doctrine\CouchDB\CouchDBClient;
use Drupal\multiversion\Workspace\WorkspaceManagerInterface;

/**
 * Class Deploy
 * @package Drupal\deploy
 */
class Deploy implements DeployInterface {

  /**
   * @var \Drupal\multiversion\Workspace\WorkspaceManagerInterface
   */
  protected $workspaceManager;

  /**
   * @param \Drupal\multiversion\Workspace\WorkspaceManagerInterface $workspace_manager
   */
  public function __construct(WorkspaceManagerInterface $workspace_manager) {
    $this->workspaceManager = $workspace_manager;
  }

  /**
   * @param $source_domain
   * @param array $configuration
   * @return CouchDBClient
     */
  public function createSource($source_domain, array $configuration = []) {
    // Parse the source domain
    $source_domain_parts = parse_url($source_domain);

    // Split the database name from the path
    $path = explode('/', $source_domain_parts['path']);
    $dbname = array_pop($path);
    $path = trim(implode('/', $path), '/');

    // Create the source client
    $source = CouchDBClient::create([
      'host' => $source_domain_parts['host'],
      'path' => $path,
      'port' => !empty($source_domain_parts['port']) ? $source_domain_parts['port'] : 80,
      'user' => $source_username,
      'password' => $source_password,
      'dbname' => $dbname,
      'timeout' => 10
    ]);

    return $source;
  }

  /**
   * @param $target_domain
   * @param array $configuration
   * @return CouchDBClient
     */
  public function createTarget($target_domain, array $configuration = []) {
    // Parse the source domain
    $target_domain_parts = parse_url($target_domain);

    // Split the database name from the path
    $path = explode('/', $target_domain_parts['path']);
    $dbname = array_pop($path);
    $path = trim(implode('/', $path), '/');

    // Create the source client
    $target = CouchDBClient::create([
      'host' => $target_domain_parts['host'],
      'path' => $path,
      'port' => !empty($target_domain_parts['port']) ? $target_domain_parts['port'] : 80,
      'user' => $target_username,
      'password' => $target_password,
      'dbname' => $dbname,
      'timeout' => 10
    ]);

    return $target;
  }

  /**
   * @param CouchDBClient $source
   * @param CouchDBClient $target
   * @return array
     */
  public function push(CouchDBClient $source, CouchDBClient $target) {

    try {
      // Create the replication task
      $task = new ReplicationTask();
      // Create the replication
      $replication = new Replication($source, $target, $task);
      // Generate and set a replication ID
      $replication->task->setRepId($replication->generateReplicationId());
      // Start the replication
      $replicationResult = $replication->start();
    }
    catch (\Exception $e) {
      \Drupal::logger('Deploy')->info($e->getMessage() . ': ' . $e->getTraceAsString());
      return ['error' => $e->getMessage()];
    }
    // Return the response
    return $replicationResult;
  }

}