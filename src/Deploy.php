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
   * {@inheritdoc}
   */
  public function createSource(EndpointInterface $source) {
    // Create the source client
    $source_client = CouchDBClient::create([
      'url' => $source->getPlugin(),
      'timeout' => 10
    ]);

    return $source_client;
  }

  /**
   * {@inheritdoc}
   */
  public function createTarget(EndpointInterface $target) {
    // Create the source client
    $target = CouchDBClient::create([
      'url' => $target->getPlugin(),
      'timeout' => 10
    ]);

    return $target;
  }

  /**
   * {@inheritdoc}
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