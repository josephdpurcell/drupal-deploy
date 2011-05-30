<?php

class DeployProcessorQueue implements DeployProcessor {

  var $provider = NULL;

  function __construct(DeployProvider $provider) {
    $this->provider = $provider;
  }

  function process(DeployEndpoint $endpoint) {
    foreach ($this->provider->getResources() as $resource) {
      // @todo Add resource and endpoint to queue.
    }
  }

}
