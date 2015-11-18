<?php

namespace Drupal\deploy;

use Relaxed\Replicator\ReplicationTask;
use Relaxed\Replicator\Replication;
use Doctrine\CouchDB\CouchDBClient;
use Drupal\multiversion\Workspace\WorkspaceManagerInterface;

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
   * @param $target_domain
   * @param $target_workspace
   * @return array
   * @throws \Doctrine\CouchDB\HTTP\HTTPException
   */
  public function push($target_domain, $target_username, $target_password, $target_workspace) {
    // Get url for current site
    global $base_url;
    // Parse the base url
    $base_url_parts = parse_url($base_url);
    // Parse the target domain
    $target_domain_parts = parse_url($target_domain);
    // Use current active workspace as the source
    $source_workspace = $this->workspaceManager->getActiveWorkspace()->id();
    // Create the source client
    $source = CouchDBClient::create([
        'host' => $base_url_parts['host'],
        'path' => 'relaxed',
        'port' => $target_domain_parts['port'],
        'user' => $target_username,
        'password' => $target_password,
        'dbname' => $source_workspace,
        'timeout' => 10
    ]);
    // Create the target client
    $target = CouchDBClient::create([
        'host' => $target_domain_parts['host'],
        'path' => 'relaxed',
        'port' => $target_domain_parts['port'],
        'user' => $target_username,
        'password' => $target_password,
        'dbname' => $target_workspace,
        'timeout' => 10
    ]);
    // Create the replication task
    $task = new ReplicationTask();
    // Create the replication
    $replication = new Replication($source, $target, $task);
    // Generate and set a replication ID
    $replication->task->setRepId($replication->generateReplicationId());
    // Start the replication
    $replicationResult = $replication->start();
    // Return the response
    return $replicationResult;
  }

}