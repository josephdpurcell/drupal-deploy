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
  public function push($target_domain, $target_workspace) {
    global $base_url;
    $base_url_parts = parse_url($base_url);
    $target_domain_parts = parse_url($target_domain);
    $source_workspace = $this->workspaceManager->getActiveWorkspace()->id();
    $source = CouchDBClient::create(array('host' => $base_url_parts['host'], 'dbname' => $source_workspace, 'port' => 80));
    $target = CouchDBClient::create(array('host' => $target_domain_parts['host'], 'dbname' => $target_workspace, 'port' => 80));
    $task = new ReplicationTask();
    $task->setCreateTarget(true);
    $replication = new Replication($source, $target, $task);
    $replication->task->setRepId($replication->generateReplicationId());
    $replicationResult = $replication->start();
    return $replicationResult;
  }

}