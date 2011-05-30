<?php

class DeployPlan {

  var $processor = NULL;

  var $endpoints = array();

  function __construct(DeployProcessor $processor) {
    $this->processor = $processor;
  }

  function addEndpoint(DeployEndpoint $endpoint) {
    $this->endpoints[] = $endpoint;
  }

  function deploy() {
    foreach ($this->endpoints as $endpoint) {
      $this->processor->process($endpoint);
    }
  }

}
