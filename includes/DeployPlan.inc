<?php

class DeployPlan {

  var $processor = NULL;

  var $endpoints = array();

  function __construct(DeployProcessor $processor) {
    $this->processor = $processor;
  }

  function deploy() {
    foreach ($this->endpoints as $endpoint_name) {
      $this->processor->process($endpoint_name);
    }
  }

}
