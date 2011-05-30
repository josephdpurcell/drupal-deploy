<?php

class DeploySetup {

  var $worker = NULL;

  var $endpoints = array();

  function __construct(DeployWorker $worker) {
    $this->worker = $worker;
  }

  function addEndpoint(DeployEndpoint $endpoint) {
    $this->endpoints[] = $endpoint;
  }

  function deploy() {
    foreach ($this->endpoints as $endpoint) {
      $worker->deploy($endpoint);
    }
  }

}
