<?php

namespace Drupal\deploy;

use Doctrine\CouchDB\CouchDBClient;

/**
 * Interface DeployInterface
 * @package Drupal\deploy
 */
interface DeployInterface {

  /**
   * @return \Doctrine\CouchDB\CouchDBClient
   */
  public function createSource($source_domain, array $configuration);

  /**
   * @return \Doctrine\CouchDB\CouchDBClient
   */
  public function createTarget($target_domain, array $configuration);

  /**
   * @param \Doctrine\CouchDB\CouchDBClient $source
   * @param \Doctrine\CouchDB\CouchDBClient target
   */
  public function push(CouchDBClient $source, CouchDBClient $target);

}