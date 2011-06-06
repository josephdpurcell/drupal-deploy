<?php

/**
 * @file
 * This is not a full plugin, but an abstract class that plugins can extend.
 */

abstract class DeployServiceRest implements DeployService {

  function __construct(Array $config = array()) {}

  function deploy(Array $entities) {}

}
