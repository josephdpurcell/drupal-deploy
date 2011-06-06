<?php

class DeployPlan {

  var $processor = NULL;

  var $endpointObjects = array();

  function __construct(DeployProcessor $processor) {
    $this->processor = $processor;
  }

  function addEndpoint(DeployEndpoint $endpoint) {
    // The 'endpoints' property is already used bu the schema API.
    $this->endpointObjects[] = $endpoint;
  }

  function deploy($endpoint_name = NULL) {
    foreach ($this->endpointObjects as $endpoint) {
      if (empty($endpoint_name)) {
        $this->processor->process($endpoint);
      }
      elseif ($endpoint->name == $endpoint_name) {
        $this->processor->process($endpoint);
      }
    }
  }

}
