<?php

class DeployProcessorMemory implements DeployProcessor {

  var $provider = NULL;

  function __construct(DeployProvider $provider) {
    $this->provider = $provider;
  }

  function deploy(DeployEndpoint $endpoint) {
    $endpoint->render($this->provider->getResources());
  }

}
