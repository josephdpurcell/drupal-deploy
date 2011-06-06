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

  function deploy($endpoint_name = NULL) {
    foreach ($this->endpoints as $endpoint) {
      if (empty($endpoint_name)) {
        $this->processor->process($endpoint);
      }
      elseif ($endpoint->name == $endpoint_name) {
        $this->processor->process($endpoint);
      }
    }
  }

}
