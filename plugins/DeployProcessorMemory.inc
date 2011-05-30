<?php

class DeployProcessorMemory implements DeployProcessor {

  var $provider = NULL;

  function __construct(DeployProvider $provider) {
    $this->provider = $provider;
  }

  function process(DeployEndpoint $endpoint) {
    $endpoint->deploy($this->provider->getResources());
  }

}
