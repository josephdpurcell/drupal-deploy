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
  public function createSource(EndpointInterface $source);

  /**
   * @param EndpointInterface $target
   * @return \Doctrine\CouchDB\CouchDBClient
   */
  public function createTarget(EndpointInterface $target);

  /**
   * @param \Doctrine\CouchDB\CouchDBClient $source
   * @param \Doctrine\CouchDB\CouchDBClient target
   * @return array
   */
  public function push(CouchDBClient $source, CouchDBClient $target);

}