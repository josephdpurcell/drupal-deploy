<?php

namespace Drupal\deploy;

use Doctrine\CouchDB\CouchDBClient;

interface DeployInterface {

  /**
   * @return \Doctrine\CouchDB\CouchDBClient
   */
  public function createSource($source_domain, $source_username, $source_password);

  /**
   * @return \Doctrine\CouchDB\CouchDBClient
   */
  public function createTarget($target_domain, $target_username, $target_password);

  /**
   * @param \Doctrine\CouchDB\CouchDBClient $source
   * @param \Doctrine\CouchDB\CouchDBClient target
   */
  public function push(CouchDBClient $source, CouchDBClient $target);

}