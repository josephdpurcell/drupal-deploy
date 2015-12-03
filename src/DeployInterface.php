<?php

namespace Drupal\deploy;

use Doctrine\CouchDB\CouchDBClient;
use Drupal\relaxed\Entity\EndpointInterface;

/**
 * Interface DeployInterface
 * @package Drupal\deploy
 */
interface DeployInterface {

  /**
   * @param \Drupal\relaxed\Entity\EndpointInterface $source
   * @return \Doctrine\CouchDB\CouchDBClient
   */
  public function createSource(EndpointInterface $source);

  /**
   * @param \Drupal\relaxed\Entity\EndpointInterface $target
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